#include "stdafx.h"
#include "twitterReader.h"


using namespace boost;


std::string TWEET_LOCATION = "C:\\Users\\Tyler Catlin\\Documents\\College\\IS\\tweets";

int main()
{

	std::vector<std::string> fileList;
	std::vector<std::string> tweetList;

	std::unordered_set<std::string> posDic;
	std::unordered_set<std::string> negDic;

	std::string posWordFile = "C:\\Users\\Tyler Catlin\\Documents\\College\\IS\\wordlists\\positives.txt";
	std::string negWordFile = "C:\\Users\\Tyler Catlin\\Documents\\College\\IS\\wordlists\\negatives.txt";
	std::ifstream posWordFileStream(posWordFile);
	std::ifstream negWordFileStream(negWordFile);
	fileList = readAllFileNames(TWEET_LOCATION);
	std::string currentFile;


	populateDictionaries(negWordFileStream, posWordFileStream, posDic, negDic);					//retrieve dictionaries, put them in unordered sets

	for (int i = 0; i < fileList.size(); i++) {		
		currentFile = fileList.at(i);
		tweetList = tweetCleaner(currentFile);													//go through all files in the tweet directory (set by TWEET_LOCATION)
		twitterDataReader(tweetList, posDic, negDic);
	}
	//directionCalculator();       //rudimentary directionality calculator
	return 0;
	
}



void populateDictionaries(std::ifstream &negWordFileStream, std::ifstream &posWordFileStream, std::unordered_set<std::string> &posDic, std::unordered_set<std::string> &negDic) {


	std::string posLine;
	std::string negLine;

	std::string posWordFile = "C:\\Users\\Tyler Catlin\\Documents\\College\\IS\\wordlists\\positives.txt";
	std::string negWordFile = "C:\\Users\\Tyler Catlin\\Documents\\College\\IS\\wordlists\\negatives.txt";

	while (std::getline(negWordFileStream, negLine)) {
		negDic.insert(negLine);                              //populates the negative dictionary
	}
	while (std::getline(posWordFileStream, posLine)) {
		posDic.insert(posLine);                             //populates the positive dictionary
	}
	std::cout << "size of pos/neg dictionaries: " << posDic.size() << "/" << negDic.size() << std::endl;   //just prints the size of each dictionary
}



void directionCalculator() 
{
	int day4 = 0;
	int day3 = 0;
	int day2 = 0;
	int day1 = 0;
	int today = 0;
	std::cout << "please enter the last 4 days' and today's positive percentage."<<std::endl;
	std::cout << "4 days ago:" << std::endl;
		std::cin >> day4;
	std::cout << "3 days ago:" << std::endl;
		std::cin >>day3;
	std::cout << "2 days ago:" << std::endl;													//asks for input for all the positive percentages
		std::cin >>day2;
	std::cout << "1 day ago:" << std::endl;
		std::cin >>day1;
	std::cout << "today:" << std::endl;
		std::cin >>today;

		int total = day4*.2 + day3*.4 + day2*.6 + day1*.75 + today;
		total = total / 3;                                 //uncomment for windowing
		if (total > 64) {
			std::cout << "BUY BUY BUY" << std::endl;
		}
		else {
			std::cout << "DO NOT BUY!" << std::endl;
		}
		/*
		if (today > 64) {
			std::cout << "BUY BUY BUY" << std::endl;
		}
		else {											   //uncomment for single-day data predictions
			std::cout << "DO NOT BUY!" << std::endl;

			*/
}



void twitterDataReader(std::vector<std::string> tweetList, std::unordered_set<std::string> posDic, std::unordered_set<std::string> negDic)
{

	int posCounter = 0;
	int negCounter = 0;
	double positiveFrac;
	std::string currLine;

		positiveFrac = posNegCounter(posCounter, negCounter, negDic, posDic, tweetList);				//get the positive percentage of the tweets
		std::ofstream out;
		out.open("C:\\Users\\Tyler Catlin\\Documents\\College\\IS\\positiveFractions.txt", std::ios::app);  
		out << positiveFrac << std::endl;																	//write the positive percentages to a file

}

std::vector<std::string> readAllFileNames(std::string path)
{

	DIR* directory;
	dirent* pDirectory;
	std::vector<std::string> fileList;
	directory = opendir(path.c_str());

	while (pDirectory = readdir(directory))
	{
		fileList.push_back(pDirectory->d_name);
	}
	return fileList;
}



std::vector<std::string> tweetCleaner(std::string& twitterFile)
{
	std::regex rgx("\\\n\\\n *.+ \\\n");
	//    [A-Z][a-z][a-z] \\d+\\\n\\\n *.+ \\\n
	std::smatch match;
	std::cout << twitterFile;
	std::vector<std::string> tweetHolder;
	std::ifstream twitterFiler;

	twitterFiler.open(TWEET_LOCATION + "\\" + twitterFile, std::ifstream::in);

	std::string tweetFile((std::istreambuf_iterator<char>(twitterFiler)), std::istreambuf_iterator<char>());

	int counter = 0;
		while (std::regex_search(tweetFile, match, rgx))
		{
			for (unsigned i = 0; i < match.size(); ++i) {
				tweetHolder.push_back(match[i]);
				tweetFile = match.suffix().str();
			}
		}
	return tweetHolder;
}


double posNegCounter(int &posCounter, int &negCounter, std::unordered_set<std::string> negDic, std::unordered_set<std::string> posDic, std::vector<std::string> tweetList)
{
	std::string currLine;
	double posFractionTweet;
	double posFractionTotal;
	int posCounterTotal = 0;
	int negCounterTotal = 0;
	std::string secondaryStringContainer = "";

	for (int i = 0; i < tweetList.size(); i++) {
		currLine = tweetList[i];
		std::transform(currLine.begin(), currLine.end(), currLine.begin(), ::tolower);
		char_separator<char> sep("()?!#-;, ");                        //the characters than separate different tokens
		tokenizer<char_separator<char>> tokens(currLine, sep);

		for (auto& t : tokens) {

		    std::string tHolder = t;
			if (posDic.find(secondaryStringContainer + " " + t) != posDic.end()) {
				posCounter++;
				//std::cout << t << std::endl;
				//std::cout << secondaryStringContainer + " " + t << std::endl;					//increments counter if bigram is found
				secondaryStringContainer = t;
			}
			else if (posDic.find(t) != posDic.end()) {
				posCounter++;
				//std::cout << t << std::endl;													//increments counter if unigram is found
				secondaryStringContainer = t;
			}
			std::cout << secondaryStringContainer + " " + tHolder << std::endl;
			if (negDic.find(secondaryStringContainer + " " + tHolder) != negDic.end()) {
				negCounter++;
				//std::cout << t << std::endl;
				//std::cout << secondaryStringContainer + " " + tHolder << std::endl;				//increments counter if bigram is found
				secondaryStringContainer = tHolder;
			}
			else if (negDic.find(tHolder) != negDic.end()) {
				negCounter++;
				secondaryStringContainer = tHolder;												//increments counter if unigram is found
			}
			else(secondaryStringContainer = tHolder);


			

		}
		std::cout << "Positives: " << posCounter << "\nNegatives: " << negCounter << std::endl;
		posFractionTweet = double(posCounter) / (double(posCounter) + double(negCounter));
		posCounter = 0;
		negCounter = 0;
		if (posFractionTweet > .5) {
			posCounterTotal++;
		}
		else if (posFractionTweet <= .5) {
			negCounterTotal++;
		}
		
	}
	std::cout << "Positive tweets: " << posCounterTotal << "\nNegative tweets: " << negCounterTotal << std::endl;
	posFractionTotal = double(posCounterTotal) / (double(posCounterTotal) + double(negCounterTotal));

	std::cout << "Positive Percentage: " << posFractionTotal;


	/*
	if (posCounter > 0 || negCounter > 0) {
		std::cin >> posFraction; //uncomment this block to pause and see output
	}
	*/
	return posFractionTotal;
}
