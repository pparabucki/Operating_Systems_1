// Project:	Operativni Sistemi
// File: 	KernelS.cpp
// Date: 	Jul 2012
// Author: 	Petar Parabucki 115/06 
// Contents: 
//			Class: KernelSem  

#include <iostream.h>

#include "KernelS.h"
#include "ListPCB.h"
//#include "Thread.h"
//#include "PCB.h"
#include "Lock.h"
#include "Schedule.h"

extern int semPreempt;

void dispatch();

KernelSem::KernelSem(int init) : value(init){
	LOCK();
	this->waitingList = new ListPCB();
	UNLOCK();
}
KernelSem::~KernelSem () {
	LOCK(); 
	this->waitingList->delAll(); 
	UNLOCK(); 
}

int KernelSem::wait(){
	LOCK();
	if ( (--value )< 0 ){

			PCB::running->status = PCB::BLO;
			PCB::running->blockSem = this;
			
			this->waitingList->addItem(PCB::running);
		
			UNLOCK();
			dispatch();		
	}

	UNLOCK();
	
	if(semPreempt){
		dispatch();
	}

	if(PCB::running->semSignal){ return 1; }
	else if(PCB::running->probudjena) return 0;
	return 2;
}

void KernelSem::signal(){
	LOCK();
	if( ++value < 1){
		PCB* toUnblock = this->waitingList->getLast();
		if( toUnblock ){ 
			if(this == toUnblock->blockSem) toUnblock->semSignal = 1;
			this->waitingList->delItem(toUnblock);
			toUnblock->status = PCB::REA;
			Scheduler::put(toUnblock);

		}
	}
	UNLOCK();
	if (semPreempt ){
			dispatch();
	}
}

void KernelSem::inc() {		LOCK();	value++; UNLOCK();	}

void KernelSem::dec() { 	LOCK(); value--; UNLOCK();	}