// Project:	Operativni Sistemi
// File: 	Semaphor.h
// Date: 	Jul 2012
// Author: 	Petar Parabucki 115/06 
// Contents: 
//			Class: Semaphor 
#ifndef _Semaphor_h_
#define _Semaphor_h_


extern int semPreempt;

class KernelSem;

class Semaphore {
public:
	Semaphore (int init = 1);
   ~Semaphore ();

   virtual int wait ();
   virtual void signal ();

   int val () const; // Vraca trenutnu vrednost semafora

private:
	KernelSem* myImpl;
};
#endif 