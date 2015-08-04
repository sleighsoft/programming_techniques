#include <iostream>
#include <time.h>
#include <vector>
#include <algorithm>

struct Interval
{
    Interval(const int i, const int start, const int length)
    : index(i)
    , start(start)
    , end(start + length){}

    int index;
    int start;
    int end;
};

const static int MaxEnd = 74;
const static int MaxDuration = 10;

const static int N = 20;

std::ostream & operator<<(std::ostream & os, const std::vector<Interval> & I)
{
	os << I.size() << std::endl;

    int index = 0;
    for(auto &i : I)
    {
        os << "#" << index++ << "\t|";
        for(int j = 0; j < i.start; ++j)
        {
            os << ".";
        }
        for(int j = i.start; j < i.end; ++j)
        {
            os << "x";
        }
        for(int j = i.end; j < MaxEnd; ++j)
        {
            os << ".";
        }
        os << "|" << std::endl;
    }
	return os;
}

void randomize(std::vector<Interval> & intervals)
{
    intervals.clear();
    srand(time(0));

    for (int i = 0; i < N; i++)
    {
        int duration = rand() % MaxDuration + 1;
        int start = rand() % (MaxEnd - duration);

        intervals.push_back(Interval(i, start, duration));
    }
}


void schedule(const std::vector<Interval> & intervals)
{

    std::cout << std::endl << "intervals (randomized):" << std::endl << intervals;


	auto sorted = intervals;
    std::sort(sorted.begin(), sorted.end(),
            [](const Interval &i1, const Interval &i2){return i1.end < i2.end;});

    std::cout << std::endl << "intervals (sorted):" << std::endl << sorted;

    auto scheduled = std::vector<Interval>();

    for(auto &i : sorted)
    {
        if(scheduled.size() == 0)
        {
            scheduled.push_back(i);
        }
        else
        {
            if(i.start > scheduled.back().end)
            {
                scheduled.push_back(i);
            }
        }
    }
    std::cout << std::endl << "intervals (scheduled, " << scheduled.size() << " of " << sorted.size() << " possible)"
        << std::endl << scheduled << std::endl;
}

int main(int argc, char** argv) {

    auto intervals = std::vector<Interval>();

    randomize(intervals);
    schedule(intervals);

    return 0;
}
