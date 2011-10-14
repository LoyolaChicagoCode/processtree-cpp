/*
 * process.h
 *
 *  Created on: Oct 5, 2011
 *      Author: laufer
 */

#ifndef PROCESS_H_
#define PROCESS_H_

#include <iostream>
#include <string>

#include "parsing.h"

struct process {
	unsigned int pid, ppid;
	std::string cmd;
	process() { }
	process(char* const line, Format_Info const& fi) { fi.parse_process(*this, line); }
	friend std::ostream& operator<<(std::ostream& str, process const& data) {
		return str << data.pid << ": " << data.cmd;
	}
};

#endif /* PROCESS_H_ */
