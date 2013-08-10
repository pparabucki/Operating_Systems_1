// Project:	Operativni Sistemi
// File: 	Thread.cpp
// Date: 	Jul 2012
// Author: 	Petar Parabucki 115/06
// Contents: 
//			Classes: Thread 

#include <iostream.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Lock.h"
#include "Thread.h"
#include "KernelE.h"
#include "PCB.h"
#include "ListPCB.h"
#include "Schedule.h"
#include "KernelS.h"

class KernelEv;

extern ListPCB* list;
ListPCB* sleepList = new ListPCB();
extern MyIdle* myidle;
extern IVTEntry* nizIVT[];
extern Time currTime;
extern unsigned curSP, curSS, curBP;
void dispatch();



ID Thread::getID(){

	return this->myPCB->threadID;
} 

TName Thread::getName(){

	return this->myPCB->Name;
}

Thread::Thread (TName name ,  StackSize stackSize, Time timeSlice){
	LOCK();
    this->myPCB = new PCB(name,stackSize,timeSlice,this); 	// ovde se poziva konstruktor klase PCB, odnosno pravi se jedan objekat
    														// klase PCB koji se sadrzi u objektu Thread
    if(this->myPCB->status != PCB::IDL && this->myPCB->status != PCB::MAIN) list->addItem(this->myPCB);
    UNLOCK();
}

void Thread::start(void){
	LOCK();
	if(this->myPCB->status != PCB::IDL){ 
		if(myPCB->status == PCB::CRE){
			myPCB->status = PCB::REA;
			Scheduler::put(this->myPCB);
		}
	}
	UNLOCK();
}
 
Thread* Thread::getThreadById(ID id){
 	return list->getByID(id);
 }

ID Thread::getIdOf(TName name){
 	return list->getIdByName(name);
 } 

int Thread::wakeUp(){
	
	LOCK();

	if(myPCB->status == PCB::BLO){

	myPCB->status = PCB::REA;
	
	if(this->myPCB->blockSem != NULL){

		this->myPCB->blockSem->inc();

		this->myPCB->blockSem->waitingList->delItem(this->myPCB);
				
		this->myPCB->blockSem = NULL;

	}
	
	if(this->myPCB->blockEv!=NULL){
		
		this->myPCB->deblokirana = 1;
		this->myPCB->blockEv->signal();
		this->myPCB->blockEv = NULL;
	}
	if(sleepList->count()){
		sleepList->delItem(this->myPCB);
		this->myPCB->timeToWakeUp = 0;
		
	}
	
	Scheduler::put(this->myPCB);
			
	} 
	UNLOCK();
	return this->myPCB->probudjena;
}

int Thread::waitToComplete(){
		
	if (this->myPCB->status != PCB::FIN){
		this->myPCB->wtcSem->wait();
		return 1;
	}
	if (this->myPCB->probudjena) return 0;
}



void Thread::~Thread(){
	
	LOCK();

	waitToComplete();

	list->delItem(this->myPCB);
		
	delete this->myPCB;

	UNLOCK();
}

int Thread::sleep(Time timeToSleep){		
	
		LOCK();
		
	
		PCB::running->status = PCB::BLO;		
		sleepList->addItem(PCB::running);
		PCB::running->timeToWakeUp = currTime + timeToSleep;
	
		UNLOCK();
		
		dispatch();
		
		if(PCB::running->probudjena) return 0;
		
		return 1;
}