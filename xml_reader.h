#ifndef XML_READER_H
#define XML_READER_H

#include <bits/stdc++.h>
using namespace std;
class Xml_line {
    public:
    string line;
    string tag;
    unordered_map<string, string> attbs;

    void parse() {
        stringstream ss;
        ss.str(line);
        vector<string> tokens;
        ss >> tag;
        string token;
        string a1, a2;
        while (getline(ss, a1, '\"') && getline(ss, a2, '\"')) {
            a1 = a1.substr(a1.find_first_not_of(' ')); // xoa dau cach o dau
            a1.pop_back(); // bo dau =
            attbs[a1] = a2;
        }
    }
    
    void setLine(string line) {
        if (line.empty()) return;
        line = line.substr(line.find_first_not_of(' ')); // xoa dau cach o dau
        if (line[0] == '<') line = line.substr(1); // xoa dau < o dau
        if (line.back() == '>') line.pop_back(); // xoa dau > o cuoi
        if (line.back() == '/') line.pop_back(); // xoa dau / o cuoi

        this->line = line;
        this->attbs.clear();
        this->parse();
    }

    Xml_line() {
        this->line = "";
    }

    Xml_line(string line) {
        this->setLine(line);
    }
};
#endif // !XML_READER_H