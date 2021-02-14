/*
*	Copyright (c) David de Jesus M. Borges
*	Date:	02/04/2021
*
*	This file contains the entry point of the Page-Replacement program
*/



#include <iostream>
#include <algorithm>
#include "Algorithms.h"
#include "Cmdline_facilities.h"

vector<Page> create_virtual_memory(int size);

constexpr int vmemory_size = 6;
constexpr int phmemory_size = vmemory_size / 2;

int get_choosed_algo();

void run_fifo(Conversion_table&, vector<Page>&);
void run_second_chance(Conversion_table&, vector<Page>&);
void run_least_reused(Conversion_table&, vector<Page>&);

int time = 0;


int main()
{
	// These exists through the lifetime of our program and passed as arguments
	// funciton calls

	vector<Page> virtual_memory = create_virtual_memory(vmemory_size);
	Conversion_table ctbl = get_conversiontbl(phmemory_size);

	int choosed_algorithm = 0;
	choosed_algorithm = get_choosed_algo();
	switch (choosed_algorithm) {
	case 1:
		run_fifo(ctbl, virtual_memory);
		break;
	case 2:
		run_second_chance(ctbl, virtual_memory);
		break;
	case 3:
		run_least_reused(ctbl, virtual_memory);
		break;
	default:
		std::cerr << "Invalid input " << "Aborting " << '\n';
		return -1;
	}

}


void run_fifo(Conversion_table& ctbl, vector<Page>& virtual_memory) {
	// Print the state of frame, pages, get the choice from the user perform
	//the page replacement using FIFO and then update the frames and pages

	std::cout << "You choosed FIFO (First In First Out) " << '\n';
	while (true) {
		std::cout << "\n\nThe frame table is currently :" << '\n';
		print_frametbl(ctbl);

		print_pages_disk(virtual_memory);
		int choice = get_page_fault(virtual_memory);

		std::cout << "\n\nThe algorithm choosed this frame: \n";
		print_frametbl(ctbl);
		int frame = fifo(ctbl);
		mark_selected_frame(ctbl, frame, choice);

		//Locate the page selected by the user in virtual memory
		Page temp;
		temp.pageId = choice;
		auto it = find(virtual_memory.begin(), virtual_memory.end(), temp);

		if (ctbl[frame]) {					//is the frame being used by a page
			ctbl[frame]->frame_index = -1;	// if so dispose it
		}

		// Place the page in the frame
		ctbl[frame] = &(*it);
		ctbl[frame]->created = time;

		it = find(virtual_memory.begin(), virtual_memory.end(), temp);
		it->frame_index = frame;

		std::cout << "\n============================\n";
		std::cout << "============================\n";
		time++;
	}
}


void run_second_chance(Conversion_table& ctbl, vector<Page>& virtual_memory) {

	std::cout << "You choosed second chance " << '\n';
	while (true) {
		std::cout << "\n\nThe frame table is currently :" << '\n';
		print_frametbl_second(ctbl);


		if (!get_pages_in_memory(virtual_memory).empty()) {

			set_activation_bits(virtual_memory);
			print_frametbl_second(ctbl);
		}

		print_pages_disk(virtual_memory);


		int choice = get_page_fault(virtual_memory);

		int frame = second_chance(ctbl);

		std::cout << "\n\nThe algorithm choosed this frame: \n";
		print_frametbl_second_m(ctbl);
		print_choosed_frame_s(ctbl, frame, choice);

		Page temp;					// Just for comparing
		temp.pageId = choice;
		auto it = find(virtual_memory.begin(), virtual_memory.end(), temp);

		if (ctbl[frame]) {
			ctbl[frame]->frame_index = -1;
		}

		ctbl[frame] = &(*it);
		ctbl[frame]->created = time;

		it = find(virtual_memory.begin(), virtual_memory.end(), temp);
		it->frame_index = frame;

		std::cout << "\n============================\n";
		std::cout << "============================\n";
	}
	time++;
}

void run_least_reused(Conversion_table& ctbl, vector<Page>& virtual_memory) {
	std::cout << "You choosed least recently used " << '\n';

	while (true) {
		std::cout << "\n\nThe frame table is currently :" << '\n';
		print_frametbl(ctbl);


		access_page_disk(virtual_memory, time);

		std::cout << "Select page to generate page fault\n";
		print_pages_disk(virtual_memory);
		int choice = get_page_fault(virtual_memory);

		int frame = least_recently(ctbl);


		std::cout << "\n\nThe algorithm choosed this frame: \n";
		print_frametbl(ctbl);
		mark_selected_frame(ctbl, frame, choice);

		Page temp;					// Just for comparing
		temp.pageId = choice;
		auto it = find(virtual_memory.begin(), virtual_memory.end(), temp);

		if (ctbl[frame]) {
			ctbl[frame]->frame_index = -1;
		}

		ctbl[frame] = &(*it);
		ctbl[frame]->last_acess = time;
		ctbl[frame]->created = time;

		it = find(virtual_memory.begin(), virtual_memory.end(), temp);
		it->frame_index = frame;

		std::cout << "\n============================\n";
		std::cout << "============================\n";
		time++;
	}
}

vector<Page> create_virtual_memory(int size)
{
	vector<Page> virtual_memory;

	for (int i = 0; i < size; ++i) {
		Page page;
		page.pageId = i;
		page.frame_index = -1;

		virtual_memory.push_back(page);
	}

	return virtual_memory;
}

int get_choosed_algo() {
	int input = 0;

	std::cout << "Select wich algorithm you want to use by  "
		<< "typing the number and press ENTER" << '\n';
	std::cout << "1 " << "FIFO" << '\n';
	std::cout << "2 " << "Second Chance " << '\n';
	std::cout << "3 " << "Least recently used" << '\n';

	std::cin >> input;

	return input;
}
