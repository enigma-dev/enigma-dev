/**
 * @file  quickstack.h
 * @brief A file implementing a small, fast stack type.
 * 
 * @section License
 * 
 * Copyright (C) 2011 Josh Ventura
 * This file is part of JustDefineIt.
 * 
 * JustDefineIt is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3 of the License, or (at your option) any later version.
 * 
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef _QUICKSTACK__H
#define _QUICKSTACK__H

/** Namespace holding specialized container data types designed to up speed or minimize allocation.
**/
namespace quick {
  /** A basic stack structure designed to minimize allocation.
  **/
  template<typename tp, int chunksize=5> class stack
  {
    /** Private template type for storing a chunk of stack nodes. **/
    struct node {
      tp data; ///< The data stored in the node.
      node *prev; ///< The node after this one, be it in this chunk or the next. If prev==NULL, the stack is presently "empty."
      node *next; ///< The node before this node, be it in this chunk or the last. If next==NULL, a push will allocate a new node.
    } *ntop;
    
    size_t tnum;
    
    /**
      Private utility function to allocate a new chunk of nodes and validate it.
      This is accomplished by setting enveloped nodes' \c next and \c prev members
      to pointers within the chunk.
      @param p  A pointer to the last node of the chunk before this one.
    **/
    node* alloc(node* p) {
      node* r = new node[chunksize];
      for (int i = 1; i < chunksize; i++)
        r[i - 1].next = r + i,
        r[i].prev = r + i - 1;
      r[chunksize - 1].next = 0;
      r[0].prev = p;
      return r;
    }
    
    /**
      Traverses the nodes, deleting each chunk.
      This method is to be called for cleanup.
    **/
    void freeall() {
      while (ntop->prev) ntop = ntop->prev;
      while (ntop) {
        node* dm = ntop;
        for (int i = 0; i < chunksize; i++)
          ntop = ntop->next;
        delete[] dm;
      }
    }
    
    public:
      /// Default constructor; creates an empty stack.
      stack(): tnum(0) { ntop = alloc(0); } 
      /** Push a new node onto the stack with the value given by x.
          In actuality, it is unlikely that a new node will be allocated
          rather than a pointer being incremented and value being set. **/
      void push(tp x) { if (!ntop->next) ntop->next = alloc(ntop); ntop = ntop->next; ntop->data = x; ++tnum; }
      /** Pushes a new node onto the stack, but instead of copying x, simply swaps it with the new node.
          This leaves the given value in the same state it would be if constructed fresh. **/
      void enswap(tp &x) { if (!ntop->next) ntop->next = alloc(ntop); ntop = ntop->next; ntop->data.swap(x); ++tnum; }
      /** Pops an item from the stack, returning the value it contained.
          In actuality, this only involves changing the top pointer to
          its previous node's. **/
      tp &pop() { tp &r = ntop->data; ntop = ntop->prev; --tnum; return r; }
      /** Return the item of the top node on the stack. **/
      tp& top() { return ntop->data; }
      /** Default destructor. Picks up after our class. **/
      ~stack() { freeall(); }
      /** Returns whether the stack is empty.
          In other words, returns whether any of the nodes in our stack
          are in use. **/
      bool empty() { return !ntop->prev; }
      /** Returns whether the stack is empty.
          In other words, returns whether any of the nodes in our stack
          are in use. **/
      size_t size() { return tnum; }
      
      /// Standard iterator type
      class iterator {
        node *n; ///< The node content of this iterator
        friend class quick::stack<tp>;
        iterator(node *np): n(np) {}
        public:
          tp &operator*() const  { return n->data; }
          tp *operator->() const { return &n->data; }
          iterator &operator--()    { n = n->prev; return *this; }
          iterator  operator--(int) { return iterator(n->prev);  }
          iterator &operator++()    { n = n->next; return *this; }
          iterator  operator++(int) { return iterator(n->next);  }
          bool operator==(const iterator& it) const { return n == it.n; }
          bool operator!=(const iterator& it) const { return n != it.n; }
      };
      iterator begin() { return iterator(ntop); }
      iterator end() { return iterator(NULL); }
  };
}

#endif
