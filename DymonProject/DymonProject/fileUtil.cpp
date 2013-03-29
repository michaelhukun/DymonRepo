//created by Hu Kun 30 Nov 2012
#include "fileUtil.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


using namespace utilities;
using namespace std;


vector<string> FileUtil::split(const string &s, char delim) {
     stringstream ss(s);
    string item;
    vector<string> elems;
    while(getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}