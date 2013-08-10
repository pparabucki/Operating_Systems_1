// Project:	Operativni Sistemi
// File: 	Thread.h
// Date: 	Jul 2012
// Author: 	Petar Parabucki 115/06
// Contents: 
//			Classes: Thread

#ifndef _Thread_h_
#define _Thread_h_

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time; 			// time , x 55ms
const Time defaultTimeSlice = 2;	// default = 2*55mss
typedef int ID;
typedef char* TName;


class PCB; // Kernel's implementation of a user's thread 

class Thread {

public:

	void start();							
	int waitToComplete();					// returns: 1 - run completed 	, 0 - woken up  
	int wakeUp();							// returns: 1 - deblocked 		, 0 - no effect	
	static int sleep(Time timeToSleep); 	// returns: 1 - run completed 	, 0 - woken up

	ID getID();
	TName getName();

	static Thread * getThreadById(ID id);
	static ID getIdOf(TName name); 

	virtual ~Thread();			// u destruktoru treba da postoji waittocomplete .. 

protected:
	
	friend class PCB;
	Thread (TName name ,  StackSize stackSize=defaultStackSize, Time timeSlice=defaultTimeSlice); 
	virtual void run() {}

private:
	
	PCB* myPCB;
	
};

void dispatch();

#endif 