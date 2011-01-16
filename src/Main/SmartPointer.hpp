#ifndef GAME_PTR_HPP_
#define GAME_PTR_HPP_

#include <iostream>

#include "../Debug/console.h"

template <class T> class game_ptr {
   private:
      T* ptr;
      int timestamp;
   public:
      explicit game_ptr(T* p = 0) : ptr(p){ DEBUG_M("Constructor made %p pointing to %p", this, ptr); }
      ~game_ptr()    { DEBUG_M("Deleting '%p'", ptr); delete ptr; }
      T& operator*()  { DEBUG_M("requested refrence..."); return *ptr; }
      T* operator->() { DEBUG_M("requested pointer..."); return ptr; }
      
};

#endif
