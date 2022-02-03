/*
 * io.h
 *
 *  Created on: Dec 26, 2011
 *      Author: laufer
 */

#ifndef IO_H_
#define IO_H_

#include <string>

void print_spaces(unsigned int n);
void print_process_info(unsigned int i, const std::string& cmd);
void init_io();
bool read_line(char* buf, size_t size);
void flush_stdout();

#endif /* IO_H_ */
