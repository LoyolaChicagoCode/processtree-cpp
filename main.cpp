#include <stdio.h>  // popen, fgets
#include <iostream>
#include <map>

#include "process.h"

using namespace std;

const unsigned int BUF_SIZE = 1 << 16;

void print_tree(map<int, process>& m, multimap<int, int>& t, int i, int l) {
	// indent, then print current process
	for (int k = 0; k < l; k++)
		cout << "  ";
	process& proc = m[i];
	cout << proc << endl;
	// print children indented by one more level
	for (multimap<int, int>::iterator e = t.lower_bound(i); e != t.upper_bound(i); e++)
		print_tree(m, t, e->second, l + 1);
}

int main(int argc, char* argv[]) {
	FILE* const p = popen("ps -ef", "r");
	char buf[BUF_SIZE];
	map<int, process> m;
	multimap<int, int> t;

	// read lines, parse to process object, and insert into table
	if (p != NULL) {
		fgets(buf, sizeof(buf), p); // skip header
		while (fgets(buf, sizeof(buf), p) != NULL) {
			process proc(buf);
			m.insert(pair<int, process>(proc.pid, proc));
		}
	}

	// create tree by reversing first table into second one
	for (map<int, process>::iterator i = m.begin(); i != m.end(); i++) {
		t.insert(pair<int, int>(i->second.ppid, i->second.pid));
	}

	// print as tree
	print_tree(m, t, 1, 0);
}
