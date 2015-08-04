#include <climits>
#include <iostream>
#include <vector>
#include <algorithm>

int minTimeDogTraining(int searchValueX, int searchValueY, const std::vector<int> &boxes)
{
	int count = 1;
	int posX = -1;
	int posY = -1;
	for(auto &v : boxes)
	{
		if(v == searchValueX)
		{
			posX = count;
		}
		else if (v == searchValueY)
		{
			posY = count;
		}
		count++;
	}
	if((boxes.size() - posX) < posX)
	{	
		if((boxes.size() % 2) == 1 && (boxes.size() / 2 + 1) == posX){}
		else
		{
			posX = boxes.size() - posX + 1;
		}
	}
	if((boxes.size() - posY) < posY)
	{
		if((boxes.size() % 2) == 1 && (boxes.size() / 2 + 1) == posY){}
		else
		{
			posY = boxes.size() - posY + 1;
		}
	}
	int min = 0;
	if(posX == -1 || posY == -1)
	{
		min = -1;
	}
	else
	{
		min = std::max(posX, posY);
	}
	if(min > boxes.size())
	{
		min = -1;
	}
	return min;
}

int main(int argc, char * argv[])
{
	if (argc < 3)
	{
		std::cout << "Missing parameters. Usage: <searchValue1> <searchValue2> [box1] [box2] [...]" << std::endl;
		return 0;
	}

	const int searchValueX = std::atoi(argv[1]);
	const int searchValueY = std::atoi(argv[2]);
	if(searchValueX < 1 || searchValueY < 1)
	{
		std::cerr << "Search values must be greater than zero!" << std::endl;
	}
	
	if (searchValueX == searchValueY)
	{
		std::cerr << "Search values must be different!" << std::endl;
		return 1;
	}

	std::vector<int> boxes;
	for (int i = 3; i < argc; i++)
	{
		int value = std::atoi(argv[i]);
		if (value < 1)
		{
			std::cerr << "Box value must be greater than zero!" << std::endl;
			return 2;
		}

		boxes.push_back(std::atoi(argv[i]));
	}

	int minTime = minTimeDogTraining(searchValueX, searchValueY, boxes);
	if (minTime == -1)
	{
		std::cout << "There exists no two boxes with the given search values!" << std::endl;
	}
	else
	{
		std::cout << "The coach will have to wait at least " << minTime << " seconds!" << std::endl;
	}
	
	return 0;
}