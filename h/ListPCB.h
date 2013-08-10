// Project:	Operativni Sistemi
// File: 	ListPCB.h
// Date: 	Jul 2012
// Author: 	Petar Parabucki 115/06
// Contents:  
//			Class: ListPCB

#ifndef _ListPCB_h_
#define _ListPCB_h_

//#include "Thread.h"
#include "PCB.h"


class ListPCB {

  
  friend class Thread;
  friend class Semaphore;
  friend class KernelSem;
  friend class PCB;
  friend void interrupt timer();
  friend class MojaNit;
  friend class MyIdle;
  friend class KernelEv;
  friend int userMain(int,char**);
  friend int main(int,char**);
  friend class Consumer;
  friend class KeyboardEvent;
  friend class IVTEntry;

  struct Elem {                		 		// ELEMENT LISTE:
    PCB* ptr;                 				// - sadržaj,
    Elem* next;                		 		// - pokazivac na sledeci element,
    Elem(PCB* pt){ ptr=pt; }	  			// - konstruktor.

    PCB* returnPtr(){ return ptr; }
  };

                                                          // Konstruktor:
public: ListPCB() { head=curr=tail=NULL; cnt=0;  }        // - podrazumevani,
private:    

  Elem* head;         // Pokazivac na pocetak liste.
  Elem* curr;         // Pokazivac na tekuci element liste.
  Elem* tail;         // Pokazivac na kraj liste
  int cnt;
  
  ~ListPCB() {/*cout<<"Destruktor LISTE"<<endl;*/ delAll(); }        	// Destruktor.
  int count();           	 // Broj elemenata liste.
  void printList();  		   // Prolazak kroz celu listu i njeno ispisivanje.
 
  void addItem(PCB*);   	  // Dodavanje na kraj.
  
  void delItem(PCB*); 			// Brisanje Elementa iz liste.
  void delAll();            // Brisanje svih PCB-ova iz liste.
  void delAllFin();         // Brisanje svih PCB-ova iz liste koje su zavrseni.
  void delAllWokenUp();     // Brisanje svih probudjenih iz liste uspavanih procesa.

  Thread* getByID(ID);		  	 // Dohvatanje Thread-a po ID-u.
  Thread* getByName(TName);		 // Dohvatanje Thread-a po Imenu.
  ID      getIdByName(TName);  // Dohvatanje ID po imenu Thread-a.

  void checkSL();              // Provera uspavanih niti i njihovo budjenje.
  void checkBL();              // Provera blokiranih niti i njihovo nasilno budjenje.

  Elem* getHead(){ return head; } // Dohvatanje Head pokazivaca.
  Elem* getTail(){ return tail; } // Dohvatanje Tail pokazivaca.

  PCB* getFirst() { if ( this->head ) return this->head->ptr; else return NULL; } // Dohvatanje pvog PCB-a u listi
  PCB* getLast()  { if ( this->tail ) return this->tail->ptr; else return NULL; } // Dohvatanje poslednjeg PCB-a u listi

  void timeReset(Time);        // Azuriranje vremena liste. 
  
};


#endif