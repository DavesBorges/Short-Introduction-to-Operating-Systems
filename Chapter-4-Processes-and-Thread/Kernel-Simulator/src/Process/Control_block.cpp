/*
*	Author David de Jesus M. Borges
*	Date:	02/04/2021
*
*			This file contains the definitions of functions for conveting back 
*		and forth from int to Priority and the overloaded operators < and ==
*		for Control_block type and < for Priority type
*	
*/

#include "Control_block.h"

bool operator<(const Priority& p1, const Priority& p2) {
	int priority1 = 0;
	int priority2 = 0;
	switch (p1) {
	case Priority::high:
		priority1 = 3;
		break;
	case Priority::medium:
		priority1 = 2;
		break;
	case Priority::low:
		priority1 = 1;
		break;
	default:
		break;
	}

	switch (p2) {
	case Priority::high:
		priority2 = 3;
		break;
	case Priority::medium:
		priority2 = 2;
		break;
	case Priority::low:
		priority2 = 1;
		break;
	default:
		break;
	}

	return priority1 < priority2;
}

Priority int_to_priority(int i) {
	switch (i) {
	case 1:
		return Priority::low;
	case 2:
		return Priority::medium;
	case 3:
		return Priority::high;
	default:
		return Priority::medium;
	}
}

int priority_to_int(const Priority& priority) {
	switch (priority) {
	case Priority::high:
		return 3;
	case Priority::medium:
		return 2;
	case Priority::low:
		return 1;
	default:
		return 2;
	}

}


bool operator == (const Control_block& b1, const Control_block& b2) {
	return b1.pid == b2.pid;
}

bool operator <(const Control_block& b1, const Control_block& b2) {
	return b1.pid < b2.pid;
}