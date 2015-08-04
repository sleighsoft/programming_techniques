#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include <cstdlib>
#include <string>
#include <cassert>
#include <algorithm>
#include <ctime>
#include <cmath>

const int NUMBER_TABLES = 10; // max number of tables in the restaurant
const int AVG_SEATS_PER_TABLE = 6; // average number of seats per table

struct Order
{
    int table;
    int coffee;
    int coke;
    int burger;
    int salad;
    int id;
};

bool operator<(const Order& o1, const Order& o2) {
    return o1.table < o2.table;
}

int orderSerialNumber = 0; // all orders get a unique number (could be the time as well)

std::vector<Order> takeOrders(char* path)
{
	std::vector<Order> orders;
    
    std::ifstream file(path);
    std::string field, line;
    
    int currentLineNum = 0;
    
    Order order;
    
    while (std::getline(file, line))
    {
        std::istringstream linestream;
        linestream.str(line);
        int fieldNum = 0;
        currentLineNum++;
        
        while (std::getline(linestream, field, ';'))
        {
            try
            {
                switch (fieldNum)
                {
                    case 0:
                        order.table = std::stoi(field);
                        break;
                    case 1:
                        order.coffee = std::stoi(field);
                        break;
                    case 2:
                        order.coke = std::stoi(field);
                        break;
                    case 3:
                        order.burger = std::stoi(field);
                        break;
                    case 4:
                        order.salad = std::stoi(field);
                        break;
                }
            }
            catch (const std::invalid_argument&)
            {
                std::cout << "Couldn't convert entry " << currentLineNum << " correctly (invalid argument)!" << std::endl;
                std::cout << field << std::endl;
            }
            catch (const std::out_of_range&)
            {
                std::cout << "Couldn't convert entry " << currentLineNum << " correctly (out of range)!" << std::endl;
                std::cout << field << std::endl;
            }
            
            fieldNum++;
        }
        order.id = ++orderSerialNumber;
        orders.push_back(order);
    }
    return orders;
}

void processOrders(std::vector<Order>& currentOrders, std::vector<Order>& incomingOrders)
{
    for(auto &o : incomingOrders)
    {
        currentOrders.push_back(o);
    }
    std::sort(currentOrders.begin(), currentOrders.end());
}

void mergeOrders(std::vector<Order>& currentOrders)
{
    std::vector<Order> mergedOrders;
    auto itBegin = currentOrders.begin();
    while(itBegin < currentOrders.end())
    {
        int id = itBegin->id;
        int table = itBegin->table;
        int coffee = itBegin->coffee;
        int coke = itBegin->coke;
        int burger = itBegin->burger;
        int salad = itBegin->salad;
        ++itBegin;
        int found = 1;
        while(itBegin != currentOrders.end() && table == itBegin->table)
        {
            coffee += itBegin->coffee;
            coke += itBegin->coke;
            burger += itBegin->burger;
            salad += itBegin->salad;
            ++itBegin;
            found++;
        }
        Order newOrder = {
            table,
            coffee,
            coke,
            burger,
            salad,
            id
        };
        mergedOrders.push_back(newOrder);
    }
    currentOrders = mergedOrders;
}

int pay(int table, std::vector<Order>& currentOrders)
{
    int toPay = 0;
    for(auto &o : currentOrders)
    {
        if(o.table == table)
        {
            toPay += ((o.coffee + o.coke) - floor((o.coffee + o.coke) / 4)) * 2;
            toPay += (o.burger - floor(o.burger / 3)) * 5;
            toPay += o.salad * 4;
            break;
        }
    }
    return toPay;
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cout << "not enough arguments - USAGE: burger <SAMPLE DATASET>" << std::endl;
        return -1;	// invalid number of parameters
    }
    
    std::vector<Order> currOrders;
    
    std::cout << "Take new orders.." << std::endl;
	auto newOrders = takeOrders(argv[1]);
    std::cout << "Process orders.." << std::endl;
    processOrders(currOrders, newOrders);
    
    std::cout << "Merge orders.." << std::endl;
    mergeOrders(currOrders);
    
    std::cout << "Payment: " << std::endl;
    const int expectedResults[NUMBER_TABLES] = { 180, 198, 216, 166, 0, 293, 265, 186, 245, 204 };
    int payment = 0;
    for(int tableIdx=0; tableIdx < NUMBER_TABLES; tableIdx++)
    {
        payment = pay(tableIdx, currOrders);
        assert(payment >= 0);
        if(payment != expectedResults[tableIdx])
        {
            std::cout << "WRONG RESULT - Expected: " << expectedResults[tableIdx] << " - Calculated: " << payment << std::endl;
        }
        else
        {
            if(payment == 0)
            {
                std::cout << "No open bill for table " << tableIdx << std::endl;
            }
            else
            {
                std::cout << "Table " << tableIdx << " pays " << payment << " Euro" << std::endl;
            }
        }
    }
    
    return 0;
}
