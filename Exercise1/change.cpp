#include <cstdlib>
#include <iostream>
#include <map>
#include <fstream>

using namespace std;

map<int, int> coins;

void change(const int due, const int paid, map<int, int>& change)
{
	// List of coin types
	change[1] = 0;
	change[2] = 0;
	change[5] = 0;
	change[10] = 0;
	change[20] = 0;
	change[50] = 0;
	change[100] = 0;
	change[200] = 0;
	change[500] = 0;
	change[1000] = 0;
	change[2000] = 0;
	change[5000] = 0;

	int change_dif = paid - due;

	int currentCoinIndex = coins.size();
	int currentCoinValue = coins[coins.size()];

	// Subtracts the highest possible coin from the change_dif for as long as it does not result
	// in a negative number. Then lowers the coin and repeats the process.
	while(change_dif > 0)
	{
		while(currentCoinIndex > 0) {
			if(change_dif - currentCoinValue >= 0) {
				break;
			}
			currentCoinIndex--;
			currentCoinValue = coins[currentCoinIndex];
		}
		
		change_dif -= currentCoinValue;
		change[currentCoinValue]++;
	}

	cout << "coin, num" << endl;
	for (int i = 12; i > 0; i--) {
		if(change[coins[i]] > 0) {
			cout << coins[i] << ", " << change[coins[i]] << endl;
		}
	}
}

int main(int argc, char * argv[])
{
	char badInput = 1;
	char withOutput = 0;
	// Input parameter handling
	if(argc == 3)
	{
		badInput = 0;
	} 
	else if(argc == 5)
	{
		if(strcmp(argv[3], "-o") == 0)
		{
			badInput = 0;
			withOutput = 1;
		}
	}
	if(badInput)
	{
		cout << "Specify the following arguments: due paid (-o <filename>)" << endl;
		return 1;
	}

	const int due  = atoi(argv[1]); 
	const int paid = atoi(argv[2]);
	coins[1] = 1;
	coins[2] = 2;
	coins[3] = 5;
	coins[4] = 10;
	coins[5] = 20;
	coins[6] = 50;
	coins[7] = 100;
	coins[8] = 200;
	coins[9] = 500;
	coins[10] = 1000;
	coins[11] = 2000;
	coins[12] = 5000;

	// Input handling
	if (due < 0 || paid < 0)
	{
		cout << "Invalid input, not in the range of an integer" << endl;
		return 1;
	}

	if (paid < due)
	{
		cout << "You have not paid enough money" << endl;
		return 1;
	}

	// Initialize map and call change()
	map<int,int> change_map;
	change(due, paid, change_map);

	// If -o command was given ouput it into the apropriate file
	if(withOutput)
	{
		ofstream outputFile;
		outputFile.open(argv[4]);

 		outputFile << "coin, num" << "\n";
		for (int i = 12; i > 0; i--) {
			if(change_map[coins[i]] > 0) {
				outputFile << coins[i] << ", " << change_map[coins[i]] << "\n";
			}
		}
  		outputFile.close();
	}

	return 0;
}
