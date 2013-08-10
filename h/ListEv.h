// Project:	Operativni Sistemi
// File: 	ListEv.h
// Date: 	Decembar 2012
// Author: 	Petar Parabucki 115/06
// Contents:  
//			Class: ListEvent

#ifndef _ListEv_h_
#define _ListEv_h_

//#include "Event.h"
#include "PCB.h"

class IVTEntry;

class ListEvent {

  friend class IVTEntry;
  friend class Event;
  friend class KernelEv;
  friend class Thread;
  friend int main(int,char**);

  struct Elem {                		 		// ELEMENT LISTE:
    Event* ptr;                 			// - sadržaj,
    Elem* next;                		 		// - pokazivac na sledeci element,
    Elem(Event* pt){ ptr=pt; }	  		// - konstruktor.
  };

                                                            // Konstruktor:
public: ListEvent() { head=curr=tail=NULL; cnt=0; }        // - podrazumevani,

private:    

  Elem* head;         // Pokazivac na pocetak liste.
  Elem* curr;         // Pokazivac na tekuci element liste.
  Elem* tail;         // Pokazivac na kraj liste
  int cnt;
  
  ~ListEvent() { delAll(); }        	// Destruktor.

  int count();           	  // Broj elemenata liste.
 
  void addItem(Event*);     // Dodavanje na kraj.
  
  void delItem(Event*); 		// Brisanje Elementa iz liste.
  void delAll();            // Brisanje svih Event-ova iz liste.

  IVTEntry* getEntry(IVTNo);   // Dohvatanje IVTEntry-ja na osnovu broja ulaza.

  void signalAll();         // Signalizira sve Event-ove.

};


#endif