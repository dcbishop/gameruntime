#include "GameServer.hpp"

#include "../Main/Globals.hpp"
#include "../Main/PropertyClass.hpp"
#include "../GameServer/PacketWriter.hpp"

#include "../Debug/console.h"

void GameServer::start() {
   ip::tcp::socket::non_blocking_io non_blocking_io(true);
  
   // Open an IPv6 socket
   try {
      if(enableIPv6_) {
         // Open the sockets, for most sysems an IPv6 socket should also open IPv4 but not pre-Vista Windows...
         DEBUG_M("Opening IPv6 socket...");
         //socket6_ = new ip::udp::socket(io_service_, ip::udp::endpoint(ip::udp::v6(), getPort()));
         
         socket6_ = new ip::udp::socket(io_service_);
         ip::udp::endpoint ep6(ip::udp::v6(), getPort());
         //ip::udp::endpoint ep6();
         socket6_->open(udp::v6());
         if(!enableIPv4_) {
            DEBUG_M("Setting v6 only socket option.");
            socket6_->set_option(ip::v6_only(true));
         }
         socket6_->bind(ep6);
         DEBUG_M("Listening on port %d", socket6_->local_endpoint().port());
      }
   } catch (std::exception& e) {
       ERROR("Could not open IPv6 socket: %s", e.what());
   }

   // Find out if the IPv6 socket is dualstack
   if(socket6_) {
      ip::v6_only option;
      socket6_->get_option(option);
      isDualStack_ = !option.value();
   }
   
   // Open an IPv4 socket
   try {   
      if(enableIPv4_) {
         // If there is no IPv6 support or the socket doesn't support dual stack with IPv4, open a seperate IPv4 socket.
         if(!socket6_ || !isDualStack_) {
            DEBUG_M("Opening IPv4 socket...");
            socket4_ = new ip::udp::socket(io_service_, ip::udp::endpoint(ip::udp::v4(), getPort()));
         } else {
            DEBUG_M("Dual stack supported. Skipping IPv4.");
         }
      }
   } catch (std::exception& e) {
       ERROR("Could not open IPv4 socket: %s", e.what());
   }
   
   // Turn off socket blocking...
   if(socket4_) {
      socket4_->io_control(non_blocking_io);
   }
   if(socket6_) {
      socket6_->io_control(non_blocking_io);
   }
}

void GameServer::recieve() {
   // Keep processing received data untill there is no more...
   size_t bytes = receive_from_();
   while(bytes != 0) {
      LOG("%s: %s", last_sender_.address().to_string().c_str(), recv_buf_);
      bytes = receive_from_();
   }   
}

void GameServer::transmit() {
   ptime current_time = microsec_clock::universal_time();
   static ptime last_time = current_time;

   // Limit our transmit rate
   static time_duration transmit_rate_ = time_duration(0, 0, 0, time_duration::ticks_per_second()/transmits_per_second_);
   if(current_time - last_time < transmit_rate_) {
      return;
   }
   
   // Loop through all the tracked properties and send any that have a dirty network flag
   for(PropertyMap::iterator it = globals.getTransmissionList()->begin(); it!=globals.getTransmissionList()->end(); it++) {
      // If it's not market for transmit, skip it...
      if(!(it->second)->getTransmitFlag()) {
         continue;
      }
      // Remove the transmit flag...
      it->second->setTransmitFlag(false);

      unsigned int size = PacketWriter::set(send_buf_, buf_size, it->second->getSharedId(), it->second);
      send_buf_[size] = '\0';

      writeAll(send_buf_, size);
   }
   last_time = current_time;
}

/**
 * Broadcast a message to everyone.
 */
void GameServer::writeAll(const char* data, const unsigned int& size) {

   // TODO: DEBUG
#if DEBUG_LEVEL >= DEBUG_VERY_HIGH
   cout << "[SEND]: ";
   for(unsigned int i = 0; i < size; i++) {
      cout << hex << setfill ('0') << setw(8) << (int)data[i] << " ";
   }
   cout << endl;
#endif
  
   for(ConnectionsList::iterator it = connections_.begin(); it < connections_.end(); it++) {
      write(data, size, (*it));
   }
}

void GameServer::write(const char* data, const unsigned int& size, const udp::endpoint& destination) {
   //TODO: Choose the correct socket (if dual stack then just socket6_, otherwise we have to choose 4 for 4 clients);
   udp::socket* sockout;
   if(socket6_) {
      sockout = socket6_;
   } else if(socket4_) {
      sockout = socket4_;
   }

   boost::system::error_code  sockout_error;
   sockout->send_to(asio::buffer(data, size), destination, 0, sockout_error);
   if(sockout_error) {
      DEBUG_M("Send error '%s'...", sockout_error.message().c_str());
   }
}

void GameServer::addConnection(ip::udp::endpoint endpoint) {
   DEBUG_M("Adding connection '%s'", endpoint.address().to_string().c_str());
   connections_.push_back(endpoint);
}

void GameServer::addConnection(const string& address, const string& port) {
   LOG("Resolving '%s' '%s'", address.c_str(), port.c_str());
   try {
      udp::resolver resolver(io_service_);

      udp::resolver::query query(address, port);
      udp::resolver::iterator endpoint_iterator = resolver.resolve(query);
      udp::resolver::iterator end;

      // Iterate through all the endpoints and send a ping to find the working/best.
      int size = PacketWriter::ping(send_buf_, buf_size, 0);
      while (endpoint_iterator != end) {
         udp::endpoint endpoint = (*endpoint_iterator).endpoint();
         if(enableIPv6_ && (endpoint.protocol() == endpoint.protocol().v6())) {
            DEBUG_M("Found a IPv6: '%s'", endpoint.address().to_string().c_str());
            write(send_buf_, size, endpoint);
         } else if(enableIPv4_ && (endpoint.protocol() == endpoint.protocol().v4())) {
            DEBUG_M("Found a IPv4: '%s'", endpoint.address().to_string().c_str());
            write(send_buf_, size, endpoint);
         }
         endpoint_iterator++;
      }
      //addConnection((*endpoint_iterator).endpoint());
   } catch (std::exception& e) {
      ERROR("Failed to resolve: %s", e.what());
      return; // TODO: Write XMPP error message here
   }

   // Await the first response to our ping.
   ptime start_time = microsec_clock::universal_time();
   static time_duration timeout_delay = time_duration(0, 0, timeout_, 0);
   size_t bytes = receive_from_();
   while(bytes == 0) {
      bytes = receive_from_();
      // TODO: Throttle?...
      if(microsec_clock::universal_time() - start_time  > timeout_delay) {
         ERROR("Timed out...");
         throw "Timed out...";
         return;
      }
   }
   
   // Connect to the respondering endpoint.
   addConnection(last_sender_);
}



size_t GameServer::receive_from_() {
   if(socket6_) {
      try {
         size_t bytes = socket6_->receive_from(asio::buffer(recv_buf_), last_sender_);
         if(bytes > 0) {
            return bytes;
         }
         
      } catch(std::exception e) {
         // Nothing...
      }
   }

   if(socket4_) {
      try {
         size_t bytes = socket4_->receive_from(asio::buffer(recv_buf_), last_sender_);
         if(bytes > 0) {
            return bytes;
         }
      } catch(std::exception e) {
        // Nothing...
      }
   }

   return 0;
}
