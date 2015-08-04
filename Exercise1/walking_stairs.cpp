#include <cstdlib>
#include <iostream>

unsigned long long combinations(int number)
{
	number = number + 1;
	unsigned long long previous = 0;
	unsigned long long result = 1;
	unsigned long long sum;

	for (int i = 2; i <= number; ++i)
	{
		sum = previous + result;
		previous = result;
		result = sum;
	}
	return result;
}


int main(int argc, char * argv[]){

	if(argc != 2)
		return 1;	// invalid number of parameters

	int n = std::atoi(argv[1]);

	if(n > 92)
	{
		std::cout << "Input cannot be computed. Use a value smaller than 93" << std::endl;
		return 1;
	}

	std::cout << combinations(n) << "\n";

	return 0;
}
