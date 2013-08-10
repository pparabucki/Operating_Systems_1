// Project:	Operativni Sistemi
// File: 	Semaphor.cpp
// Date: 	Jul 2012
// Author: 	Petar Parabucki 115/06 
// Contents: 
//			Class: Semaphor 

#include "Semaphor.h"
//#include "bounded.h"
#include "IVTEntry.h"
#include "KernelS.h"
#include "PCB.h"


Semaphore::Semaphore (int init) {
	LOCK();
	myImpl = new KernelSem(init);
	UNLOCK();
}

Semaphore::~Semaphore () {
	LOCK();
	delete myImpl;
	UNLOCK();
}

int Semaphore::wait(void) {
	return  myImpl->wait();
}

void Semaphore::signal(void) {
	myImpl->signal();
}

int Semaphore::val(void) const {
	return myImpl->val();
}