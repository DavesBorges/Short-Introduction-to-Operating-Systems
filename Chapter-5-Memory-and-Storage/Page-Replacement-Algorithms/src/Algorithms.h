/*
*	Copyright (c) David de Jesus M. Borges
*	Date:	02/04/2021
*
*	This file contains the declarations of the page replacement algorithms
*/

#ifndef ALGORITHMS_GUARD
#define ALGORITHMS_GUARD
#include "Paging_structures.h" 

/// <summary>
/// First In First Out Algorithm.
/// If no frame is available, the oldest page is the victim
/// </summary>
/// <param name="ctbl"></param>
/// <returns>: the index of the frame should be placed</returns>
int fifo(const Conversion_table& ctbl);

/// <summary>
/// Second Chance Algorithm
/// </summary>
/// <param name="ctbl"></param>
/// <returns>the index of the frame should be placed</returns>
int second_chance(Conversion_table& ctbl);

/// <summary>
/// Least Recently Used Algorithm
/// </summary>
/// <param name="ctbl"></param>
/// <returns>the index of the frame should be placed</returns>
int least_recently(Conversion_table& ctbl);

#endif // !ALGORITHMS_GUARD
