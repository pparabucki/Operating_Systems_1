// Project:	Operativni Sistemi
// File: 	Lock.h
// Date: 	Jul 2012 
// Author: 	Petar Parabucki 115/06
// Contents: 
//			LOCK()  
//			UNLOCK()

#ifndef _Lock_h_
#define _Lock_h_

#define  _lock asm cli;
#define  _unlock asm sti;

#define LOCK() _lock
#define UNLOCK() _unlock

#endif