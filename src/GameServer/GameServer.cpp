#include "GameServer.hpp"

#include "../Main/Globals.hpp"
#include "../Main/PropertyClass.hpp"

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
   for(PropertyMap::iterator it = globals.getTransmissionList()->begin(); it!=globals.getTransmissionList()->end(); it++) {
      // If it's not market for transmit, skip it...
      if(!(it->second)->getTransmitFlag()) {
         continue;
      }
      // Remove the transmit flag...
      it->second->setTransmitFlag(false);
      string a = "a" + it->second->toString();
      string data = "SET ";// + it->second->getSharedId() + it->second->toString();
      writeAll(data);
   }
}

void GameServer::writeAll(const string& data) {
   DEBUG_M("%s", data.c_str());
   boost::system::error_code  ignored_error;
   
   //TODO: Clean me up, choose the correct socket (if dual stack then just socket6_, otherwise we have to choose 4 for 4 clients);
   udp::socket* sockout;
   if(socket6_) {
      sockout = socket6_;
   } else if(socket4_) {
      sockout = socket4_;
   }
   
   for(ConnectionsList::iterator it = connections_.begin(); it < connections_.end(); it++) {
      DEBUG_M("SEND");
      sockout->send_to(asio::buffer(data), (*it), 0, ignored_error);
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
