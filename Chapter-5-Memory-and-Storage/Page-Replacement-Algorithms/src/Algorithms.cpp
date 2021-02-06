/*
*	Copyright (c) David de Jesus M. Borges
*	Date:	02/04/2021
* 
*	This file contains the implementation of the paging replacement algorithms
*/

#include <algorithm>
#include "Algorithms.h"

using namespace std;

Conversion_table::iterator available_frame(Conversion_table& ctbl) {
	for (auto it = ctbl.begin(); it != ctbl.end(); ++it) {
		int i = it - ctbl.begin();

		if (!ctbl[i]) {
			return ctbl.begin() + i;
		}
	}
	return ctbl.end();
}


bool fifo_compare(const Page* p1, const Page* p2) 
{
	if (!p1) {
		return false;
	}
	if (!p2) {
		return true;
	}
	return p1->created < p2->created;
}



int fifo(const Conversion_table& ctbl)
{
	int min_index = 0;
	bool found = false;
	int minimum;
	for (int i = 0; i < ctbl.size(); ++i) {
		if(ctbl[i]){
			if (!found) {
				found = true;
				minimum = ctbl[i]->created;
				min_index = i;
			}
			else if (ctbl[i]->created < minimum) {
				min_index = i;
				minimum = ctbl[i]->created;
			}
		}
		else {
			return i;
		}
	}
	return min_index;
}


int second_chance(Conversion_table& ctbl)
{
	auto it = available_frame(ctbl);
	if (it != ctbl.end()) {
		return it - ctbl.begin();
	}
	for (int i = 0; i < ctbl.size(); ++i) {
		
		if (ctbl[i]->had_chance) {

			ctbl[i]->has_chance = false;
			ctbl[i]->had_chance = true;
		}
		else {
			ctbl[i]->had_chance = false;
			return i;
		}
	}
}


int least_recently(Conversion_table& ctbl) 
{
	auto it = available_frame(ctbl);

	if (it != ctbl.end()) {
		return it - ctbl.begin();
	}
	bool found = false;
	int minimum;
	int min_index = 0;

	for (int i = 0; i < ctbl.size(); ++i) {
		if (ctbl[i]) {
			if (!found) {
				found = true;
				minimum = ctbl[i]->last_acess;
				min_index = i;
			}
			else if (ctbl[i]->last_acess < minimum) {
				min_index = i;
				minimum = ctbl[i]->last_acess;
			}
		}
		else {
			return i;
		}
	}
	return min_index;
}