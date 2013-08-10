// Project:	Operativni Sistemi
// File: 	KernelS.h
// Date: 	Jul 2012
// Author: 	Petar Parabucki 115/06 
// Contents: 
//			Class: KernelSem  

#ifndef _KernelS_h_
#define _KernelS_h_

#include <iostream.h>

#include "ListPCB.h"
//#include "Semaphore.h"
#include "PCB.h"

class ListPCB;

class KernelSem {
	friend class KernelEv;
	friend class Semaphore;
	friend class Thread;
	friend int main(int,char**);
	

public:
	KernelSem (int);
   ~KernelSem ();

	int wait ();
    void signal ();

    void inc();
    void dec();

   int val () const { return value;} 	
 
private:
	
	int value;
	
	ListPCB* waitingList;

};

#endif