#include "bits/stdc++.h"

#define RC_SUCCESS 0 // No error
#define RC_ERROR -1  // An unexpected error has occured

#define RESULTS_FILENAME     "results.txt"

bool sentenceSplitter(std::string &fname, std::vector<std::string> &sentences);
bool wordpairMapping(std::vector<std::string> &sentences, std::map<std::pair<std::string, std::string>, int> &wordpairFreq_map);
bool freqWordpairMmap(std::map<std::pair<std::string, std::string>, int> &wordpairFreq_map, std::multimap<int, std::pair<std::string, std::string> > &freqWordpair_mmap);
void printWordpairs(std::multimap<int, std::pair<std::string, std::string> > &freqWordpair_multimap, std::string outFname, int topCnt, int botCnt);

//utility functions
std::vector<std::string> readFile(std::string filename);
std::vector<std::string> getSentences(std::string &str);

void getCount(int &topCnt, int &botCnt, std::multimap<int, std::pair<std::string, std::string> > &freqMmap);
bool compareStringCaseInsensitive(const std::string &lhs, const std::string &rhs);
std::vector<std::string> splitString(const std::string &str, const std::string &delimiter = " ");
std::string sortSentence(std::string &line);