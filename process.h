/*
 * process.h
 *
 *  Created on: Oct 5, 2011
 *      Author: laufer
 */

#ifndef PROCESS_H_
#define PROCESS_H_

#ifdef USE_IOSTREAM
	#include <iostream>
#endif
#include <string>

struct process {
	unsigned int pid, ppid;
	std::string cmd;
#ifdef USE_IOSTREAM
	friend std::ostream& operator<<(std::ostream& str, process const& data) {
		return str << data.pid << ": " << data.cmd;
	}
#endif
};

#endif /* PROCESS_H_ */
