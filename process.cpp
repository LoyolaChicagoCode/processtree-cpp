/*
 * process.h
 *
 *  Created on: Oct 5, 2011
 *      Author: laufer
 */

#include <iostream>

#include "parsing.h"
#include "process.h"

using namespace std;

process::process() { }

void parse_process(process& p, char* const line, Format_Info& fi);

process::process(char* const line, Format_Info& fi) {
	parse_process(*this, line, fi);
}

ostream& operator<<(std::ostream& str, process const& data) {
	return str << data.pid << ": " << data.cmd;
}
