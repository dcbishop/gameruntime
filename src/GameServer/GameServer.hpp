#ifndef GAMERUNTIME_GAMESERVER_HPP_
#define GAMERUNTIME_GAMESERVER_HPP_

#include <iostream>
using namespace std;

//TODO: Also support native asio...
#include <boost/asio.hpp>
using namespace boost::asio;

const int buf_size = 8192;

class GameServer {
   public:
      GameServer(): port_(11010) {
         start();
      }
      
      ~GameServer() {
         delete socket4_;
         delete socket6_;
      }
      
      void start() {
         ip::tcp::socket::non_blocking_io non_blocking_io(true);
         
         // Open the sockets, for most sysems an IPv6 socket should also open IPv4 but not pre-Vista Windows...
         DEBUG_M("Opening IPv4 socket...");
         socket4_ = new ip::udp::socket(io_, ip::udp::endpoint(ip::udp::v4(), getPort()));
         
         DEBUG_M("Opening IPv6 socket...");
         socket6_ = new ip::udp::socket(io_);
         ip::udp::endpoint endpoint6(ip::udp::v6(), getPort());
         socket6_->open(endpoint6.protocol());
         socket6_->set_option(ip::v6_only(true));
         socket6_->bind(endpoint6);
         
         // Turn off socket blocking...
         socket4_->io_control(non_blocking_io);
         socket6_->io_control(non_blocking_io);
      }

      void recieve() {
         ip::udp::endpoint sender_;
         try {
            size_t bytes = socket6_->receive_from(boost::asio::buffer(recv_buf_), sender_);
            cout << recv_buf_ << endl;
         } catch(std::exception e) {
            // Nothing...
         }
         
         try {
            size_t bytes = socket4_->receive_from(boost::asio::buffer(recv_buf_), sender_);
            cout << recv_buf_ << endl;
         } catch(std::exception e) {
           // Nothing...
         }
      }
      
      int getPort() {
         return port_;
      }
   
   
   
   //boost::array<char, 128> recv_buf_;
   /*udp::endpoint sender_endpoint;
   size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);
   std::cout.write(recv_buf.data(), len);*/
    
   private:
      io_service io_;
      ip::udp::socket* socket4_;
      ip::udp::socket* socket6_;
      int port_;
      char recv_buf_[buf_size];
};

#endif /* GAMERUNTIME_GAMESERVER_HPP_ */
