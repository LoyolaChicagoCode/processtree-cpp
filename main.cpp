#include <iostream>

#include <string>
#include <vector>
#include <unordered_map>

#include "io.h"
#include "process.h"
#include "parsing.h"

using std::unordered_map;
using std::vector;
using std::pair;
using std::string;

const unsigned int LINE_BUF_SIZE = 16348;

void print_tree(unordered_map<int, string>& m, unordered_map<int, vector<int> >& t, int i, int l) {
	// indent, then print current process
	for (int k = 0; k < l; k++)
		print_space();
	print_process_info(i, m[i]);
	// print children indented by one more level
	for (vector<int>::iterator e = t[i].begin(); e != t[i].end(); e++)
		print_tree(m, t, *e, l + 1);
}

int main(int argc, char* argv[]) {
	char buf[LINE_BUF_SIZE];
	unordered_map<int, string> m;
	unordered_map<int, vector<int> > t;

	// analyze header line
	read_line(buf, LINE_BUF_SIZE);
	process_parser parser(buf);

	// read lines, parse to process object, and insert into table
	process proc;
	while (read_line(buf, LINE_BUF_SIZE)) {
		parser.parse(proc, buf);
		m.insert(pair<int, string>(proc.pid, proc.cmd));
		t[proc.ppid].push_back(proc.pid);
	}

	// print as tree
	for (vector<int>::iterator e = t[0].begin(); e != t[0].end(); e++)
		print_tree(m, t, *e, 0);
	flush_stdout();
}
