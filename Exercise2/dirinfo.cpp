#include <iostream>
#include <fstream>
#include <dirent.h>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <set>

#if defined _WIN32
#include <direct.h>
#elif defined __GNUC__
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

inline char separator()
{
#ifdef _WIN32
    return '\\';
#else
    return '/';
#endif
}

struct DirectoryAnalysis
{
	std::set<std::string> keys;
	std::multimap<std::string, std::string> extensionFiles;
	std::map<std::string, unsigned long long> diskRequirement;
	std::set<std::string> flatSet;
};

struct CommandLineParameter
{
	CommandLineParameter(int argc, char* argv[])
		: flat(false)
		, fnum(false)
		, fsize(false)
		, files(false)
		, outputPathAndFile("output.csv")
		
	{
		int count = 1;
		bool foundScannedDir = false;
		for (int i = 1; i < argc; ++i)
		{
			if (!strcmp(argv[i], "-fsize"))
			{
				fsize = true;
			}
			else if (!strcmp(argv[i], "-fnum"))
			{
				fnum = true;
			}
			else if (!strcmp(argv[i], "-files"))
			{
				files = true;
			}
			else if (!strcmp(argv[i], "-flat"))
			{
				if((i + 1) < argc)
				{
					flat = true;
					flatDirectory = argv[i + 1];
					i++;	
				}
			}
			else if (!strcmp(argv[i], "-o"))
			{
				if((i + 1) < argc)
				{
					outputPathAndFile = argv[i + 1];
					i++;	
				}			
			}
			else
			{
				if(!foundScannedDir)
				{
					scannedDirectory = argv[i];
					foundScannedDir = true;
				}
			}
		}
		if (scannedDirectory.empty())
		{
			std::cerr << "No scan directory argument passed!" << std::endl;
		}
	}

	bool flat;
	bool fnum;
	bool fsize;
	bool files;
	std::string flatDirectory;
	std::string scannedDirectory;
	std::string outputPathAndFile;
};

void removeDirectory(std::string dir)
{
#if defined _WIN32
    _rmdir(dir.data());
#elif defined __GNUC__
    rmdir(dir.data());
#endif
}

void createDirectory(std::string dir)
{
#if defined _WIN32
    _mkdir(dir.data());
#elif defined __GNUC__
    mkdir(dir.data(), 0777);
#endif
}

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

void traverseDirectory(std::string path, DirectoryAnalysis& store)
{
	DIR *dir;
	struct dirent *ent;

	dir = opendir(path.c_str());
	
	if (dir != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			std::string name = ent->d_name;
			if (ent->d_type == DT_DIR)
			{
				std::cout << "Dir: " << name << std::endl;
				if(name.compare("..") != 0 && name.compare(".") != 0)
				{
					traverseDirectory(path + name + separator(), store);
				}
			}
			else
			{
				std::cout << "File: " << name << std::endl;
				std::string extension = split(name, '.').back();
				store.keys.insert(extension);
				store.extensionFiles.insert(std::pair<std::string, std::string>(extension, (path + name)));
				store.flatSet.insert(path + name);
				struct stat fileinfo;
				std::string fullpath = path + name;
				std::cout << "Fullpath: " << fullpath << std::endl;
				if (!stat(fullpath.c_str(), &fileinfo))
				{
					std::cout << (unsigned int)fileinfo.st_size << "bytes" << std::endl;
					store.diskRequirement[extension] += (unsigned int)fileinfo.st_size;
				}
				else
				{
					std::cout << "(stat() failed for this file)\n" << std::endl;
				}
			}
		}
		closedir(dir);
	}
	else
	{
		std::cout << "Failed to read directory" << std::endl;
	}
}

int main(int argc, char * argv[])
{
	if (argc < 2)
	{
		std::cout << "not enough arguments - USAGE: dirinfo [DIR] -o [OUTPUT.csv] COLUMNS" << std::endl;
		std::cout << "possible COLUMNS are:" << std::endl;
		std::cout << "-fsize\t\t\tSummarized file size for files with same extension will be exported" << std::endl;
        std::cout << "-fnum\t\t\tSummarized number of files with same extension will be exported" << std::endl;
		std::cout << "-files\t\t\tRelative paths of files with same extension will be exported" << std::endl;
		std::cout << "-flat <Directory>\t\t\tFlattens the input to the directory" << std::endl;

		return -1;
	}
	CommandLineParameter cmd(argc, argv);
	if (cmd.scannedDirectory.empty())
	{
		return 1;
	}

	DirectoryAnalysis store;
    createDirectory("foo");
	traverseDirectory(cmd.scannedDirectory, store);
	
	std::ofstream outputFile;
	outputFile.open(cmd.outputPathAndFile);

	for (auto &key : store.keys)
	{
		outputFile << key;
		if(cmd.fsize)
		{
			outputFile << "," << store.diskRequirement[key];
		}
		if(cmd.fnum)
		{
			outputFile << "," << store.extensionFiles.count(key);
		}
		if(cmd.files)
		{
    		auto ret = store.extensionFiles.equal_range(key);
			for (auto it = ret.first; it != ret.second; ++it)
			{
				outputFile << "," << (*it).second;
			}
		}
		outputFile << std::endl;
	}
	
	outputFile.close();
	
	if(cmd.flat) {
		std::string prefix1 = "..";
		std::string prefix2 = ".";
		for(auto &key : store.flatSet)
		{
			std::string path = key;
			std::replace(path.begin(), path.end(), separator(), '_');
			if(path.substr(0, prefix1.size()) == prefix1) {
				path = path.substr(prefix1.size());
			}
			if(path.substr(0, prefix2.size()) == prefix2) {
				path = path.substr(prefix2.size());
			}
			path = cmd.flatDirectory + path;
			std::ifstream  src(key, std::ios::binary);
			std::ofstream  dst(path,  std::ios::binary);
			dst << src.rdbuf();
		}
	}
	
	return 0;
}