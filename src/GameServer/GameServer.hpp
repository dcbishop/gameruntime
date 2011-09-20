#ifndef GAMERUNTIME_GAMESERVER_HPP_
#define GAMERUNTIME_GAMESERVER_HPP_

#include <iostream>
#include <vector>
#include <unordered_set>
using namespace std;

//TODO: Also support native asio...
#include <boost/asio.hpp>
using namespace boost;
using namespace asio;
using namespace ip;

const int buf_size = 8192;
typedef vector<ip::udp::endpoint> ConnectionsList;

#include <boost/thread/mutex.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost::posix_time;
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

typedef unordered_set<boost::uuids::uuid> TokenSet;

class GameServer {
   public:
      GameServer(int port=11010, bool enableIPv6=true, bool enableIPv4=true): port_(port), enableIPv6_(enableIPv6), enableIPv4_(enableIPv4) {
         socket6_ = NULL;
         socket4_ = NULL;
         transmits_per_second_ = 33;//Hz
         isDualStack_ = false;
         timeout_ = 10;
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
      void processPacket(const unsigned int& size);
      void connect(const string& address, const string& port, const boost::uuids::uuid& token);
      void addConnection(ip::udp::endpoint endpoint);
      void writeAll(const char* data, const unsigned int& size);
      void write(const char* data, const unsigned int& size, const udp::endpoint& destination);
      void addToken(const boost::uuids::uuid& token) {
         tokens_lock_.lock();
         tokens_.insert(token);
         tokens_lock_.unlock();
      }
      bool findAndRemoveToken(const boost::uuids::uuid& token) {
         tokens_lock_.lock();
         TokenSet::iterator result = tokens_.find(token);

         if(result != tokens_.end()) {
            tokens_.erase(result);
            tokens_lock_.unlock();
            return true;
         }

         tokens_lock_.unlock();
         return false;
      }

   private:
      size_t receive_from_();
      void debugDumpBuffer_(const char* buffer, const unsigned int& size);

      int transmits_per_second_;
      io_service io_service_;
      ip::udp::socket* socket4_;
      ip::udp::socket* socket6_;
      int port_;
      char recv_buf_[buf_size];
      char send_buf_[buf_size];
      bool enableIPv6_;
      bool enableIPv4_;
      bool isDualStack_;
      ConnectionsList connections_;
      ip::udp::endpoint last_sender_;
      int timeout_;
      boost::mutex tokens_lock_;
      TokenSet tokens_;

};

#endif /* GAMERUNTIME_GAMESERVER_HPP_ */
