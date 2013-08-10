// Project:	Operativni Sistemi
// File: 	IVTEntry.h
// Date: 	Decembar 2012
// Author: 	Petar Parabucki 115/06 
// Contents: 
//			Class :  IVTEntry  
//
//			Macro's   : 
//						PREPAREENTRY


#ifndef _IVTEntry_h_
#define _IVTEntry_h_

#define PREPAREENTRY(iNum,old)\
void interrupt interruptRoutine##iNum(...);\
IVTEntry entry##iNum(iNum, interruptRoutine##iNum);\
void interrupt interruptRoutine##iNum(...){\
LOCK();\
nizIVT[iNum]->signalAll();\
if (old == 1) (*(nizIVT[iNum]->oldHandler))(...);\
UNLOCK();\
}

class IVTEntry;

extern IVTEntry* nizIVT[];


typedef void interrupt (*IR)(...);
typedef unsigned char IVTNo;


#include "ListEv.h"


class IVTEntry{

	friend class Event;
	friend class KernelEv;
	friend class ListPCB;
	friend class ListEvent;
	friend void interrupt interruptRoutine9(...);
	friend IR iHand(...);
	friend int main(int,char**);

public:
	IVTEntry();
	IVTEntry(IVTNo iNum, IR iHand);
	~IVTEntry();

	IR	newHandler;
	IR	oldHandler;

	void signalAll();
	static IVTEntry* getEntry(IVTNo iNum);
private:
	
	void addETL	(KernelEv *e);
	void delEFL	();

	unsigned char ivtNo;
	KernelEv* blockedEv;
};

#endif