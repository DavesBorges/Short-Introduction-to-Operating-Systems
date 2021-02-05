/*
*	Copyright (c) David de Jesus M. Borges
*	Date:	02/04/2021
* 
*	This file contains the declarations of functions used to interact with 
*	the user through the commandline
*	
*/

#ifndef CMDLINE_GUARD
#define CMDLINE_GUARD
#include "Paging_structures.h"



/// <summary>
///	Print the frames as a horizontal block
/// If there is a page associated with a frame, 
/// the frame will have the pageId inside its cell
/// </summary>
/// <param name="ctbl">:the Conversion_table containting 
/// the map from Frame's to Page*</param>
void print_frametbl(const Conversion_table& ctbl);

/// <summary>
///	Print the frames as a horizontal block
/// If there is a page associated with a frame, 
/// the frame will have the pageId 
/// inside its cell as well as the activation bit
/// </summary>
/// <param name="ctbl">:the Conversion_table containting 
/// the map from Frame's to Page*</param>
void print_frametbl_second(const Conversion_table& ctbl);

/// <summary>
/// A variant of print_frametbl_second
///  where the last state of the activation bit is printed
/// </summary>
void print_frametbl_second_m(const Conversion_table& ctbl);


void print_pages_disk(const vector<Page>& virtual_memory);

/// <summary>
/// 
/// </summary>
/// <param name="virtual_memory">: list of Pages </param>
/// <param name="time">: the current time in any suitable unit</param>
void access_page_disk(vector<Page>& virtual_memory, int time);


const vector<Page> get_pages_in_memory(const vector<Page>& virtual_memory);

/// <summary>
/// Get the page choosed by the user to cause a page fault
/// </summary>
/// <param name="virtual_memory"></param>
/// <returns>the pageId of the page</returns>
int get_page_fault(const vector<Page>& virtual_memory);

/// <summary>
/// Prints a '^' with the page id below the frame passed as the frame parameter
/// ,assuming that each cell is 4 chacters long, and asks the user to type
/// ENTER to continue 
/// </summary>
/// <param name="ftbl"></param>
/// <param name="frame">: Frame where '^' sould be below</param>
/// <param name="pageId"> the pageId is presented below the '^'</param>
void mark_selected_frame(const Conversion_table& ftbl, int frame, int pageId);


/// <summary>
/// Prints a '^' with the page id below the frame passed as the frame parameter
/// assuming that each cell is 6 chacters long
/// </summary>
/// <param name="ftbl"></param>
/// <param name="frame">: Frame where '^' sould be below</param>
/// <param name="pageId"> the pageId is presented below the '^'</param>
void print_choosed_frame_s(const Conversion_table& ftbl, int frame, int pId);


/// <summary>
/// Asks the user to enter a list of pages to set activation bits to 1
/// </summary>
/// <param name="virtual_memory">: all the pages</param>
void set_activation_bits(vector<Page>& virtual_memory);


#endif // !CMDLINE_BODY_GUARD
