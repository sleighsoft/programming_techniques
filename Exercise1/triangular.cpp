#include <cstdlib>
#include <iostream>
#include <limits.h>
#include <math.h>
#include <string>


int triangular(int n)
{
	int sum = 0;
	for(int i = 0; i < n; ++i) {
		sum += i;
	}
	return sum;
}

void pretty_print(int decimal)
{
	std::string decimalNumber = std::to_string(decimal);
	int insertAt = decimalNumber.length() - 3;
	while (insertAt > 0) {
    	decimalNumber.insert(insertAt, ".");
    	insertAt = insertAt - 3;
    }

	std::cout << decimalNumber;
}

int main(int argc, char * argv[])
{
	if(argc != 2)
		return 1;	// invalid number of parameters

	int n = std::atoi(argv[1]);

	double max = ceil(-0.5 + sqrt(0.25 + (double)INT_MAX * 2));

	if (0 <= n && n <= max) 
	{
		pretty_print(triangular(n));
		std::cout << std::endl;
	} 
	else
	{
		std::cout << "Input is out of integer range for triangular calculation" << std::endl;
		std::cout << "domain = [0;";
		pretty_print(max);
		std::cout << "]" << std::endl;
		std::cout << "codomain = [0;";
		pretty_print(INT_MAX);
		std::cout << "]" << std::endl;
	}	

	return 0;
}
