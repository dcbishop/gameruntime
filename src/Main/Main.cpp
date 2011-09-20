#include <iostream>
using namespace std;

#include "Globals.hpp"
#include "BaseClass.hpp"
#include "Player.hpp"
#include "Creature.hpp"
#include "World.hpp"

#include "../GameServer/GameServer.hpp"

#include "../Debug/console.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/program_options.hpp>
namespace po = boost::program_options;


int main(int argc, char* argv[]) {

   int port;
   string hostname;
   bool isServer = false;
   bool enableIPv6 = true;
   bool enableIPv4 = true;
   bool testmode = false;
   int localport = 0;
   int remoteport = 11010;
   string debugtoken_s = "8cbf22cf-d244-4784-a888-f0a37457ca6f";
   po::options_description desc("Allowed options");
   desc.add_options()
      ("help", "Display this help and exit.")
      ("version", "Output version information and exit.")
      ("server,s", "Start a dedicated server.")
      ("6,6", "Forces server to use IPv6.")
      ("4,4", "Forces server to use IPv4.")
      ("testmode", "DEBUG")
      ("token", po::value<string>(&debugtoken_s), "DEBUG")
      ("hostname,h", po::value<string>(&hostname), "Connect to server with this hostname.")
      ("port,p", po::value<int>(&port)->default_value(11010), "Specify port number (for server this is the listening port number).")
   ;

   po::positional_options_description p;
   p.add("hostname", 1).add("port", 1);

   po::variables_map vm;
   po::store(po::command_line_parser(argc, argv).
             options(desc).positional(p).run(), vm);
   po::notify(vm);

   // help arg
   if (vm.count("help")) {
       cout << desc << "\n";
       return 0;
   }
   
   if (vm.count("testmode")) {
      testmode = true;
   }


   // version arg
   if (vm.count("version")) {
      cout << argv[0] << " " << globals.git_version << ", built: " << globals.build_date.c_str() << "\n";
      return 0;
   }

   LOG("Starting %s, version %s, built %s...", argv[0], globals.git_version.c_str(), globals.build_date.c_str());

   // Check for both IPv6 and IPv4
   if(vm.count("4") && vm.count("6")) {
      enableIPv6 = true;
      enableIPv4 = true;
      ERROR("Conflicting args -4 and -6. By default both IPv4 and IPv6 are enabled, you don't specify both.");
      return 1;
   }

   // IPv6 arg...
   if(vm.count("6")) {
      enableIPv6 = true;
      enableIPv4 = false;
   }
   
   // IPv4 arg...
   if(vm.count("4")) {
      enableIPv4 = true;
      enableIPv6 = false;
   }

   // server arg
   if (vm.count("server")) {
       isServer = true;
       localport = port;
       remoteport = 0;
   } else {
      // If this is a client
      isServer = false;
      remoteport = port;
      localport = 0;
      if(hostname.empty()) {
         ERROR("You must specify a server to connect to");
         return 1;
      }
   }

   World world;
   world.setStringId("The_World");
   
   Area area1;
   area1.setStringId("Area_01");
   Area area2;
   area2.setStringId("Area_02");
   
   world.addArea(&area1);
   world.addArea(&area2);
   
   Player player1;
   Player player2;
   area1.addObject(&player1);
   area2.addObject(&player2);
   
   DEBUG_M("localport: %d, remoteport: %d", localport, remoteport);
   
   boost::uuids::uuid debug_token = boost::uuids::string_generator()(debugtoken_s);
   
   GameServer game_server_(localport, enableIPv6, enableIPv4);
   //game_server_.addToken(debug_token);
   if(!isServer) {
      std::ostringstream port_ss;
      port_ss << remoteport;
      game_server_.connect(hostname, port_ss.str(), debug_token);
   }

   float x = 0.0f;
   float speed = 0.01f;
   int direction = 1;
   while(true) {
      game_server_.recieve();
      // TODO: DEBUG CODE
      if(testmode) {
         if(isServer) {
            x+=speed*direction;
            player1.setX(x);
            if(x > 10.0f) {
               direction = -1;
            }
            if(x < -10.0f) {
               direction = 1;
            }
         }
      }
      game_server_.transmit();
   }
      
   LOG("Finished.");
}
