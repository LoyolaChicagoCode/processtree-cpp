#include <vector>
#include <unordered_map>

#include "io.h"
#include "process.h"
#include "parsing.h"

using std::unordered_map;
using std::vector;
using std::pair;
using std::string;

typedef vector<unsigned int> p_list;
typedef unordered_map<unsigned int, string> cmd_map;
typedef unordered_map<unsigned int, vector<unsigned int> > ppid_map;

const size_t LINE_BUF_SIZE = 16348;

void print_tree(cmd_map& m, ppid_map& t, unsigned int i, unsigned int l) {
	// indent, then print current process
    print_spaces(l);
	print_process_info(i, m[i]);
	// print children indented by one more level
	for (p_list::iterator e = t[i].begin(); e != t[i].end(); e++)
		print_tree(m, t, *e, l + 1);
}

int main(const int argc, const char* const argv[]) {
	char line[LINE_BUF_SIZE];
	cmd_map m;
	ppid_map t;

	// analyze header line
    init_io();
	read_line(line, LINE_BUF_SIZE);
	process_parser parser(line);

	// read lines, parse to process object, and insert into table
	process proc;
	while (read_line(line, LINE_BUF_SIZE)) {
		parser.parse(proc, line);
		m.insert(pair<unsigned int, string&>(proc.pid, proc.cmd));
		t[proc.ppid].push_back(proc.pid);
	}

	// print as tree
	for (p_list::iterator e = t[0].begin(); e != t[0].end(); e++)
		print_tree(m, t, *e, 0);
	flush_stdout();
}
