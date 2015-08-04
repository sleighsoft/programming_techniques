#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <map>
#include <vector>
#include <algorithm>
#include <numeric>
#include <chrono>

struct Route
{
	int airlineId;
	int sourceId;
	int destinationId;
};

bool operator<(const Route& r1, const Route& r2) {
	return r1.destinationId < r2.destinationId;
}

void importRoutesData(char* path, std::vector<Route>& routes)
{
	std::cout << "Importing routes data.." << std::endl;
	std::ifstream file(path);
	std::string field, line;
	
	while (std::getline(file, line))
	{
		std::istringstream linestream;
		linestream.str(line);
		int fieldNum = 0;
		Route route;
		route.airlineId = route.sourceId = route.destinationId = -1;

		while (std::getline(linestream, field, ';'))
		{
			try
			{
				switch (fieldNum)
				{
					case 1: // airline id
						route.airlineId = std::stoi(field);
						break;
					case 3: // source id
						route.sourceId = std::stoi(field);
						break;
					case 5: // dest id
						route.destinationId = std::stoi(field);
						break;
					default:
						break;
				}
			}
			catch (const std::invalid_argument&)
			{
				//std::cout << "Couldn't convert field " << currentLineNum << " correctly (invalid argument)!" << std::endl;
				//std::cout << field << std::endl;
			}
			catch (const std::out_of_range&)
			{
				//std::cout << "Couldn't convert field " << currentLineNum << " correctly (out of range)!" << std::endl;
				//std::cout << field << std::endl;
			}

			fieldNum++;
		}

		if (route.airlineId > -1 && route.sourceId > -1 && route.destinationId > -1)
			routes.push_back(route);
	}
}

int linearSearch(int destID, std::vector<Route>& routes, long long& numLookups)
{
	int numRoutes = 0;

	for(auto& route : routes)
	{
		if(route.destinationId == destID)
		{
			++numRoutes;
		}
		++numLookups;
	}

	return numRoutes;
}

std::pair<long long, long long> evaluateLinearSearch(std::vector<Route>& routes)
{
	long long numLookups = 0;
	long long duration = 0;

	auto start = std::chrono::system_clock::now();
	for(int i = 0; i <= 9541; ++i)
	{
		linearSearch(i, routes, numLookups);
	}
	auto end = std::chrono::system_clock::now();
	duration = (end - start).count();
	return std::make_pair(numLookups, duration);
}

int binarySearch(int destID, std::vector<Route>& routes, long long& numLookups)
{
	int start = 0;
	int end = routes.size();
	int numRoutes = 0;
	int mid = 0;
	while(end > start)
	{
		mid = (end + start) / 2;

		auto currentDestID = routes.at(mid).destinationId;
		if(currentDestID == destID)
		{
			// Search around it now
			int temp = mid - 1;
			++numRoutes;
			while(temp > 0 && routes.at(temp).destinationId == destID)
			{
				++numLookups;
				++numRoutes;
				--temp;
			}
			temp = mid + 1;
			while(temp < routes.size() && routes.at(temp).destinationId == destID)
			{
				++numLookups;
				++numRoutes;
				++temp;
			}
			break;
		}
		else if(currentDestID < destID)
		{
			start = mid + 1;
		}
		else
		{
			end = mid - 1;
		}
		++numLookups;
	}
	return numRoutes;
}

std::pair<long long, long long> evaluateBinarySearch(std::vector<Route>& routes)
{
	long long numLookups = 0;
	long long duration = 0;
	std::sort(routes.begin(), routes.end());
	auto start = std::chrono::system_clock::now();
	for(int i = 1; i <= 9541; ++i)
	{
		binarySearch(i, routes, numLookups);
	}
	auto end = std::chrono::system_clock::now();
	duration = (end - start).count();
	return std::make_pair(numLookups, duration);
}

int main(int argc, char * argv[])
{
	if(argc != 2)
	{
		std::cout << "not enough arguments - USAGE: sort [ROUTE DATASET]" << std::endl;
		return -1;	// invalid number of parameters
	}

	static std::vector<Route> routes;

	std::cout << "Given path to routes.csv: " << argv[1] << std::endl;

	importRoutesData(argv[1], routes);

	auto result = evaluateLinearSearch(routes);
	std::cout << result.first << " - " << result.second << std::endl;
	result = evaluateBinarySearch(routes);	
	std::cout << result.first << " - " << result.second << std::endl;

	return 0;
}
