/*
*	Copyright (c) David de Jesus M. Borges
*	Date:	02/04/2021
*
*	This file contains the implementation of the funtions used to interact with
*	the user through the command line
*/


#include <iostream>
#include <sstream>
#include <iomanip>
#include <exception>
#include <algorithm>
#include "Cmdline_facilities.h"



using namespace std;



void print_frametbl(const Conversion_table& ftbl)
{	
	 
	cout	<< "FRAME TABLE" <<  '\n' << '\n';
	for (int i = 0; i < ftbl.size(); ++i) {
		if (ftbl[i]) {
			std::cout << setw(3) << ftbl[i]->pageId  <<  '|';
		}
		else {
			std::cout << setw(4) << "____|" ;
		}
	}
	std::cout << '\n' << '\n';
}

void print_frametbl_second(const Conversion_table& ctbl)
{
	cout << "FRAME TABLE" << '\n' << '\n';
	cout << "P" << "  A";

	cout << "\t\t\t" << "P = page \t" << "A = activation bit" << '\n';
	for (int i = 0; i < ctbl.size(); ++i) {

		if (ctbl[i]) {
			std::cout<<ctbl[i]->pageId << "  -"	// print the eactivaiont bit
				<< ctbl[i]->has_chance << '|';
		}
		else {
			std::cout << setw(3) << "  -0|";
		}
	}
	std::cout << '\n' << '\n';
}


void print_frametbl_second_m(const Conversion_table& ctbl) {
	cout << "FRAME TABLE" << '\n' << '\n';
	cout << "P" << "  A";

	cout << "\t\t\t" << "P = page \t" << "A = activation bit" << '\n';
	for (int i = 0; i < ctbl.size(); ++i) {

		if (ctbl[i]) {
			std::cout << ctbl[i]->pageId << "  -"
				<< ctbl[i]->had_chance << '|';
		}
		else {
			std::cout << setw(3) << "  -0|";
		}
	}
	std::cout << '\n' << '\n';
}


void print_pages_disk(const vector<Page>& virtual_memory) 
{
	std::cout << "Select one of the available pages to generate a page fault"
		<< "\n AVAILABLE PAGES "<< "\n";
	for (int i = 0; i < virtual_memory.size(); ++i) {
		if (virtual_memory[i].frame_index < 0) {		
			std::cout << setw(3) << virtual_memory[i].pageId << ';';
		}
	}
	std::cout << '\n' << '\n';
}

const vector<Page> get_pages_in_memory(const vector<Page>& virtual_memory)
{
	vector<Page> pages_in_memory;
	for (auto& page : virtual_memory) {
		if (page.frame_index > -1) {
			pages_in_memory.push_back(page);
		}
	}
	return pages_in_memory;
}

const vector<Page> get_pages_in_disk(const vector<Page>& virtual_memory) 
{
	vector<Page> pages_in_disk;
	for (auto& page : virtual_memory) {
		if (page.frame_index < 0) {
			pages_in_disk.push_back(page);
		}
	}
	return pages_in_disk;
}

int get_page_fault(const vector<Page>& virtual_memory) 
{
	// Get and validate input
	int input = 0;
	if (! (cin >> input)) {
		std::cout << "Input error " << '\n';
		if (cin.bad())
			throw runtime_error("Invalid input");

	}

	
	Page temp;				// variable to verify if input == page_id 
	temp.pageId = input;

	// Get only the pages that are in the disk and look for the choosed page
	auto pages_in_disk = get_pages_in_disk(virtual_memory);
	auto it = find(pages_in_disk.begin(), pages_in_disk.end(), temp);
	
	// A loop to get a valid page from the user case the first input were 
	// not in disk
	while (it == pages_in_disk.end()) {
		cout << "Please enter a page that is in the available pages" << '\n';
		std::cin >> input;
		
		if (cin.bad())
			throw runtime_error("Invalid input");

		temp.pageId = input;
		it = find(pages_in_disk.begin(), pages_in_disk.end(), temp);
	}

	std::cout << "\n\n Page fault generated for " << input << '\n';
	return input;
}


void mark_selected_frame(const Conversion_table& ftbl, int frame, int pageId) 
{
	int setw_argument = frame * 5 + 2;
	std::cout << setw(setw_argument) << "^" << '\n';
	std::cout << setw(setw_argument) << pageId << '\n';
	std::cout << "Press ENTER to continue " << '\n';

	string line;
	getline(cin, line);
	getline(cin, line);
}

void print_choosed_frame_s(const Conversion_table& ftbl, int frame, int pageId)
{
	std::cout << char(8) << char(8);
	int setw_argument = frame * 7 + 2;
	std::cout << setw(setw_argument) << "^" << '\n';
	std::cout << setw(setw_argument) << pageId << '\n';
	std::cout << "Press ENTER to continue " << '\n';

	string line;
	getline(cin, line);
	getline(cin, line);
}

void set_activation_bits(vector<Page>& virtual_memory) {
	
	auto pages_in_memory = get_pages_in_memory(virtual_memory);
	if (pages_in_memory.empty()) {
		return;
	}


	std::cout << "Select pages to set Activation bit to 1" << '\n';
	std::cout << "Enter a list of pages and then press Enter " << '\n';

	// Get the full line and then parse it
	std::string line;
	std::getline(cin, line);
	if (line == "")
		std::getline(cin, line);
	istringstream stream{ line };

	int input;
	while (!stream.eof()) {
		stream >> input;
		auto& vm = virtual_memory;

		Page tempPage;
		tempPage.pageId = input;

		auto it = find(vm.begin(), vm.end(), tempPage);
		if (it == vm.end()) {
			std::cout << "Invalid page " << '\n';
		}
		else if (it->frame_index < 0) {
			std::cout << "page " << input << " is not in memory" << '\n';
		}
		else {
			it->has_chance = true;
			it->had_chance = true;
		}
	}

	std::cout << '\n';
}

void access_page_disk(vector<Page>& virtual_memory, int time) 
{
	auto pages_in_memory = get_pages_in_memory(virtual_memory);

	if (pages_in_memory.empty()) {
		return;
	}
	
	std::cout << "´Select pages to acces :" << '\n';
	std::cout << "Enter a list of pages and then press Enter " << '\n';

	std::string line;
	std::getline(cin, line);
	if (line == "")
	std::getline(cin, line);
	istringstream stream{ line };

	int input;
	while (stream.good()) {
		stream >> input;
		
		auto& vm = virtual_memory;

		Page tempPage;
		tempPage.pageId = input;

		auto it = find(vm.begin(), vm.end(), tempPage);
		if (it == vm.end()) {
			std::cout << "Invalid page " << '\n';
		}
		else if (it->frame_index < 0 {
			std::cout << "page " << input << " is not in memory" << '\n';
		}
		else {
			it->last_acess = time;
		}
	}

	std::cout << '\n';
}