// Project:	Operativni Sistemi
// File: 	Event.h
// Date: 	Decembar 2012
// Author: 	Petar Parabucki 115/06
// Contents: 
//			Class: Event  

#ifndef _Event_h_
#define _Event_h_

#include "IVTEntry.h"

typedef unsigned char IVTNo;

class KernelEv;

class Event {

public:
	Event(IVTNo ivtNo);
	~Event();

	int wait();
	void signal();

private:
	KernelEv* myImpl;

};
#endif 