/* 	Author: David de Jesus M. Borges 
 *	Date:	02/04/2021 
 * 
 *	This file contains:
 *		- The declaration of the Command struct used to store one single
 *		pseudo program instruction. Ex: "load 25"
 *
 */


struct Command {
    std::string m_operator;         
    int operand;
};


std::ostream& operator<<(std::ostream& outstream, const Command& c) {
    return outstream << c.m_operator << " " << c.operand;
}

std::istream& operator>>(std::istream& inputstream, Command& c) {
    return inputstream >> c.m_operator >> c.operand;
}
