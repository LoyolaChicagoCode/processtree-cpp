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

struct process {
	unsigned int pid, ppid;
	std::string cmd;
	process() { }
	friend std::ostream& operator<<(std::ostream& str, process const& data) {
		return str << data.pid << ": " << data.cmd;
	}
};

#endif /* PROCESS_H_ */
