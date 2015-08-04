#include <cassert>
#include <iostream>
#include <string>
#include <cstddef>
#include <vector>
#include <cstring>
#include <cmath>
#include <algorithm>

static const auto N = 4;

auto A = std::vector<int>();
auto B = std::vector<int>();
auto C = std::vector<int>();
const std::vector<char> names {'A', 'B', 'C'};

void print()
{
    #ifdef _WIN32
    std::system("cls");
    #else
    std::system("clear");
    #endif

    std::vector<std::vector<int>> vl {A,B,C};
    int index = 0;
    std::for_each(vl.begin(), vl.end(),
                [&](const std::vector<int> &vec){
                    std::cout << names[index++] << std::endl;
                    std::for_each(vec.rbegin(), vec.rend(), 
                                [](const int &n){
                                    for(int i = 0; i < n; ++i){std::cout << "-";}
                                    std::cout << std::endl;
                                }); 
                    std::cout << std::endl;
                });
    std::cout << std::endl << std::endl;
}

void ToH(const int n, std::vector<int> & a, std::vector<int> & b, std::vector<int> & c, int & moves) 
{
    if(n > 0)
    {
        ToH(n-1, a, c, b, moves);
        if(a.size() > 0)
        {
            c.push_back(a.back());
            a.pop_back();
            ++moves;
            print();
            getchar();
        }
        ToH(n-1, b, a, c, moves);
    }
}

int main(int argc, char ** argv) 
{
    int moves = 0;

    for (int i = N; i > 0; --i)
        A.push_back(i);

	print();
    ToH(N, A, B, C, moves);
    std::cout << "minimal number of moves: " << moves << std::endl;
    return 0;
}
