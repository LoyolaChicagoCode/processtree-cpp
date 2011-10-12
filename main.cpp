#include <cstdio>  // popen, fgets
#include <cstring> // strtok
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

//#include "process.h"

using namespace std;

const unsigned int BUF_SIZE = 1 << 16;

//void print_tree(map<int, process>& m, multimap<int, int>& t, int i, int l) {
//	// indent, then print current process
//	for (int k = 0; k < l; k++)
//		cout << "  ";
//	process& proc = m[i];
//	cout << proc << endl;
//	// print children indented by one more level
//	for (multimap<int, int>::iterator e = t.lower_bound(i); e != t.upper_bound(i); e++)
//		print_tree(m, t, e->second, l + 1);
//}


typedef vector<string>::iterator ITER;

int header_index(ITER b, ITER e, string name) {
	ITER it = find(b, e, name);
	return it != e ? it - b : -1;
}

int main(int argc, char* argv[]) {
	char buf[BUF_SIZE];
//	map<int, process> m;
	multimap<int, int> t;

	// analyze header line
	cin.getline(buf, BUF_SIZE);
	const char* tok = strtok(buf, " \t\n");
	vector<string> tokens;
	while (tok != NULL) {
		tokens.push_back(tok);
		tok = strtok(NULL, " \t\n");
	}

	for (vector<string>::iterator i = tokens.begin(); i != tokens.end(); i++)
		cout << *i << " ";
	cout << endl;

//	size_t i_pid = -1, i_ppid = -1, i_cmd = -1;
//	if ((it = find(it_begin, it_end, string("PID"))) != it_end) i_pid = it - it_begin;
//	if ((it = find(it_begin, it_end, string("PPID"))) != it_end) i_ppid = it - it_begin;
//	if ((it = find(it_begin, it_end, string("COMMAND"))) != it_end) i_cmd = it - it_begin;
//	else {
//		cerr << "required header field " << name << " missing!" << endl;
//		exit(1);
//	}
//	size_t i_cmd2 = find(it_begin, it_end, string("COMMAND")) - it_begin;
//	cout << i_pid << " " << i_ppid << " " << i_cmd << " " << i_cmd2 << endl;

	return 0;


	// read lines, parse to process object, and insert into table
//	if (p != NULL) {
//		fgets(buf, sizeof(buf), p); // skip header
//		while (fgets(buf, sizeof(buf), p) != NULL) {
//			process proc(buf, 1, 1, 1);
//			m.insert(pair<int, process>(proc.pid, proc));
//		}
//	}
//
//	// create tree by reversing first table into second one
//	for (map<int, process>::iterator i = m.begin(); i != m.end(); i++) {
//		t.insert(pair<int, int>(i->second.ppid, i->second.pid));
//	}
//
//	// print as tree
//	print_tree(m, t, 1, 0);
}
