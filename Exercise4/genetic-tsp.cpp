#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <random>
#include <algorithm>
#include <iterator>
#include <limits>
#include <string>
#include <ctime>

using namespace std;
ostream& operator<<(ostream& os, const vector<int>& A)
{
    os << A[0];
    for(int i=1; i<A.size(); ++i) os << "," << A[i];
    return os;
}
inline float frand(float a, float b)
{
    return a+((b-a) * (float)rand() / (float)RAND_MAX);
}

const int N = 20;
const vector<int> NullTour(N, -1);
const int M = 20;
int minDist = numeric_limits<int>::max();
int maxDist = numeric_limits<int>::min();

const vector<string> cityName {
    "Berlin", "Hamburg", "München", "Köln", "Frankfurt am Main", "Stuttgart", "Düsseldorf", "Dortmund", "Essen", "Bremen", "Dresden", "Leipzig", "Hannover", "Nürnberg", "Duisburg", "Bochum", "Wuppertal", "Bielefeld", "Bonn", "Münster"
};

const int distance_table[N][N] = {
	{000, 255, 504, 477, 424, 512, 477, 422, 453, 315, 165, 149, 249, 378, 471, 439, 452, 336, 479, 398}, 
	{255, 000, 612, 356, 393, 534, 338, 284, 308, 95, 377, 294, 132, 462, 321, 297, 320, 196, 370, 237}, 
	{504, 612, 000, 456, 304, 190, 486, 477, 493, 582, 359, 360, 488, 150, 503, 486, 470, 483, 433, 510}, 
	{477, 356, 456, 000, 152, 288, 34, 72, 57, 269, 474, 380, 249, 336, 56, 62, 37, 162, 25, 123}, 
	{424, 393, 304, 152, 000, 152, 182, 177, 190, 329, 371, 293, 261, 187, 200, 184, 167, 212, 131, 218}, 
	{512, 534, 190, 288, 152, 000, 322, 328, 335, 478, 412, 364, 401, 157, 342, 332, 311, 364, 264, 371}, 
	{477, 338, 486, 34, 182, 322, 000, 57, 30, 248, 485, 389, 240, 363, 23, 41, 26, 150, 59, 101}, 
	{422, 284, 477, 72, 177, 328, 57, 000, 32, 196, 439, 340, 182, 343, 49, 17, 36, 93, 90, 51}, 
	{453, 308, 493, 57, 190, 335, 30, 32, 000, 217, 470, 371, 212, 364, 18, 14, 24, 122, 81, 70}, 
	{315, 95, 582, 269, 329, 478, 248, 196, 217, 000, 405, 310, 100, 433, 229, 207, 231, 118, 285, 150}, 
	{165, 377, 359, 474, 371, 412, 485, 439, 470, 405, 000, 100, 312, 259, 487, 456, 460, 375, 467, 434}, 
	{149, 294, 360, 380, 293, 364, 389, 340, 371, 310, 100, 000, 214, 229, 389, 358, 363, 275, 375, 334}, 
	{249, 132, 488, 249, 261, 401, 240, 182, 212, 100, 312, 214, 000, 338, 229, 199, 217, 90, 258, 151}, 
	{378, 462, 150, 336, 187, 157, 363, 343, 364, 433, 259, 229, 338, 000, 377, 355, 343, 337, 318, 370}, 
	{471, 321, 503, 56, 200, 342, 23, 49, 18, 229, 487, 389, 229, 377, 000, 32, 33, 138, 81, 84}, 
	{439, 297, 486, 62, 184, 332, 41, 17, 14, 207, 456, 358, 199, 355, 32, 000, 26, 109, 84, 61}, 
	{452, 320, 470, 37, 167, 311, 26, 36, 24, 231, 460, 363, 217, 343, 33, 26, 000, 128, 58, 85}, 
	{336, 196, 483, 162, 212, 364, 150, 93, 122, 118, 375, 275, 90, 337, 138, 109, 128, 000, 175, 62}, 
	{479, 370, 433, 25, 131, 264, 59, 90, 81, 285, 467, 375, 258, 318, 81, 84, 58, 175, 000, 142}, 
	{398, 237, 510, 123, 218, 371, 101, 51, 70, 150, 434, 334, 151, 370, 84, 61, 85, 62, 142, 000}
};
bool tourDefined(const vector<int>& tour)
{
    for(int i=0; i<N; ++i)
    {
        if(tour[i]==-1) return false;
    }
    return true;
}
bool findCity(const vector<int>& tour, int city)
{
    assert(city>=0 && city<N);
    assert(tour.size()==N);
    
    for(int i=0; i<N; ++i)
    {
        if(tour[i]==city) return true;
    }
    return false;
}

bool vectorContains(const vector<int> v, int elem)
{
    if(find(v.begin(), v.end(), elem) != v.end())
    {
       return true;
    } 
    else
    {
       return false;
    }
}

bool validTour(const vector<int>& tour)
{
    for(int i=0; i<N; ++i)
    {
        if(!findCity(tour, i)) return false;
    }
    return true;
}

int cityDistance(int city1, int city2)
{
    assert(city1>=0 && city1<N);
    assert(city2>=0 && city2<N);

	return distance_table[city1][city2]; 
}

int tourLength(const vector<int>& T)
{
    assert(validTour(T));
    
    int length = 0;
    int previousLoc = T[0];
    for(auto &loc : T)
    {
        length += cityDistance(previousLoc, loc);
        previousLoc = loc;
    }
    length += cityDistance(previousLoc, T[0]);
    return length;
}

void printTourCityNames(const vector<int>& T)
{
    assert(validTour(T));
    for(int i=0; i<N; ++i) cout << (i==0 ? "" : "-") << cityName[T[i]];
    cout << endl;
}

void insertCity(vector<int>& tour, int city)
{
    assert(city>=0 && city<N);
    assert(tour.size()<N);
    
	tour.push_back(city);
}

void generateTours(vector< vector<int> >& tourSet)
{
    std::random_device rd;
    std::mt19937 g(rd());
    vector<int> basicTour;
    for(int i=0; i < N; ++i)
    {
        basicTour.push_back(i);
    }
    for(int i=0; i < tourSet.size(); ++i)
    {
        vector<int> newRandomTour = basicTour;
        std::shuffle(newRandomTour.begin(), newRandomTour.end(), g);
        tourSet[i] = newRandomTour;
    }
}

void crossover(const vector<int>& parent1, const vector<int>& parent2, vector<int>& child)
{
    assert(validTour(parent1));
    assert(validTour(parent2));
    child.clear();
    int random = rand() % parent1.size() + 1;
    
    for(int i = 0; i < random; ++i)
    {
        child.push_back(parent1[i]);
    }
    for(int i = random; i < parent1.size(); ++i)
    {
        if(!vectorContains(child, parent2[i]))
        {
            child.push_back(parent2[i]);
        }
    }
    for(int i = 0; i < parent1.size(); ++i)
    {
       if(!vectorContains(child, i))
        {
            child.push_back(i);
        } 
    }
}

void mutate(vector<int>& tour)
{
    assert(validTour(tour));
    const float mutationProbability = 0.02f;
    float randomMutation;
    int randomPosition;
    for(int i=0; i < tour.size(); ++i)
    {
        randomMutation = frand(0,1);
        if(randomMutation <= mutationProbability)
        {
            while((randomPosition = rand() % tour.size()) == i);
            swap(tour[i], tour[randomPosition]);            
        }
    }
}

bool tourLenghtComparator(pair<int,int> firstElem, pair<int,int> secondElem)
{
    return firstElem.first < secondElem.first;
}

vector<pair<int,int>> fitness(vector<vector<int>>& tourSet)
{
    vector<pair<int,int>> F;

    int index = 0;
    for(auto &tour : tourSet)
    {
        F.push_back(make_pair(tourLength(tour), index));
        index++;
    }
	std::sort(F.begin(), F.end(), tourLenghtComparator);
    return F;
}

pair<int,int> evolution(vector<vector<int>>& tourSet, bool elite)
{
    assert(tourSet.size()==M);
    pair<int,int> statistics;
    
    auto F = fitness(tourSet);
    statistics.first = F[0].first;
    statistics.second = F[M-1].first;

    crossover(tourSet[F[0].second], tourSet[F[1].second], tourSet[F.back().second]);
    for(int i = 0; i < tourSet.size(); ++i)
    {
        if(i != F[0].second && i != F[1].second && i != F.back().second)
        {
            mutate(tourSet[i]);
        }
    }
	return statistics;
}


int main(int argc, char** argv)
{
    srand((unsigned int)time(0));
    
    std::ofstream stats("output.csv", std::ios::binary);
    stats << "min tour length" << "; " << "max tour length" << endl;
    
    vector<vector<int>> TourSet(M, NullTour);
    generateTours(TourSet);
    
    for(int e=0; e<1000; e++)
    {
        auto lengths = evolution(TourSet, false);
        
        stats << lengths.first << "; " << lengths.second <<endl;
    }
    
    auto FE = fitness(TourSet);
    cout << "final shortest trip:  " << FE[0].first << "km" << endl;
    cout << "over all generations, min dist = " << minDist << ", max dist = " << maxDist << endl;
    
    stats.close();
    
    return 0;
}

