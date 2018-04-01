#include "fileIOs_wordPairs.h"

using namespace std;

bool sentenceSplitter(string &fname, vector<std::string> &sentences)
{
    vector<string> vec;
    bool retVal = false;

    if (!fname.empty())
    {
        vec = readFile(fname);

        if (vec.size() > 0)
        {
            sentences = vec;
            retVal = true;
        }
    }

    return retVal;
}

void 
printWordpairs(
    multimap<int, pair<string, string> > &freqWordpair_multimap, 
    string outFname, 
    int topCnt, 
    int botCnt)
{
    ofstream resFile;
    pair<string, string> tempPair;
    int tempVal;

    cout <<"Start write word pairs to file: "<< outFname<<endl;
    try 
    {
        resFile.open(RESULTS_FILENAME);

        for (auto it = freqWordpair_multimap.begin(); it != freqWordpair_multimap.end(); it++)
        {
            if (it->first == topCnt || it->first == botCnt)
            {
                tempPair = it->second;
                tempVal = it->first;

                resFile << "<"<<tempPair.first <<", "<<tempPair.second<<"> : "<<tempVal<<endl;
            }
        }
    }
    catch (const std::ios_base::failure &e)
    {
        std::cout << "Caught an ios_base::failure.\n"
                  << "Error code: " << e.code().value()
                  << " (" << e.code().message() << ")\n"
                  << "Error category: " << e.code().category().name() << '\n';
    }

    cout << "End write word pairs to file: " << outFname << endl;
}

bool 
freqWordpairMmap(
    map<pair<string, string>, int> &wordpairFreq_map, 
    multimap<int, pair<string, string> > &freqWordpair_mmap)
{
    multimap<int, pair<string, string> > freqMmap;
    bool retVal = false;

    for (auto it = wordpairFreq_map.begin(); it != wordpairFreq_map.end(); it++)
    {
        freqMmap.insert(pair<int, pair<string, string> >(it->second, it->first));
    }

    if (freqMmap.size() > 0)
    {
        retVal = true;
        freqWordpair_mmap = freqMmap;
    }

    return retVal;
}

bool 
wordpairMapping(vector<string> &sentences, map<pair<string, string>, int> &wordpairFreq_map)
{
    bool retVal = false;
    vector<string> vec, words;
    string line, tempI, tempJ;
    pair<string, string> mapKey;
    map<pair<string, string>, int> freqMap;
    int count = 0;

    for (auto it = sentences.begin(); it != sentences.end(); it++)
    {
        line = *it;
        vec.push_back(sortSentence(line));
    }

    for (auto it = vec.begin(); it != vec.end(); it++)
    {
//        cout << *it << endl;
        line = *it;
        words = splitString(line);
        for (int i = 0; i < words.size(); i++)
        {
            tempI = words[i];
            //Convert the string to lowercase for comparison
            transform(tempI.begin(), tempI.end(), tempI.begin(), ::tolower);
            for (int j = i+1; j < words.size(); j++)
            {
                tempJ = words[j];
                //Convert the string to lowercase for comparison
                transform(tempJ.begin(), tempJ.end(), tempJ.begin(), ::tolower);
                if ((tempI != tempJ) && (!tempI.empty() || !tempJ.empty()))
                {
                    mapKey = make_pair(words[i], words[j]);
                    if (freqMap.find(mapKey) == freqMap.end())
                    {
                        //This pair is not in the map. Add it
                        freqMap[mapKey] = 1;
                    }
                    else
                    {
                        count = freqMap[mapKey];
                        count++;
                        freqMap[mapKey] = count;
                    }
                }
            }
        }
    }

    if (freqMap.size() > 0)
    {
        retVal = true;
        wordpairFreq_map = freqMap;
    }

    return retVal;
}

/*Read sentences from input file*/
vector<string> getSentences(string &str)
{
    vector<string> wordVector;

    std::size_t prev = 0, pos;
    while ((pos = str.find_first_of(".?", prev)) != std::string::npos)
    {
        if (pos > prev)
            wordVector.push_back(str.substr(prev, pos - prev));
        prev = pos + 1;
    }
    if (prev < str.length())
        wordVector.push_back(str.substr(prev, std::string::npos));

    return wordVector;
}

/*Read input file*/
vector<string> readFile(string filename)
{
    ifstream file;
    string inStr, line;
    vector<string> vec;

    try 
    {
        file.open(filename);

        while(getline(file, line))
        {
            if (line.find_first_not_of(' ') != std::string::npos)
            {
                inStr.append(line);
            }
        }

        vec = getSentences(inStr);
    }
    catch (const std::ios_base::failure &e)
    {
        std::cout << "Caught an ios_base::failure.\n"
                  << "Error code: " << e.code().value()
                  << " (" << e.code().message() << ")\n"
                  << "Error category: " << e.code().category().name() << '\n';
    }

    file.close();

    return vec;
}

/*Get the top & bottom count from the map*/
void getCount(
    int &topCnt,
    int &botCnt,
    multimap<int, pair<string, string> > &freqMmap)
{
    auto it = freqMmap.begin();
    auto rit = freqMmap.rbegin();

    topCnt = it->first;
    botCnt = rit->first;
}

/*compares two strings*/
bool compareStringCaseInsensitive(const string &lhs, const string &rhs)
{

    string::size_type common_length = std::min(lhs.length(), rhs.length());

    for (string::size_type i = 0; i < common_length; ++i)
    {
        if (toupper(lhs[i]) < toupper(rhs[i]))
            return true;
        if (toupper(lhs[i]) > toupper(rhs[i]))
            return false;
    }

    if (lhs.length() < rhs.length())
        return true;
    if (lhs.length() > rhs.length())
        return false; //can ignore

    return false; //equal should return false
}

/*Split sentence into words*/
vector<string>
splitString(const string &str, const string &delimiter)
{
    vector<string> tokens;

    string::size_type lastPos = 0;
    string::size_type pos = str.find(delimiter, lastPos);

    while (string::npos != pos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = pos + delimiter.size();
        pos = str.find(delimiter, lastPos);
    }

    tokens.push_back(str.substr(lastPos, str.size() - lastPos));
    return tokens;
}

/*Sort words in a sentence*/
string
sortSentence(string &line)
{
    vector<string> vec;
    string temp;

    vec = splitString(line);
    sort(vec.begin(), vec.end(), compareStringCaseInsensitive);

    for (int i = 0; i < vec.size(); i++)
    {
        if (!vec[i].empty() && vec[i] != "\"")
        {
            temp.append(vec[i]);

            //Add spaces between each word
            if (i != vec.size() - 1)
            {
                //Do not add space at the end
                temp += " ";
            }
        }
    }

    return temp;
}
