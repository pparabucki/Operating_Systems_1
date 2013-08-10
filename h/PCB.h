// Project:	Operativni Sistemi
// File: 	PCB.h
// Date: 	Jul 2012
// Author: 	Petar Parabucki 115/06
// Contents: 
//			Class: PCB 
//
//			Macro's   : GETSTACK
//						SETSTACK

#ifndef _PCB_h_
#define _PCB_h_


#define GETSTACK() asm{\
mov curSP, sp;\
mov curSS, ss;\
mov curBP, bp;\
}\

#define SETSTACK() asm{\
mov sp, curSP;\
mov ss, curSS;\
mov bp, curBP;\
}\

#include <stdio.h> 
#include <dos.h>
#include <setjmp.h>

//#include "Thread.h"
#include "Schedule.h"
#include "Nit.h"
//#include "Semaphor.h"
#include "KernelS.h"
#include "KernelE.h"
#include <lock.h>



class Event;
class KernelEv;
class KernelSem;
class Semaphore;

typedef unsigned long StackSize;
typedef unsigned int Time; 	
typedef int ID;
typedef char* TName;

class PCB { 

	friend int main(int,char**);
	friend class Thread;
	friend class MyIdle;
	friend class MojaNit;
	friend class ListPCB;
	friend class ListEvent;
	friend void interrupt timer();
	friend class KernelSem;
	friend class KernelEv;
	friend class IVTEntry;
	friend class Producer;
	friend class Consumer;
	friend void dispatch();
	
	enum ThrStatus {
		CRE=1, ACT , BLO , REA , FIN , IDL , MAIN  // KREIRANA=1 , AKTIVNA=2 , BLOKIRANA=3 , SPREMNA=4 , GOTOVA=5 , IDLE = 6 , MAIN = 7
	};

public:

	PCB(TName name , StackSize stackSize, Time timeSlice, Thread* myThread);
	
	StackSize 	getStackSize();
	Time 		getTimeSlice();
	ID 			getThreadID();

	unsigned getStackPointer ();
	unsigned getStackSegment ();
	unsigned getBasePointer	 ();

	PCB* getMyPCB();

	static void wrappMyPCB();

private:
	
	// destruktor 
	~PCB();	
	
	// Kontekst niti
	unsigned int ss, sp, bp;
	unsigned* stack;
	unsigned probudjena;
	unsigned deblokirana;	// za event
	short semSignal;
	short eveSignal;
	StackSize stackSize;

	// Drugi parametri niti
	TName Name; 
	Time curr , allow;		// curr - trenutno vreme , allow - timeSlice
	Time timeToWakeUp;		
	Thread* myThread;
	ThrStatus status;		// definise u kom stanju je nit
	ID threadID;

	KernelSem *wtcSem;
	KernelSem *blockSem; 
	KernelEv  *blockEv;
	unsigned signalizira;

	static ID ukID;
	static PCB* running;
	 
};

#endif 