#ifndef GAMERUNTIME_PROPERTYCLASS_HPP_
#define GAMERUNTIME_PROPERTYCLASS_HPP_

#include <unordered_map>
#include <sstream>
using namespace std;

#include <cstring>

#include "../Main/SharedId.hpp"
#include "../Main/Globals.hpp"
#include "../Main/Serializable.hpp"

class PropertyClass: public HasSharedId, public Serializable {
   public:
      PropertyClass(const string& name): name_(name) {setDefualts_();}

      ~PropertyClass() {
         // TODO: Lock accessed global properties!
         //globals.deregisterReplication(this);
         //globals.deregisterSave(this);
      }
      
      // This sets an indication that the property have been altered since it was loaded from local storage
      void setDirty() {
         setSaveFlag();
         setTransmitFlag(true);
      }
      
      virtual string toString() const = 0;
      
      // Adds property to the list of things that have been altered since it was loaded from local storage
      void setSaveFlag() {
         if(is_tempory_) {
            return;
         }

         if(need_save_) {
            return;
         }
         
         need_save_ = true;
         //globals.getSaveList().add(this);
      }
      
      const string& getName() const { return name_; }

      // Sets that this property needs to be transmitted to the clients
      void setTransmitFlag(bool flag=true) {
         if(!flag) {
            need_transmit_ = false;
            return;
         }
         // Add this to the list of things that are track for network transmit, things to be sent to any clients that connect to server      
         if(!is_transmitter_) {
            is_transmitter_ = true;
            globals.addTransmitter(this);
         }
         need_transmit_ = true;
      }
      
      bool getTransmitFlag() {
         return need_transmit_;
      }
      
   protected:
      void setDefualts_() {
         is_transmitter_ = false;
         need_transmit_ = false;
         need_save_ = false;
         was_local_ = false;
         is_tempory_ = false;
      }

   private:
      const string name_; // The properties access name (TODO, this needs to be psudo static!)

      bool need_save_; // If it has been altered since it was last saved.
      bool is_transmitter_; // If its in the list of things to be transmitted to clients when they change/connect.
      bool need_transmit_; // If it has been altered since last network transmit and needs to be sent next update.
      bool was_local_; // If it was loaded from a map file. If it is destroyed we have to fake that (Not for properties?).
      bool is_tempory_; // If this is set, any changes to the object will be lost on save.

};

template <typename T>
class Property: public PropertyClass {
   public:
      Property(const string& name, T value):
         PropertyClass(name),
         value_(value)
      { setDefualts_(); }

      const T& get() const { return value_; }

      virtual void set(const T& value) {
         setDirty();
         value_ = value;
      }
      virtual string toString() const {
         std::ostringstream o;
         o << value_;
         return o.str();
      }
      
      void networkSerialize(char* data) const {
         memcpy(data, &value_, getSerializedSize());
      }
      size_t getSerializedSize() const { return sizeof(value_); }

   private:
      T value_;   
};
typedef PropertyClass* PropertyClassPtr;

typedef unordered_map<string, PropertyClassPtr> Properties; 
class HasProperties {
   public:
      void bindProperty(PropertyClassPtr property) {
         properties_.insert(make_pair(property->getName(), property));
      }

      PropertyClassPtr getProperty(const string& name) {
         return properties_[name];
      }
     
   private:
      Properties properties_;   
};

#endif /* GAMERUNTIME_PROPERTYCLASS_HPP_ */
