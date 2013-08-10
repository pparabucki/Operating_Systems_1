// Project:	Operativni Sistemi
// File: 	Nit.h
// Date: 	Jul 2012
// Author: 	Petar Parabucki 115/06 
// Contents: 
//			Classes: MyIdle

#ifndef _Nit_h_
#define _Nit_h_

#include <iostream.h>
#include <string.h>
#include "keyevent.h"
#include "PCB.h"

class PCB;
class Thread;

class MyIdle : public Thread{

public:

	friend PCB;
	friend int main(int,char**);
	friend Thread;
	friend void interrupt timer();

	MyIdle();

	void run();
	void start();
};
#endif