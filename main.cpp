//#define USE_IOSTREAM 1

#ifdef USE_IOSTREAM
	#include <iostream>
#else
	#include <cstdio>  // popen, fgets
#endif
#include <string>
#include <vector>
#include <hash_map>


#include "process.h"

using __gnu_cxx::hash_map;
using std::vector;
using std::pair;
using std::string;

const unsigned int LINE_BUF_SIZE = 2048;
const unsigned int IO_BUF_SIZE = 8192;

void print_tree(hash_map<int, string>& m, hash_map<int, vector<int> >& t, int i, int l) {
	// indent, then print current process
	for (int k = 0; k < l; k++)
#ifdef USE_IOSTREAM
		std::cout << ' ';
#else
		putchar(' ');
#endif
#ifdef USE_IOSTREAM
	std::cout << i << ": " << m[i] << '\n';
#else
	printf("%d: ", i);
	puts(m[i].c_str());
#endif
	// print children indented by one more level
	for (vector<int>::iterator e = t[i].begin(); e != t[i].end(); e++)
		print_tree(m, t, *e, l + 1);
}

int main(int argc, char* argv[]) {
	char buf[LINE_BUF_SIZE];
	char obuf[IO_BUF_SIZE];
	hash_map<int, string> m;
	hash_map<int, vector<int> > t;

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
		m.insert(pair<int, string>(proc.pid, proc.cmd));
		t[proc.ppid].push_back(proc.pid);
	}

	// print as tree
	for (vector<int>::iterator e = t[0].begin(); e != t[0].end(); e++)
		print_tree(m, t, *e, 0);
#ifdef USE_IOSTREAM
	std::cout.flush();
#else
	fflush(stdout);
#endif
}
