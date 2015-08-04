#include <cstdlib>
#include <iostream>

long stepCount = 0;

int fibonacci(int n)
{
    if(n <= 0 || n > 46 )  //fibonacci of negative n is 0, the max numer to be printed is 46 (fib(46) = 1.836.311.903, fib(47) = 2.971.215.073 --> signed int: 2.147.483.647
    {
        return 0;
    }
    if(n <= 2 ) //fibonacci(1) = 1, fib(2) = 1
    {
        return 1;
    }
    ++stepCount; 
    return fibonacci(n - 1) + fibonacci(n - 2);
}


int main(int argc, char * argv[])
{
	if(argc != 2)
		return 1;	// invalid number of parameters

	int n = std::atoi(argv[1]); 


	std::cout << n << " : " << fibonacci(n);
	std::cout << " : #" << stepCount << ".\n";

	return 0;
}
