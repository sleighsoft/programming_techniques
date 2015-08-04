#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <map>
#include <vector>
#include <algorithm>
#include <numeric>

float calculateDistanceBetween(float lat1, float long1, float lat2, float long2)
{
	float dlat1 = lat1*((float)M_PI / 180.0f);

	float dlong1 = long1*((float)M_PI / 180.0f);
	float dlat2 = lat2*((float)M_PI / 180.0f);
	float dlong2 = long2*((float)M_PI / 180.0f);

	float dLong = dlong1 - dlong2;
	float dLat = dlat1 - dlat2;

	float aHarv = pow(sin(dLat / 2.0f), 2.0f) + cos(dlat1)*cos(dlat2)*pow(sin(dLong / 2), 2);
	float cHarv = 2 * atan2(sqrt(aHarv), sqrt(1.0f - aHarv));
	const float earth = 6378.137f;
	return earth*cHarv;
}

struct AirportInfo
{
	std::string m_name; 
	std::string m_city;
	std::string m_country;
	float pos[2];

	std::vector<std::pair<int, int>> m_routes;
	std::vector<float> m_routeLengths;
	float m_averageRouteLength;
};

void importAirportData(char* path, std::map<int, AirportInfo>& airportInfo)
{
	std::cout << "Importing airport data.." << std::endl;
	std::ifstream file(path);
	std::string field, line;
	
	int currentLineNum = 0;

	while (std::getline(file, line)) 
	{
		std::istringstream linestream;
		linestream.str(line);
		int fieldNum = 0;
		int currentID = -1;
		std::string result;
		currentLineNum++;
		
		while (std::getline(linestream, field, ';'))
		{
			try
			{
				switch (fieldNum)
				{
				case 0: // id
					currentID = std::stoi(field);
					airportInfo.insert(std::make_pair(currentID, AirportInfo()));
					break;
				case 1: // name
					airportInfo[currentID].m_name = field;
					break;
				case 2: // city
					airportInfo[currentID].m_city = field;
					break;
				case 3: // country
					airportInfo[currentID].m_country = field;
					break;
				case 6: //latitude
					airportInfo[currentID].pos[0] = std::stof(field);
					break;
				case 7: // longitude
					airportInfo[currentID].pos[1] = std::stof(field);
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
	}
}

void importRoutesData(char* path, std::map<int, AirportInfo>& airportInfo)
{
	std::cout << "Importing routes data.." << std::endl;
	std::ifstream file(path);
	std::string field, line;

	int currentLineNum = 0;

	while (std::getline(file, line))
	{
		std::istringstream linestream;
		linestream.str(line);
		int fieldNum = 0;
		int sourceID = -1;
		int destID = -1;
		int stops = -1;
		std::string result;
		currentLineNum++;

		while (std::getline(linestream, field, ';'))
		{
			try
			{
				switch (fieldNum)
				{
				case 3: // source id
					sourceID = std::stoi(field);
					break;
				case 5: // dest id
					destID = std::stoi(field);
					break;
				case 7: // stops
					stops = std::stoi(field);
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
		if (sourceID != -1 && destID != -1 && stops != -1)
		{
			try
			{
				airportInfo[sourceID].m_routes.push_back(std::make_pair(destID, stops));
			}
			catch (const std::out_of_range&)
			{
				std::cout << "Source airport with ID " << sourceID << " not present in airport dataset!" << std::endl;
			}
		}
	}
}

bool isNotEqualOne(const std::pair<int, int> &val)
{
	if(val.second == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void removeNonDirectFlights(std::map<int, AirportInfo>& airportInfo)
{
	std::cout << "Remove non-direct flights (i.e., at least one stop)" << std::endl;
	for(auto &item : airportInfo)
	{
		std::vector<std::pair<int, int>> m_routes = item.second.m_routes;		
		auto toBeRemoved = std::remove_if(m_routes.begin(), m_routes.end(), isNotEqualOne);
		m_routes.erase(toBeRemoved, m_routes.end());
		item.second.m_routes = m_routes;
	}
}

struct Distance
{
	AirportInfo &airportInfo;
	std::map<int, AirportInfo> &airportInfoMap;
	Distance(AirportInfo &airportInfo, std::map<int, AirportInfo>& airportInfoMap) : airportInfo(airportInfo),  airportInfoMap(airportInfoMap){}
	
	float operator()(std::pair<int, int> route)
	{
//		std::cout << "Distance between " << airportInfo.m_name << " and " << airportInfoMap[std::get<0>(route)].m_name << " is " << calculateDistanceBetween(airportInfo.pos[0], airportInfo.pos[1], airportInfoMap[std::get<0>(route)].pos[0], airportInfoMap[std::get<0>(route)].pos[1]) << std::endl;
		return calculateDistanceBetween(airportInfo.pos[0], airportInfo.pos[1], airportInfoMap[std::get<0>(route)].pos[0], airportInfoMap[std::get<0>(route)].pos[1]);
	}
};

void calculateDistancePerRoute(std::map<int, AirportInfo>& airportInfo)
{
	std::cout << "Calculate distance for each route" << std::endl;
	for(auto &item : airportInfo)
	{
		Distance distance(item.second, airportInfo);
		item.second.m_routeLengths.resize(item.second.m_routes.size());
		std::transform(item.second.m_routes.begin(), item.second.m_routes.end(), item.second.m_routeLengths.begin(), distance);
	}
}

void calculateAverageRouteDistances(std::map<int, AirportInfo>& airportInfo)
{
	std::cout << "Calculate average distance for each source airport" << std::endl;
	for(auto &item : airportInfo)
	{
		item.second.m_averageRouteLength = std::accumulate(item.second.m_routeLengths.begin(), item.second.m_routeLengths.end(), 0.0f) / item.second.m_routeLengths.size();
//		std::cout << "Average distance for airport " << item.second.m_name << " is " << item.second.m_averageRouteLength << std::endl;
	}
}

void printResults(std::map<int, AirportInfo>& airportInfo)
{
	for (auto & airport : airportInfo)
	{
		if (airport.second.m_routeLengths.size())
			std::cout << airport.second.m_name << " (" << airport.second.m_city << ", " << airport.second.m_country << "): " << airport.second.m_averageRouteLength << "km (" << airport.second.m_routeLengths.size() << " direct outgoing routes)" << std::endl;
	}
}

int main(int argc, char * argv[])
{
	if(argc != 3)
	{
		std::cout << "not enough arguments - USAGE: mapreduce [AIRPORT DATASET] [AIRLINE DATASET]" << std::endl;
		return -1;	// invalid number of parameters
	}

	static std::map<int, AirportInfo> airportInfo;

	std::cout << "Given path to airports.dat: " << argv[1] << std::endl;
	std::cout << "Given path to routes.dat: " << argv[2] << std::endl;

	importAirportData(argv[1], airportInfo);
	importRoutesData(argv[2], airportInfo);

	removeNonDirectFlights(airportInfo);
	calculateDistancePerRoute(airportInfo);
	calculateAverageRouteDistances(airportInfo);
	printResults(airportInfo);

	return 0;
}
