// Project:	Operativni Sistemi
// File: 	KernelE.cpp
// Date: 	Decembar 2012
// Author: 	Petar Parabucki 115/06 
// Contents: 
//			Class: KernelEv  

#include "KernelE.h"
#include <lock.h>

extern IVTEntry* nizIVT[];
extern ListPCB* list;
extern int semPreempt;

KernelEv::KernelEv(IVTEntry *ivtE){
	LOCK();
	
		entry = ivtE;	
		flagSignal = 0;
		broj_ulaza = ivtE->ivtNo;

		myBlockedPCBs = new ListPCB();
		
	UNLOCK();
}

KernelEv::~KernelEv(){

	LOCK();
	flagSignal = 0;
	if(myBlockedPCBs->count()){
		myBlockedPCBs->checkBL();
	}

	UNLOCK();

}

int KernelEv::wait(){

	LOCK();

	
		PCB::running->status = PCB::BLO;
		PCB::running->blockEv = this;
	
		myBlockedPCBs->addItem(PCB::running);
		
		UNLOCK();		
		dispatch();
	
	
	if (semPreempt){
		dispatch();
	}

	if(PCB::running->eveSignal){ return 1;}
	else if(PCB::running->deblokirana) return 0;
	return 2;
}

void KernelEv::signal(){

	LOCK();

	if(flagSignal==0){
		flagSignal = 1;
	}
	
	myBlockedPCBs->checkBL();
	UNLOCK();
	
	if (semPreempt ){
			dispatch();
	}
	
}
