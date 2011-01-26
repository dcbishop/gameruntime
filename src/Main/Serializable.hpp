#ifndef GAMERUNTIME_SERIALIZABLE_HPP_
#define GAMERUNTIME_SERIALIZABLE_HPP_

class Serializable {
   public:
      virtual void networkSerialize(char* data) const = 0;
      virtual size_t getSerializedSize() const = 0;
};
typedef Serializable* SerializablePtr;
#endif /* GAMERUNTIME_SERIALIZABLE_HPP_ */
