// Project: Operativni Sistemi
// File:  main.cpp
// Date:  Jul 2012
// Author:  Petar Parabucki 115/06 
// Contents: 
//     functions: int main() 

#include <iostream.h>
#include <stdio.h>
#include "IVTEntry.h"
#include "Nit.h"
#include <lock.h>


class ListPCB;
int kraj=0; 
unsigned oldTimerSEG = NULL , oldTimerOFF = NULL ;
extern unsigned curSP, curSS, curBP;
extern IVTEntry* nizIVT[];

ListPCB *list;
MyIdle* myidle = new MyIdle();

int userMain(int,char**);

void initialize(){
  asm{
          cli
          push es
          push ax

          mov ax,0  
          mov es,ax

          mov ax, word ptr es:0022h 
          mov word ptr oldTimerSEG, ax 
          mov ax, word ptr es:0020h  
          mov word ptr oldTimerOFF, ax 
          mov word ptr es:0022h, seg timer 
          
          mov word ptr es:0020h, offset timer 

          mov ax, oldTimerSEG  
          mov word ptr es:0182h, ax
          mov ax, oldTimerOFF
          mov word ptr es:0180h, ax

          pop ax
          pop es
          sti
      }
}
void restore(){
  asm {
          cli
          push es
          push ax

          mov ax,0
          mov es,ax

          mov ax, word ptr oldTimerSEG
          mov word ptr es:0022h, ax
          mov ax, word ptr oldTimerOFF
          mov word ptr es:0020h, ax

          pop ax
          pop es
          sti
      }
}


int main( int argc, char* argv[] ){
  
  LOCK();
  initialize();
    
  PCB *systemPCB = new PCB("MAIN",0,1,NULL);

  systemPCB->status = PCB::MAIN;

  PCB::running = systemPCB;

  list = new ListPCB();

  list->addItem(PCB::running);

  UNLOCK();
   
   
  int a = userMain(argc,argv);
  LOCK();
  //cout<<"Povratna vrednost userMain() = "<<a<<endl;
  UNLOCK();
  myidle->start();
  
  
  restore();
  LOCK();

  systemPCB->status = PCB::FIN;
  //myidle->myPCB->status = PCB::FIN;
  kraj=1;
  delete systemPCB;
  delete myidle;
  delete [] nizIVT;
   
  UNLOCK();

  return a;
}