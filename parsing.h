/*
 * parsing.h
 *
 *  Created on: Oct 5, 2011
 *      Author: laufer
 */

#ifndef PARSING_H_
#define PARSING_H_

#include "process.h"

class process_parser {
private:
	size_t first, second, cmd;
	bool pidFirst;
public:
	process_parser(char* const header);
	void parse(process& p, char* const line) const;
};

#endif /* PARSING_H_ */
