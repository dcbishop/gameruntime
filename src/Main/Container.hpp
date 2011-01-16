#ifndef GAMERUNTIME_CONTAINER_HPP_
#define GAMERUNTIME_CONTAINER_HPP_

typedef unordered_map<SharedId, Contained*> Contense;
typedef list<Contained*> Containers;

class Container {
   friend class Contained;

   public:
      ~Contained() {
         wipe_();
      }

      // Adds a Contained item to the list of items that are contained and updates its refrence back here
      void add(cont int& id, Contained* thing) {
         contents_.insert( make_pair(int, thing));
         thing->registerContainer_(this);
      }

      // Removes an item from the list of things that are contained and removes its refrence back here
      void remove(Contained* thing) {
         drop(thing);
         thing->deregisterContainer_(this);
      }
      
   private:
      // Removes a Contained item
      void drop_(Contained* thing) {
         content.erase(contents_->find(thing));
      }

      // Goes though all the contained objects and ensures they nolonger refrence here
      void wipe_() {
         for(Contents::iterator it = contents_.begin(); it != contents_.end(); it++) {
            it->deregisterContainer_(this);
         }
      }      
      
      typedef Contents contents_; 
};

class Contained {
   friend class Container;

   public:
      ~Contained() {
         wipe_();
      }

   private:
      registerContainer_(Container* container) {
         containers_.insert(container);
      }
      
      deregisterContainer_(Container* container) {
         containers_.erase(containers_.find(container));
      }
      
      void wipe_() {
         for(Containers::iterator it = containers_.begin(); it != containers_.end(); it++) {
            it->drop(this);
         }
      }
      

      Containers containers_; // The list of things this object is contained within.
};

#endif /* GAMERUNTIME_CONTAINER_HPP_ */
