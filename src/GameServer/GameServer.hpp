#ifndef GAMERUNTIME_GAMESERVER_HPP_
#define GAMERUNTIME_GAMESERVER_HPP_

#include <iostream>
#include <vector>
using namespace std;

//TODO: Also support native asio...
#include <boost/asio.hpp>
using namespace boost;
using namespace asio;
using namespace ip;

const int buf_size = 8192;
typedef vector<ip::udp::endpoint> ConnectionsList;

#include "../GameServer/Packets.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost::posix_time;

class GameServer {
   public:
      GameServer(int port=11010, bool enableIPv6=true, bool enableIPv4=true): port_(11010), enableIPv6_(enableIPv6), enableIPv4_(enableIPv4) {
         socket6_ = NULL;
         socket4_ = NULL;
         transmits_per_second_ = 33;//Hz
         start();
      }
      
      ~GameServer() {
         delete socket4_;
         delete socket6_;
      }
      
      int getPort() {
         return port_;
      }

      void start();
      void recieve();
      void transmit();
      void addConnection(const string& address, const string& port);
      void addConnection(ip::udp::endpoint endpoint);
      void writeAll(const char* data, const unsigned int& size);

   private:
      int transmits_per_second_;
      io_service io_service_;
      ip::udp::socket* socket4_;
      ip::udp::socket* socket6_;
      int port_;
      char recv_buf_[buf_size];
      char send_buf_[buf_size];
      bool enableIPv6_;
      bool enableIPv4_;
      ConnectionsList connections_;
};

#endif /* GAMERUNTIME_GAMESERVER_HPP_ */
