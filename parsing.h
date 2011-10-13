/*
 * parsing.h
 *
 *  Created on: Oct 5, 2011
 *      Author: laufer
 */

#ifndef PARSING_H_
#define PARSING_H_

#include "process.h"

class Format_Info {
private:
	size_t first, second, cmd;
	bool pidFirst;
public:
	Format_Info(char* const header);
	void parse_process(process& p, char* const line);
};

#endif /* PARSING_H_ */
