// Project:	Operativni Sistemi
// File: 	KernelE.h
// Date: 	Decembar 2012
// Author: 	Petar Parabucki 115/06 
// Contents: 
//			Class: KernelEv  

#ifndef _KernelE_h_
#define _KernelE_h_

#include <stdio.h>
//#include <key>
#include "IVTEntry.h"
//#include "Event.h"

typedef unsigned char IVTNo;

class ListEvent;
class IVTEntry;

class KernelEv {

	friend class ListEvent;
	friend class Event;
	friend class Thread;
	friend class ListPCB;
	friend class ListPCB;
	friend class IVTEntry;
	friend int main(int,char**);

public:
	KernelEv(IVTEntry*);
	~KernelEv();

	int wait();
	void signal();

	IVTNo broj_ulaza;
	IR routine;
private:
	
	ListPCB* myBlockedPCBs;
	IVTEntry* entry;
	short flagSignal;
};

#endif