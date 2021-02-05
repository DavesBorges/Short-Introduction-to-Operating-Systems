/*	
*	Copyright(c) David de Jesus M.Borges
*	Date:	02/04/2021
*	
*	This file contains the definition of 
*	the Paging System structures
*
*/

#ifndef SYSTEM_STRUCTURES_GUARD
#define SYSTEM_STRUCTURES_GUARD

#include <vector>

using namespace std;

typedef unsigned char byte;

/// the index represents the index of the frame in the frame_table
///the value represents the associated page. If none is associated is set to-1


constexpr int memory_size = 16;					    // our RAM size
constexpr int frames_number = 4;			
constexpr int frame_size=memory_size/frames_number;	// split our memory evenly

struct Frame{
	byte memory[frame_size];		// the array of bytes in a Frame
};

struct Page{	
	int pageId;
	byte memory[frame_size];
	int frame_index;				// Negative number means not in memory
	int created{ 0 };				// For FIFO algorithm
	bool has_chance{ 0 };			// For Second chance algorithm	
	bool had_chance{ 0 };			// previous state of has_chance;
	int last_acess{ 0 };			// Time since last acess
};

// Convert from Frame(index) to the page associated with it (value)
typedef vector<Page*> Conversion_table;	
typedef vector<Frame> Physical_memory;


inline bool operator == (const Page& p1, const Page& p2) {
	return p1.pageId == p2.pageId;
}

inline Conversion_table get_conversiontbl(int size)
{
	Conversion_table table;
	for (int i = 0; i < size; ++i) {
		table.push_back(nullptr);
	}

	return table;
}


#endif

