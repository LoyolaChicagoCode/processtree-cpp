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
private:
    size_t pid, ppid, cmd;
    bool pidFirst;
public:
    process_parser(const string& line);
    process parse(const string& line) const;
};

#endif /* PARSING_H_ */
