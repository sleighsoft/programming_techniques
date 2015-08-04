#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>
#include <vector>
#include <algorithm>

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

bool isQuote(char c)
{
	switch(c)
	{
		case '"': return true;
		default: return false;
	}
}

std::string &removeQuotes(std::string &s)
{
	s.erase(std::remove_if(s.begin(), s.end(), &isQuote), s.end());
	return s;
}

bool isValueCorrect(const std::string &teststring, const int &column)
{
	std::regex regExp;
	
	switch (column)
	{
		case 5:
			regExp = "^(?:[A-Z]{4})?$"; // Empty or four captial letters
			break;
		case 8:
			regExp = "^(?:\\d|[1-9]\\d{1,3}|[1-4]\\d{1,4}|[1-5](?:1000|0\\d{1,3}))$"; //Match 0-51000 feet
			break;
		case 10:
			regExp = "^(?:E|A|S|O|Z|N|U)$";
			break;
		default:
			regExp = ".*";
			break;
	}
	
	return std::regex_match(teststring, regExp);
}

void readTokensAndLines(char* path)
{
	std::ifstream file(path);
	std::string parsed, line;
	
	std::ofstream outputFile;
	outputFile.open("fileio.log");
	
	while (std::getline(file, line)) {
		std::istringstream linestream;
		linestream.str(line);
		
		std::vector<std::string> splits = split(line, ',');
		std::cout << splits.at(1) << " - " << splits.at(splits.size()-1) << "\n";
		
		std::string icao = removeQuotes(splits.at(5));
		std::string altitude = splits.at(8);
		std::string dst = removeQuotes(splits.at(10));
		
		bool icao_chck = isValueCorrect(icao, 5);
		bool altitude_chck = isValueCorrect(altitude, 8);
		bool dst_chck = isValueCorrect(dst, 10);
		
		if(!icao_chck || !altitude_chck || !dst_chck)
		{
			outputFile << line << " <- Error(0 value) for icao: " << icao_chck << " altitude: " << altitude_chck << " dst: " << dst_chck << "\n";			
		}
	}
	outputFile.close();
}

int main(int argc, char * argv[])
{
	if(argc != 2)
	{
		std::cout << "not enough arguments - USAGE: fileio [DATASET]" << std::endl;
		return -1;	// invalid number of parameters
	}
	
	std::cout << "Given path to airports.dat: " << argv[1] << std::endl;
	readTokensAndLines(argv[1]);

	return 0;
}
