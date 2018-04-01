#include "fileIOs_wordPairs.h"

using namespace std;

int 
main(int argc, char *argv[])
{
    string inputFile;
    int rc = RC_SUCCESS;
    vector<string> sentences;
    map<pair<string, string>, int> wordpairFreq_map;
    multimap<int, pair<string, string> > freqWordpair_mmap;
    bool retVal = false;
    int topCnt, botCnt;

    if (argc == 2)
    {
        inputFile = argv[1];
    }
    else{
        rc = RC_ERROR;
    }

    if (RC_SUCCESS != rc)
    {
        cout << endl
             << "Usage: " << argv[0] << " path to input file" << endl;
        return rc;
    }
    else
    {
        retVal = sentenceSplitter(inputFile, sentences);
        if (retVal)
        {
            retVal = wordpairMapping(sentences, wordpairFreq_map);

            if (retVal)
            {
                retVal = freqWordpairMmap(wordpairFreq_map, freqWordpair_mmap);

                if (retVal)
                {
                    getCount(topCnt, botCnt, freqWordpair_mmap);
                    printWordpairs(freqWordpair_mmap, RESULTS_FILENAME, topCnt, botCnt);
                }
            }
        }
    }

    return 0;
}