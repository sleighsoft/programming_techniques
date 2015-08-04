#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <cassert>

// function template for printing contents of containers to std::cout
template<class T>
void printContainer(T& container)
{
	std::cout << "{";
	for (auto element : container)
		std::cout << element << " ";
	std::cout << "}" << std::endl;
}

template<class T>
void merge(T leftIt, T midIt, T end)
{
	assert(leftIt <= midIt && midIt <= end);

	auto itRight = midIt;
	auto itLeft = leftIt;

	std::vector<typename T::value_type> mergedValues;

	while(itLeft != midIt && itRight != end)
	{
		if(*itLeft <= *itRight)
		{
			mergedValues.push_back(*itLeft);
			++itLeft;
		}
		else
		{
			mergedValues.push_back(*itRight);
			++itRight;
		}
	}
	mergedValues.insert(mergedValues.end(), itLeft, midIt);
	mergedValues.insert(mergedValues.end(), itRight, end);
	
	for(auto &v : mergedValues)
	{
		*leftIt = v;
		++leftIt;
	}
}

template<class T>
void mergeSort(T leftIt, T rightIt)
{
	assert(leftIt < rightIt);
	auto distance = std::distance(leftIt, rightIt);
	if(distance < 2)
	{
		return;
	}
	T mid = std::next(leftIt, distance / 2);
	mergeSort(leftIt, mid);
	mergeSort(mid, rightIt);
	merge(leftIt, mid, rightIt);
}

int main(int argc, char** argv)
{
	// define sample data
	std::vector<int> sampleDataInt = { 10, 1, 12, 33, 24, 5, 6, -7, -2, 19 };
	std::vector<std::string> sampleDataString = { "Die", "eines", "ist", "Gebrauch", "der", "Sprache", "in", "sein", "Wortes", "Bedeutung" };

	// test for integer vector
	printContainer(sampleDataInt);
	mergeSort(sampleDataInt.begin(), sampleDataInt.end());
	printContainer(sampleDataInt);

	// test for string vector
	printContainer(sampleDataString);
	mergeSort(sampleDataString.begin(), sampleDataString.end());
	printContainer(sampleDataString);

	return 0;
}
