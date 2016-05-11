#pragma once

#include <iostream>
#include <fstream>
#include <dirent.h> 
#include <string>
#include <vector>
#include <regex>
#include <fstream>
#include <iostream>
#include <unordered_set>

#include <boost/tokenizer.hpp>
//Boost is required to run this software. To obtain a copy of boost, visit http://www.boost.org/ and download the most current release. Boost works with most modern compilers, and is tested on many.
//For a list of compatible compilers, see the release notes of the most current version.

void twitterDataReader(std::vector<std::string> tweetList, std::unordered_set<std::string> posDic, std::unordered_set<std::string> negDic);
void populateDictionaries(std::ifstream &negWordFileStream, std::ifstream &posWordFileStream, std::unordered_set<std::string> &posDic, std::unordered_set<std::string> &negDic);
std::vector<std::string> readAllFileNames(std::string path = ".");
std::vector<std::string> tweetCleaner(std::string &twitterFile);
double posNegCounter(int &posCounter, int &negCounter, std::unordered_set<std::string> negDic, std::unordered_set<std::string> posDic, std::vector<std::string> tweetList);
void directionCalculator();