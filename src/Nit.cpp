// Project:	Operativni Sistemi
// File: 	Nit.cpp 
// Date: 	Jul 2012
// Author: 	Petar Parabucki 115/06
// Contents: 
//			Classes: MyIdle

#include <iostream.h>
#include <string.h>
//#include "Thread.h"
#include "Schedule.h"
#include "Nit.h"

extern ListPCB *sleepList;
extern Time currTime;
extern int kraj;

MyIdle::MyIdle() : Thread("MYIDLE",0,1){};

void MyIdle::run(){
	while(!kraj){ }	
	cout.flush();
}

void MyIdle::start(){
	Thread::start();
}
