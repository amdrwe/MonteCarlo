/*
 *  StackList.h
 *
 *  Library implementing a generic, dynamic stack (linked list version).
 *
 *  ---
 *
 *  Copyright (C) 2010  Efstathios Chatzikyriakidis (contact@efxa.org)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *  ---
 *
 *  Version 1.0
 *
 *    2010-09-23  Efstathios Chatzikyriakidis  <contact@efxa.org>
 *
 *      - added exit(), blink(): error reporting and handling methods.
 *
 *    2010-09-20  Alexander Brevig  <alexanderbrevig@gmail.com>
 *
 *      - added setPrinter(): indirectly reference a Serial object.
 *
 *    2010-09-15  Efstathios Chatzikyriakidis  <contact@efxa.org>
 *
 *      - initial release of the library.
 *
 *  ---
 *
 *  For the latest version see: http://www.arduino.cc/
 */

// header defining the interface of the source.
#ifndef _STACKLIST_H
#define _STACKLIST_H

// include Arduino basic header.
#include <Arduino.h>

// the definition of the stack class.
template<typename T>
class StackList {
  public:
    // init the stack (constructor).
    StackList ();

    // clear the stack (destructor).
    ~StackList ();

    // push an item to the stack.
    void push (const T i);

    // pop an item from the stack.
    T pop ();

    // get an item from the stack.
    T peek () const;

    // check if the stack is empty.
    bool isEmpty () const;

    // get the number of items in the stack.
    int count () const;

    // set the printer of the stack.
    void setPrinter (Print & p);

  private:
    // exit report method in case of error.
    void exit (const char * m) const;

    // led blinking method in case of error.
    void blink () const;

    // the pin number of the on-board led.
    static const int ledPin = 13;

    // the structure of each node in the list.
    typedef struct node {
      T item;      // the item in the node.
      node * next; // the next node in the list.
    } node;

    typedef node * link; // synonym for pointer to a node.

    Print * printer; // the printer of the stack.
    int size;        // the size of the stack.
    link head;       // the head of the list.
};

// init the stack (constructor).
template<typename T>
StackList<T>::StackList () {
  size = 0;       // set the size of stack to zero.
  head = NULL;    // set the head of the list to point nowhere.
  printer = NULL; // set the printer of stack to point nowhere.
}

// clear the stack (destructor).
template<typename T>
StackList<T>::~StackList () {
  // deallocate memory space of each node in the list.
  for (link t = head; t != NULL; head = t) {
    t = head->next; delete head;
  }

  size = 0;       // set the size of stack to zero.
  printer = NULL; // set the printer of stack to point nowhere.
}

// push an item to the stack.
template<typename T>
void StackList<T>::push (const T i) {
  // create a new node.
  link t = (link) new node;

  // if there is a memory allocation error.
  if (t == NULL)
    exit ("STACK: insufficient memory to create a new node.");

  // store the item to the new node.
  t->item = i;

  // new node should be the head of the list.
  t->next = head; head = t;

  // increase the items.
  size++;
}

// pop an item from the stack.
template<typename T>
T StackList<T>::pop () {
  // check if the stack is empty.
  if (isEmpty ())
    exit ("STACK: can't pop item from stack: stack is empty.");

  // get the item of the head node.
  T item = head->item;

  // remove only the head node.
  link t = head->next; delete head; head = t;

  // decrease the items.
  size--;

  // return the item.
  return item;
}

// get an item from the stack.
template<typename T>
T StackList<T>::peek () const {
  // check if the stack is empty.
  if (isEmpty ())
    exit ("STACK: can't peek item from stack: stack is empty.");

  // return the item of the head node.
  return head->item;
}

// check if the stack is empty.
template<typename T>
bool StackList<T>::isEmpty () const {
  return head == NULL;
}

// get the number of items in the stack.
template<typename T>
int StackList<T>::count () const {
  return size;
}

// set the printer of the stack.
template<typename T>
void StackList<T>::setPrinter (Print & p) {
  printer = &p;
}

// exit report method in case of error.
template<typename T>
void StackList<T>::exit (const char * m) const {
  // print the message if there is a printer.
  if (printer)
    printer->println (m);

  // loop blinking until hardware reset.
  blink ();
}

// led blinking method in case of error.
template<typename T>
void StackList<T>::blink () const {
  // set led pin as output.
  pinMode (ledPin, OUTPUT);

  // continue looping until hardware reset.
  while (true) {
    digitalWrite (ledPin, HIGH); // sets the LED on.
    delay (250);                 // pauses 1/4 of second.
    digitalWrite (ledPin, LOW);  // sets the LED off.
    delay (250);                 // pauses 1/4 of second.
  }

  // solution selected due to lack of exit() and assert().
}

#endif // _STACKLIST_H
