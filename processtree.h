#ifndef PROCESSTREE_H_
#define PROCESSTREE_H_

#include <string>
#include <vector>
#include <forward_list>
#include <unordered_map>

typedef std::unordered_map<unsigned int, const std::string> cmd_map;
typedef std::unordered_map<unsigned int, std::vector<unsigned int> > ppid_map;

// parsing info from header row of ps command
struct parser {
    long pid, ppid;    // field numbers of PID and PPID in header row
    unsigned long cmd; // column of CMD or COMMAND field in header row
};

// representation of a single process
struct process {
    long pid, ppid;
    std::string cmd;
};

void read_input(std::istream& input, std::string& header, std::forward_list<std::string>& lines);
parser parse_header(const std::string& header);
void parse_lines(const parser& parser, const std::forward_list<std::string>& lines, std::forward_list<process>& processes);
void insert_processes(const std::forward_list<process>& processes, cmd_map& m, ppid_map& t);
void print_tree(FILE* dest, const cmd_map& m, const ppid_map& t, unsigned int i, unsigned int l);
void print_forest(FILE* dest, const cmd_map& m, const ppid_map& t);

#endif // PROCESSTREE_H_
