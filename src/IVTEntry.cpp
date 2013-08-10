// Project:	Operativni Sistemi
// File: 	IVTEntry.cpp
// Date: 	Decembar 2012
// Author: 	Petar Parabucki 115/06 
// Contents: 
//			Class :  IVTEntry  

#include "IVTEntry.h"
#include <lock.h>

IVTEntry* nizIVT[256];
extern ListPCB* list;


void IVTEntry::IVTEntry(){
	
}
void IVTEntry::IVTEntry(IVTNo iNum, IR iHand){

	nizIVT[iNum] = this;
	nizIVT[iNum]->ivtNo = iNum;
	nizIVT[iNum]->newHandler = iHand;

	nizIVT[iNum]->oldHandler = (IR) getvect(iNum);

	setvect(iNum,iHand);
		
}

IVTEntry::~IVTEntry(void){
	
	LOCK();
	setvect(ivtNo, nizIVT[ivtNo]->oldHandler);
	oldHandler = NULL;
	newHandler = NULL;

	UNLOCK();
}

void IVTEntry::signalAll(){
	blockedEv->signal();
}

IVTEntry* IVTEntry::getEntry(IVTNo iNum){
	LOCK();
	return nizIVT[iNum];
	UNLOCK();

}

void IVTEntry::addETL(KernelEv *e){
	LOCK();
	blockedEv = e;
	UNLOCK();
}

void IVTEntry::delEFL(){
	LOCK();
	delete blockedEv;
	UNLOCK();
}
