#include "GameServer.hpp"

#include "../Main/Globals.hpp"
#include "../Main/PropertyClass.hpp"
#include "../GameServer/PacketWriter.hpp"

#include "../Debug/console.h"

void GameServer::start() {
   ip::tcp::socket::non_blocking_io non_blocking_io(true);
  
   if(enableIPv6_) {
      // Open the sockets, for most sysems an IPv6 socket should also open IPv4 but not pre-Vista Windows...
      DEBUG_M("Opening IPv6 socket...");
      socket6_ = new ip::udp::socket(io_service_, ip::udp::endpoint(ip::udp::v6(), getPort())); 
   }
   
   if(enableIPv4_) {
      ip::v6_only option;
      socket6_->get_option(option);

      // If there is no IPv6 socket or the socket doesn't support dual stack with IPv4, bring up a seperate IPv4 socket.
      if(!socket6_ || option.value()) {
         DEBUG_M("Opening IPv4 socket...");
         socket4_ = new ip::udp::socket(io_service_, ip::udp::endpoint(ip::udp::v4(), getPort()));
      } else {
         DEBUG_M("Dual stack supported. Skipping IPv4.");
      }
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
   ip::udp::endpoint sender_;
   if(socket6_) {
      try {
         size_t bytes = socket6_->receive_from(boost::asio::buffer(recv_buf_), sender_);
         LOG("%s: %s", sender_.address().to_string().c_str(), recv_buf_);
      } catch(std::exception e) {
         // Nothing...
      }
   }
   
   if(socket4_) {
      try {
         size_t bytes = socket4_->receive_from(boost::asio::buffer(recv_buf_), sender_);
         LOG("%s: %s", sender_.address().to_string().c_str(), recv_buf_);
      } catch(std::exception e) {
        // Nothing...
      }
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
   //DEBUG_M("%x", *data);
   boost::system::error_code  ignored_error;
   
   // TODO: DEBUG
#if DEBUG_LEVEL >= DEBUG_VERY_HIGH
   cout << "[SEND]: ";
   for(unsigned int i = 0; i < size; i++) {
      cout << hex << setfill ('0') << setw(8) << (int)data[i] << " ";
   }
   cout << endl;
#endif

   //TODO: Clean me up, choose the correct socket (if dual stack then just socket6_, otherwise we have to choose 4 for 4 clients);
   udp::socket* sockout;
   if(socket6_) {
      sockout = socket6_;
   } else if(socket4_) {
      sockout = socket4_;
   }
   
   for(ConnectionsList::iterator it = connections_.begin(); it < connections_.end(); it++) {
      DEBUG_M("SEND");
      sockout->send_to(asio::buffer(data, size), (*it), 0, ignored_error);
   }
   
   if(ignored_error) {
      DEBUG_M("Send error '%s'...", ignored_error.message().c_str());
   }
}

void GameServer::addConnection(const string& address, const string& port) {
   LOG("Resolving '%s' '%s'", address.c_str(), port.c_str());
   udp::resolver resolver(io_service_);
   udp::resolver::query query(address, port);
   udp::resolver::iterator endpoint_iterator = resolver.resolve(query);
   udp::resolver::iterator end;

   addConnection((*endpoint_iterator).endpoint());

   // TODO: Ping both IPv6 and IPv4 and select the one with lower latency (or the one that responds)...
   /*while (endpoint_iterator != end) {
      DEBUG_M("Found '%s'", (*endpoint_iterator).endpoint().address().to_string().c_str());
      endpoint_iterator++;
   }*/
}

void GameServer::addConnection(ip::udp::endpoint endpoint) {
   connections_.push_back(endpoint);
}
