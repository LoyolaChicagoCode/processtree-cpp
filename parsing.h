/*
 * parsing.h
 *
 *  Created on: Oct 5, 2011
 *      Author: laufer
 */

#ifndef PARSING_H_
#define PARSING_H_

#include <vector>

#include "process.h"

using std::vector;
using std::string;

class process_parser {
public:
    process_parser(const string& line);
    process parse(const string& line) const;
    size_t pid;    // PID field number
    size_t ppid;   // PPID field number
    size_t cmd;    // CMD or COMMAND column (not field number)
};

#endif /* PARSING_H_ */
