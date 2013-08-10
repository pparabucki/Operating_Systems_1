// Project: Operativni Sistemi
// File:  ListEvent.h
// Date:  Decembar 2012
// Author:  Petar Parabucki 115/06
// Contents:  
//      Class: ListEvent

#include <stdio.h>
#include <string.h> 
#include <iostream.h>
#include <dos.h>
#include "Schedule.h"
#include "PCB.h"
#include "Lock.h"
#include "ListEv.h"


int ListEvent::count(){
  return cnt;
}

void ListEvent::addItem(Event* newEvent){
  
  LOCK();
   Elem *novi = new Elem(newEvent);
  if(!head) head=novi;
  else tail->next=novi;
  tail=novi;
  novi->next=NULL;
  cnt++;

  UNLOCK();
 }

void ListEvent::delItem(Event* delEvent){
  
  LOCK();
  curr=head;
  Elem *prev = NULL;
  while(curr)
      if(curr->ptr != delEvent){
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
  UNLOCK();
}

void ListEvent::delAll(){
  
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

IVTEntry* ListEvent::getEntry(IVTNo iNum){

  return nizIVT[iNum];
}

void ListEvent::signalAll(){

  LOCK();

  delAll();

  UNLOCK();

}