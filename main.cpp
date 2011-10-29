//#define USE_IOSTREAM 1

#include <cstring> // strtok
#ifdef USE_IOSTREAM
	#include <iostream>
#else
	#include <cstdio>  // popen, fgets
#endif
#include <map>

#include "process.h"

using std::map;
using std::multimap;
using std::pair;

const unsigned int BUF_SIZE = 1 << 16;

void print_tree(map<int, process>& m, multimap<int, int>& t, int i, int l) {
	// indent, then print current process
	for (int k = 0; k < l; k++)
#ifdef USE_IOSTREAM
		std::cout << ' ';
#else
		putchar(' ');
#endif
#ifdef USE_IOSTREAM
	std::cout << m[i] << '\n';
#else
	printf("%d: ", m[i].pid);
	puts(m[i].cmd.c_str());
#endif
	// print children indented by one more level
	for (multimap<int, int>::iterator e = t.lower_bound(i); e != t.upper_bound(i); e++)
		print_tree(m, t, e->second, l + 1);
}

int main(int argc, char* argv[]) {
	char buf[BUF_SIZE];
	char obuf[8192];
	map<int, process> m;
	multimap<int, int> t;

	// analyze header line
#ifdef USE_IOSTREAM
	std::cout.rdbuf()->pubsetbuf(obuf, 18192);
	std::cin.getline(buf, sizeof(buf));
#else
	setbuf(stdout, obuf);
	fgets(buf, sizeof(buf), stdin);
#endif
	Format_Info fi(buf);

	// read lines, parse to process object, and insert into table
#ifdef USE_IOSTREAM
	while (std::cin.getline(buf, sizeof(buf)) != NULL) {
#else
	while (fgets(buf, sizeof(buf), stdin) != NULL) {
#endif
		process proc(buf, fi);
		m.insert(pair<int, process>(proc.pid, proc));
		t.insert(pair<int, int>(proc.ppid, proc.pid));
	}

	// print as tree
	for (multimap<int, int>::iterator e = t.lower_bound(0); e != t.upper_bound(0); e++)
		print_tree(m, t, e->second, 0);
#ifdef USE_IOSTREAM
	std::cout.flush();
#else
	fflush(stdout);
#endif
}
