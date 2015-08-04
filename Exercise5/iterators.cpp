#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <set>

template<class T>
void printContainer(T& container)
{
    std::cout << "{"; 
    for(auto element : container)
        std::cout << element << " ";
    std::cout << "}" << std::endl; 
}


template<class T>
void front_back_pairing(T& inContainer, T& outContainer)
{
    auto forwards = inContainer.begin();
    auto backwards = inContainer.rbegin();
    int half = inContainer.size()/2;
    int count = 0;
    while(forwards != backwards.base() && count < half)
    {
        outContainer.insert(outContainer.end(), *forwards);
        outContainer.insert(outContainer.end(), *backwards);
        ++forwards;
        ++backwards;
        ++count;
    }
    if(inContainer.size() % 2 != 0)
    {
        outContainer.insert(outContainer.end(), *forwards);
    }
}

template<class T>
void remove_duplicates(T& container)
{
    auto elem = container.at(0);
    std::set<decltype(elem)> foundSet;
    
    auto iter = container.begin();
    while(iter != container.end())
    {
        if(foundSet.find(*iter) != foundSet.end())
            iter = container.erase(iter);
        else
        {
            foundSet.insert(*iter);
            iter++;
        }
    }
}

template<class T>
void insert_differences(T& container)
{
    auto containerCopy = container;
    containerCopy.clear();
    for(auto it = container.begin(); it != container.end(); ++it)
    {
        if(it == container.begin())
        {
            containerCopy.push_back(*(container.end() - 1) - *it);
        }
        else
        {
            containerCopy.push_back(*(it - 1) - *it);
        }
        containerCopy.push_back(*it);
        if(it == container.end() - 1)
        {
            containerCopy.push_back(*container.begin() - *it);
        }
        else
        {
            containerCopy.push_back(*(it + 1) - *it);
        }
    }
    container = containerCopy;
}

void testFrontBackPairingFunctionality()
{
    std::vector<int> sampleDataInt = { 10,11,12,13,14,15,16,17,18,19 };
    std::list<std::string> sampleDataString = { "Die", "eines", "ist", "Gebrauch", "der", "Sprache", "in", "sein", "Wortes", "Bedeutung" };
    
    std::vector<int> helpIntVector;
    front_back_pairing(sampleDataInt, helpIntVector);
    printContainer(sampleDataInt);
    printContainer(helpIntVector);
    
    std::list<std::string> helpStringList;
    front_back_pairing(sampleDataString, helpStringList);
    printContainer(sampleDataString);
    printContainer(helpStringList);
}

void testRemoveDuplicateFunctionality()
{
    std::vector<int> sampleDataInt = { 10,11,10,13,14,15,15,15,18,19,12,11,17 };
    std::vector<std::string> sampleDataString = { "Die", "___", "eines", "ist", "Gebrauch", "der", "Sprache", "in", "sein", "Wortes", "Bedeutung", "Die", "ist", "blabla" };
    
    printContainer(sampleDataInt);
    //remove_duplicates(sampleDataInt, std::less<int>() );
    remove_duplicates(sampleDataInt);
    printContainer(sampleDataInt);
    
    printContainer(sampleDataString);
    remove_duplicates(sampleDataString);
    printContainer(sampleDataString);
}

void testAddDifferenceFunctionality()
{
    std::vector<int> sampleDataInt = { 10,11,14,16,1,18 };
    
    printContainer(sampleDataInt);
    insert_differences(sampleDataInt);
    printContainer(sampleDataInt);
}

int main(int argc, char** argv)
{
    testFrontBackPairingFunctionality();
    testRemoveDuplicateFunctionality();
    testAddDifferenceFunctionality();

    return 0;
}
