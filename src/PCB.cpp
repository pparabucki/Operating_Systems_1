// Project:	Operativni Sistemi
// File: 	PCB.cpp 
// Date: 	Jul 2012
// Author: 	Petar Parabucki 115/06
// Contents: 
//				Class :	PCB  
//
//			functions :	void interrupt timer()
//						void dispatch()
//




#define VDEC 65000

#define DEFAULT_SIZE 15

#include <stdlib.h>
#include "PCB.h"
#include "iostream.h"
#include "Nit.h"
#include "IVTEntry.h"

class IVTEntry;

unsigned curSP=NULL, curSS=NULL, curBP=NULL ;

unsigned short promena = 0; // globalna promenljiva za promenu

Time currTime=0;			// trenutno proteklo vreme

void dispatch();
void tick();


extern MyIdle *myidle;
extern ListPCB *list;
extern ListPCB *sleepList;

PCB* IDLE;
PCB* PCB::running = NULL;
ID PCB::ukID = 0;

IR oldTimerIR = NULL;


PCB::PCB(TName name , StackSize stackSize, Time timeSlice, Thread* myThread) {

	LOCK();		
	
	this->threadID = ++ukID;
	this->stackSize = stackSize;
	this->curr = timeSlice;
	this->allow = timeSlice;
	this->timeToWakeUp = 0;		// 21.12.2012
	this->myThread = myThread;
	this->status = CRE;
	
	this->probudjena = 0;
	this->deblokirana = 0;
	this->semSignal = 0;
	this->eveSignal = 0;
	
	this->blockSem = NULL;
	this->blockEv  = NULL;
	
	if(name!=NULL){								// dodeljivanje imena
	this->Name=(char*)malloc(sizeof(name));
	sprintf(this->Name,name);
    }
	else{
    name=(char*)malloc(DEFAULT_SIZE);	
    this->Name=(char*)malloc(DEFAULT_SIZE);
	sprintf(name, "Thread %d", this->threadID);
	sprintf(this->Name,name);
    free(name);
    }

    // cuvanje steka
  	stack = new unsigned int[this->stackSize];                  
	
	stack[this->stackSize-1] = 0x200; 
	 
	stack[this->stackSize-2] = FP_SEG(PCB::wrappMyPCB);                   
	stack[this->stackSize-3] = FP_OFF(PCB::wrappMyPCB);                  
	 
	sp = FP_OFF(stack+(stackSize-12));             
	bp = FP_OFF(stack+(stackSize-12));
	ss = FP_SEG(stack+(stackSize-12));                  
	

	if(strcmp(name, "MYIDLE") == 0){
		this->status = PCB::IDL;
		IDLE = this;
	}

	wtcSem = new KernelSem(0);

	UNLOCK();
}

PCB::~PCB () { 
	delete stack; 
	//delete wtcSem;
}

ID PCB::getThreadID(){
	return this->threadID;
}

StackSize 	PCB::getStackSize(){
	return this->stackSize;
}

Time PCB::getTimeSlice(){
	return this->allow;
}

unsigned PCB::getStackPointer () { return this->sp; }

unsigned PCB::getStackSegment () { return this->ss;	}

unsigned PCB::getBasePointer  () { return this->bp; }

PCB* PCB::getMyPCB() { return this;}

void PCB::wrappMyPCB(){

	PCB::running->myThread->run();
	
	LOCK();

	PCB::running->status=FIN; 

	PCB::running->wtcSem->signal();

	UNLOCK();
	
	dispatch(); 	
}

void interrupt timer(){

	GETSTACK();

	if(!promena){	
		tick(); 
		++currTime;
		asm int 60h;
	}

	if( !promena && currTime == VDEC ){
		currTime -= VDEC;
		sleepList->timeReset(VDEC);
	}

	
	if( !promena && sleepList->count() ){
		sleepList->checkSL();
	}

	if (!( PCB::running->curr == 0 && !promena )){

		if(	PCB::running->allow != 0 ) PCB::running->curr = PCB::running->curr - 1;
		if( (PCB::running->curr <= 1 &&  PCB::running->allow != 0 ) || promena ) {
			
		PCB::running->ss = curSS;		
		PCB::running->sp = curSP;
		PCB::running->bp = curBP; 

		if( PCB::running->curr <= 1 &&  PCB::running->allow != 0 && !promena ) 	PCB::running->curr = PCB::running->allow;	// resetovano vreme

		if( PCB::running->status != PCB::BLO && PCB::running->status != PCB::FIN && PCB::running->status != PCB::IDL && PCB::running->status != PCB::MAIN )
			{ PCB::running->status = PCB::REA; Scheduler::put(PCB::running); } 

		PCB::running = Scheduler::get();

		if( PCB::running == NULL){ PCB::running = IDLE;	}	 
		
		if( PCB::running != IDLE ) PCB::running->status = PCB::ACT;
 
		curSS = PCB::running->ss;		
		curSP = PCB::running->sp;
		curBP = PCB::running->bp;

		promena = 0;

		SETSTACK();

		}
	} 
}

void dispatch(){

	LOCK();

	promena = 1;

	timer();

	UNLOCK();

}

