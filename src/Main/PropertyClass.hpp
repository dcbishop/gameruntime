#ifndef GAMERUNTIME_PROPERTYCLASS_HPP_
#define GAMERUNTIME_PROPERTYCLASS_HPP_

template <typename T>
class PropertyClass {
   public:
      PropertyClass(const string& name): name_(name) {setDefualts_();}
      PropertyClass(const string& name, T value):
         value_(value),
         name_(name)
      {setDefualts_();}
      
      ~PropertyClass() {
         // TODO: Lock accessed global properties!
         //globals.deregisterReplication(this);
         //globals.deregisterSave(this);
      }
         
      T get() { return value_; }

      void set(const T& value) {
         setDirty();
         value_;
      }
      
      // This sets an indication that the property have been altered since it was loaded from local storage
      void setDirty() {
         setWillSave();
         setWillTransmit();
      }
      
      // Adds property to the list of things that have been altered since it was loaded from local storage
      void setWillSave() {
         if(isTempory_) {
            return;
         }

         if(needSave_) {
            return;
         }
         
         needSave_ = true;
         //globals.getSaveList().add(this);
      }
      

      // Sets that this property needs to be transmitted to the clients
      void setWillTransmit() {
         // Add this to the list of things that are track for network transmit, things to be sent to any clients that connect to server      
         if(!isTransmitting_) {
            isTransmitting_ = true;
            //globals.getTransmitList().add(this);
         }
         needTransmit_ = true;
      }
      
   private:
      void setDefualts_() {
         isReplicated_ = false;
         needSave_ = false;
         isTransmitting_ = false;
         needTransmit_ = false;
         wasLocal_ = false;
         isTempory_ = false;
      }
      T value_;
      const string name_; // The properties access name (TODO, this needs to be psudo static!)

      bool isReplicated_; // If it needs to be tracked and transmitted over the network.
      bool needSave_; // If it has been altered since it was last saved.
      bool isTransmitting_; // If its in the list of things to be transmitted to clients when they change/connect.
      bool needTransmit_; // If it has been altered since last network transmit and needs to be sent next update.
      bool wasLocal_; // If it was loaded from a map file. If it is destroyed we have to fake that (Not for properties?).
      bool isTempory_; // If this is set, any changes to the object will be lost on save.

};

#endif /* PROPERTYCLASS */
