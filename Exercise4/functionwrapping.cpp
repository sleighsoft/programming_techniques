#include <functional>
#include <iostream>
#include <cmath>
#include <cstring>
#include <vector>

struct CommandLineParameter
{
	CommandLineParameter(int argc, char* argv[])
		: m_fibonacci(false)
		, m_centeredTriangular(false)
		, m_powSum(false)
	{
		for (int i = 1; i < argc; i += 1)
		{
			if (!strcmp(argv[i], "-fib"))
			{
				m_fibonacci = true;
			}
			else if (!strcmp(argv[i], "-ct"))
			{
				m_centeredTriangular = true;
			}
			else if (!strcmp(argv[i], "-pow"))
			{
				m_powSum = true;
			}
		}
	}

	bool m_fibonacci;
	bool m_centeredTriangular;
	bool m_powSum;
};

int fibonacci(int number)
{
	if(number == 0 || number == 1)
	{
		return number;
	}
	int f1 = 0;
	int f2 = 1;
	int ret = 0;
	for (int i = 1; i < number; i++) {
		ret = f1 + f2; 
		f1 = f2; 	
		f2 = ret;
	}
	return ret;
}

int centeredTriangular(int number)
{
	int result = (3 * pow(number, 2) + 3 * number + 2) / 2;
	return result;
}

int powSum(int number, int exp)
{
	int sum = 0;
	for(int i = 0; i <= number; ++i)
	{
		sum += pow(i, exp);
	}
	return sum;
}

void applyFuntions(std::vector<std::function<int(int)>> & functions, int number)
{
	for(auto &func : functions)
	{
		std::cout << func(number) << std::endl;
	}
	std::cout << std::endl;
}

int main(int argc, char * argv[])
{
	std::vector<std::function<int(int)>> functions;
	CommandLineParameter cmd(argc, argv);
	
	if (cmd.m_fibonacci)
	{
		functions.push_back(fibonacci);
	}
	if (cmd.m_centeredTriangular)
	{
		functions.push_back([](int number){return centeredTriangular(number);});
	}
	if (cmd.m_powSum)
	{
		functions.push_back(std::bind(powSum, std::placeholders::_1, 2)); 
	}

	for (int i = 1; i < 45; i++)
	{
		applyFuntions(functions, i);
	}
}
