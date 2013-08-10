// Project:	Operativni Sistemi
// File: 	ListPCB.cpp
// Date: 	Jul 2012
// Author: 	Petar Parabucki 115/06
// Contents:  
//			Class: ListPCB

#include <stdio.h>
#include <string.h> 
#include <iostream.h>
#include "Schedule.h"
#include "Lock.h"
#include "ListPCB.h"

extern Time currTime;
extern ListPCB *sleepList;
extern ListPCB *blockList;


int ListPCB::count(){
	return cnt;
}

void ListPCB::printList(){
	LOCK();
	if(head==NULL){
		cout<<"U listi nema elemenata !!!"<<endl;
	}else{
	cout<<"U listi ima "<<cnt<<" elemenata, sledi ispis:"<<endl;
	curr=head;
	while(curr){
		cout<<"Nit "<<curr->ptr->Name<<" ID = "<<curr->ptr->threadID<<" STATUS ="<<curr->ptr->status<<endl;
		curr=curr->next;
	} 	
	}
	UNLOCK();
}


void ListPCB::addItem(PCB* newPCB){
	
	LOCK();
 
	Elem *novi = new Elem(newPCB);
	if(!head) head=novi;
	else tail->next=novi;
	tail=novi;
	novi->next=NULL;
	cnt++;

	UNLOCK();
 }

void ListPCB::delItem(PCB* delPCB){
	
 	LOCK();
 	curr=head;
	Elem *prev = NULL;
	while(curr){
	  	if(curr->ptr != delPCB){
	   		prev=curr;
	   		curr=curr->next;
	  	}else{
	   		Elem *old = curr;
	   		curr = curr->next;
	   		if(curr == NULL) tail = prev;	   
	   		if(!prev) head = curr;
	   		else prev->next=curr;
	   
	   		delete old;
	   		cnt--;
	  }
	}
 	UNLOCK();
}

void ListPCB::delAllFin(){
	
	LOCK();

	Elem* prev=head;
	curr=head->next;

	while(curr){
		if(curr->ptr->status == PCB::FIN)	{	// brisem samo zavrsene niti
			prev->next=curr->next;
			if( prev->next == NULL ) tail = prev;
			delete curr;
			curr=prev->next;
			cnt--;
		}else{
			prev=curr;
			curr=curr->next;
		}
	}

	UNLOCK();	
}

void ListPCB::delAllWokenUp(){
	
	LOCK();

	Elem* prev=head;
	curr=head->next;

	while(curr){
		if(curr->ptr->status == PCB::REA)	{	// brisem samo probudjene niti iz liste uspavanih
			prev->next=curr->next;
			if( prev->next == NULL ) tail = prev;
			delete curr;
			curr=prev->next;
			cnt--;
		}else{
			prev=curr;
			curr=curr->next;
		}
	}

	UNLOCK();	
}

void ListPCB::delAll(){
	
	LOCK();

	curr = head;

	while(curr){
	
		Elem* erese;
		erese=curr;
		curr=curr->next;
		delete erese;
		head=curr;
	}
	if(curr==tail){
			delete head;
			delete curr;
			delete tail;
	}
	cnt=0;
	UNLOCK(); 
		
}

Thread* ListPCB::getByID(ID id){

	LOCK();
	curr=head;

	while(curr){
		if(curr->ptr->threadID==id){
			UNLOCK();
			return curr->ptr->myThread;	
		}
		curr=curr->next;
	} 

	UNLOCK();
	return NULL;

}

Thread* ListPCB::getByName(TName name){
	LOCK();
	curr=head;

	while(curr){
		if( !( _fstrcmp(curr->ptr->Name, name )) ){
			UNLOCK();
			return curr->ptr->myThread;
		}
		curr=curr->next;
	} 

	UNLOCK();
	return NULL;
}	

ID   ListPCB::getIdByName(TName name){
	LOCK();
	curr=head;

	while(curr){
		if( !( _fstrcmp(curr->ptr->Name, name )) ){
			UNLOCK();
			return curr->ptr->threadID;
		}
		curr=curr->next;
	} 

	UNLOCK();
	return NULL;
}

void ListPCB::checkSL(){
	LOCK();

	curr=head;

	while(curr){
		if( curr->ptr->timeToWakeUp == currTime ) {
			curr->ptr->status = PCB::REA;
			curr->ptr->timeToWakeUp = 0;
			Scheduler::put(curr->ptr);
			//sleepList->delItem(curr->ptr);  deluje kao bolje resenje 
		}
		curr=curr->next;		
	} 	
	sleepList->delAllWokenUp(); // cini mi se da sporije radi
	UNLOCK();
}

void ListPCB::checkBL(){
	LOCK();
	curr=head;
	while(curr){
		if(cnt){
		
		curr->ptr->eveSignal = 1;
		curr->ptr->status = PCB::REA;
		Scheduler::put(curr->ptr);
		this->delItem(curr->ptr);
		}
	}
	UNLOCK();
}            

void ListPCB::timeReset(Time vreme){
	LOCK();
	curr=head;

	while(curr){
		curr->ptr->timeToWakeUp -= vreme; 
		curr=curr->next;		
	} 	
	UNLOCK();
}