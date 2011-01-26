#ifndef GAMERUNTIME_PACKETWRITER_HPP_
#define GAMERUNTIME_PACKETWRITER_HPP_

enum opcodes {OP_NULL, OP_RESERVED, OP_PING, OP_PONG, OP_SET, OP_DUMMYLAST, OP_ROLLOVER=255};
const int LAST_OPCODE = OP_DUMMYLAST;

#include "../Debug/console.h"

namespace PacketWriter {
   inline unsigned int set(char* buffer, const unsigned int& bufsize, const SharedId& id, const SerializablePtr object) {
      const unsigned int packet_size = 1 + sizeof(id) + object->getSerializedSize();
      if(packet_size > bufsize) {
         ERROR("Out of send buffer space.");
         return 0;
      }

      buffer[0] = OP_SET;
      memcpy(buffer+1, &id, sizeof(id));
      object->networkSerialize(buffer+1+sizeof(id));
      return packet_size;
   }
   
   inline unsigned int ping(char* buffer, const unsigned int& bufsize, const time_t& timestamp) {
      const unsigned int packet_size = 1 + sizeof(timestamp);
      if(packet_size > bufsize) {
         ERROR("Out of send buffer space.");
         return 0;
      }
      buffer[0] = OP_PING;
      memcpy(buffer+1, &timestamp, sizeof(timestamp));
      return packet_size;     
   }
   
   inline unsigned int pong(char* buffer, const unsigned int& bufsize, const time_t& timestamp) {
      const unsigned int packet_size = 1 + sizeof(timestamp);
      if(packet_size > bufsize) {
         ERROR("Out of send buffer space.");
         return 0;
      }
      buffer[0] = OP_PONG;
      memcpy(buffer+1, &timestamp, sizeof(timestamp));
      return packet_size;     
   }
};

class PacketReader {
   static void* opcodes[LAST_OPCODE];
   PacketReader() {
      opcodes[OP_NULL] = NULL;
      opcodes[OP_RESERVED] = NULL;
      opcodes[OP_PING] = NULL;
      opcodes[OP_PONG] = NULL;
      opcodes[OP_SET] = NULL;
      opcodes[OP_DUMMYLAST] = NULL;
      opcodes[OP_ROLLOVER] = NULL;
      
   }

   void lookup(char* buf, int buf_size) {
      /*if(opcodes[buf[0]]) {
      }*/
   }
   
};

#endif /* GAMERUNTIME_PACKETWRITER_HPP_ */
