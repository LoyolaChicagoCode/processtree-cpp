/*
 * io.h
 *
 *  Created on: Dec 26, 2011
 *      Author: laufer
 */

#ifndef IO_H_
#define IO_H_

#include <string>

void print_space();
void print_process_info(const int i, const std::string cmd);
void set_stdout_buffer(char* const obuf, const int size);
bool read_line(char* const buf, const int size);
void flush_stdout();

#endif /* IO_H_ */
