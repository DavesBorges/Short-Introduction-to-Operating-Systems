#include "Control_block.h"

bool operator<(const Priority& p1, const Priority& p2) {
	int priority1 = 0;
	int priority2 = 0;
	switch (p1) {
	case Priority::HIGH:
		priority1 = 3;
		break;
	case Priority::MEDIUM:
		priority1 = 2;
		break;
	case Priority::LOW:
		priority1 = 1;
		break;
	default:
		break;
	}

	switch (p2) {
	case Priority::HIGH:
		priority2 = 3;
		break;
	case Priority::MEDIUM:
		priority2 = 2;
		break;
	case Priority::LOW:
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
		return Priority::LOW;
	case 2:
		return Priority::MEDIUM;
	case 3:
		return Priority::HIGH;
	default:
		return Priority::MEDIUM;
	}
}

int priority_to_int(const Priority& priority) {
	switch (priority) {
	case Priority::HIGH:
		return 3;
	case Priority::MEDIUM:
		return 2;
	case Priority::LOW:
		return 1;
	}
}


bool operator == (const Control_block& b1, const Control_block& b2) {
	return b1.pid == b2.pid;
}

bool operator <(const Control_block& b1, const Control_block& b2) {
	return b1.pid < b2.pid;
}