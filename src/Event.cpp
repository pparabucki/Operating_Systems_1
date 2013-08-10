// Project:	Operativni Sistemi
// File: 	Event.cpp
// Date: 	Decembar 2012
// Author: 	Petar Parabucki 115/06
// Contents: 
//			Class: Event  

//#include "Event.h"
#include <event.h>
#include "lock.h"
#include "KernelEv.h"
#include "IVTEntry.h"

Event::Event (IVTNo ivtNo){
	LOCK();
	myImpl = new KernelEv(IVTEntry::getEntry(ivtNo));
	IVTEntry::getEntry(ivtNo)->blockedEv = this->myImpl;
	UNLOCK();
}

Event::~Event () {
	
	delete myImpl;
}

int Event::wait() {
	return myImpl->wait();
}

void Event::signal() {
	myImpl->signal();
}

