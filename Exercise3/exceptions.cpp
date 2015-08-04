#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cfloat>
#include <iomanip>
#include <vector>

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

std::tm stringToTime(std::string date)
{
    std::tm t;
    std::istringstream ss(date);
    ss >> std::get_time(&t, "%d.%m.%Y");
    
    if(t.tm_year < 105 || t.tm_year > 115)
        throw std::logic_error("Year should be between 2005 and 2015");
    
    return t;
}

struct FormatException
{
    int m_actLine;
    std::string m_actFields;
};

void parseLine(std::string line, int lineNum)
{
    const std::string fieldNames[3] = { "Date", "Temperature", "Rainfall" };
    auto splits = split(line, ';');
    std::string errorString;
    bool hasError = false;
    try
    {
        stringToTime(splits[0]);
    }
    catch(const std::logic_error le)
    {
        errorString += "->" + fieldNames[0] + "\n";
        hasError = true;
    }
    try
    {
        std::stof(splits[1]);
    }
    catch (const std::exception& ia) {
       errorString += "->" + fieldNames[1] + "\n";
       hasError = true;
    }
    try
    {
        std::stof(splits[2]);
    }
    catch (const std::exception& ia) {
       errorString += "->" + fieldNames[2] + "\n";
       hasError = true;
    }
    if(hasError)
    {
        FormatException ex;
        ex.m_actLine = lineNum;
        ex.m_actFields = errorString;
        throw ex;
    }
}

void writeOutFormatException(const FormatException & e)
{
    std::cerr << "Line: " << e.m_actLine << std::endl;
    std::cerr << "Error:\n" << e.m_actFields;
    try
    {
        std::ofstream outputFile("exception.log", std::ios::app);
        outputFile << "Line: " << e.m_actLine << std::endl;
        outputFile << e.m_actFields;
        outputFile.close();
    }
    catch(std::ios_base::failure e)
    {
        std::cerr << "Error while trying to write to execption.log";    
    }
}

void checkData(std::string path)
{
	int validLines = 0;
    int invalidLines = 0;
    std::ifstream file;
    
    int count = 1;
    try
    {
        file.open(path);
        file.exceptions(std::ifstream::eofbit | std::ifstream::failbit | std::ifstream::badbit);
        for(std::string line; getline(file, line);)
        {
            if(count == 1) {
                ++count;
                continue;
            }
            try
            {
                parseLine(line, count);
            }
            catch(const FormatException& fe)
            {
                writeOutFormatException(fe);
                ++invalidLines;
            }
            ++count;
        }
    }
    catch(std::ifstream::failure& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Code: " << e.code() << std::endl;   
    }
    validLines = count - invalidLines;
    std::cout << "valid lines: " << validLines << " - invalid lines: " << invalidLines << std::endl;
}

int main(int argc, char * argv[])
{
    if(argc != 2)
    {
        std::cout << "Invalid number of arguments - USAGE: exceptions [DATASET]" << std::endl;
        return -1;
    }
    
    checkData(argv[1]);

	return 0;
}
