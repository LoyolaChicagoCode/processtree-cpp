/*
 * process.cpp
 *
 *  Created on: Oct 5, 2011
 *      Author: laufer
 */

#ifndef PROCESS_H_
#define PROCESS_H_

#include <iostream>
#include <string>

using namespace std;

struct process {
	string uid;
	unsigned int pid, ppid, c;
	string stime, tty, time, cmd;
	friend std::ostream& operator<<(std::ostream& str, process const& data);
};

ostream& operator<<(std::ostream& str, process const& data) {
	str << "Proc(" << data.uid << ", " << data.pid << ", " << data.ppid << ")";
	return str;
}

#endif /* PROCESS_H_ */
