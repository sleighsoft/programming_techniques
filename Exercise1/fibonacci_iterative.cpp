#include <cstdlib>
#include <iostream>


int *fibonacci(int n)
{
	int *results = new int[2];
	int fibonacci = 0;
	int stepCount = 0;

	if (n <= 0 || n == 1)
	{
		fibonacci = (n < 0) ? 0 : n;
	}
	else
	{
		int fib1 = 0; 
		int fib2 = 1;
	 
		for ( int i = 2; i <= n; i++ ) 
		{
			fibonacci = fib1 + fib2;
			fib1 = fib2;
			fib2 = fibonacci;
			++stepCount;
		}
	}
	results[0] = fibonacci;
	results[1] = stepCount;

	return results;	
}

int main(int argc, char * argv[])
{
	if(argc != 2)
		return 1;	// invalid number of parameters

	int n = std::atoi(argv[1]); 

	int *resultsArray = fibonacci(n);

	std::cout <<  n << " : " << resultsArray[0]  << " : #" << resultsArray[1] << ".\n";

	return 0;
}
