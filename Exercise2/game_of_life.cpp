#include <iostream>
#include <ctime>
#include <cstring>
#include "bitmap_image.hpp"
#include <random>

struct Raster {
	Raster(int w, int h) : width(w), height(h)
	{
		data = new int[width*height];
	}

	Raster(int w, int h, float seedProbability) : width(w), height(h)
	{
		data = new int[width*height];

		srand(time(NULL));
		long size = width * height;
		long half = size * seedProbability;
		long random = rand() % size;
		for(long i = 0; i < half; ++i)
		{
			data[random] = 1;
		}		
	}

	Raster(const std::string &filename)
	{
		bitmap_image image(filename);

		if (!image)
		{
			std::cerr << "Could not open bitmap!" << std::endl;
		}

		height = image.height();
		width = image.width();

		data = new int[width*height];
			
		unsigned char red;
		unsigned char green;
		unsigned char blue;
		int index;
		
		for (std::size_t y = 0; y < height; ++y)
		{
			for (std::size_t x = 0; x < width; ++x)
			{
				image.get_pixel(x,y,red,green,blue);
				index = y * width + x;
				if (red == 0 && green == 0 && blue == 0)
				{
					data[index] = 1;
				}
				else
				{
					data[index] = 0;
				}
			}
		}
		
	}

	void save(const std::string &filename)
	{
		bitmap_image image(width, height);
		for (std::size_t y = 0; y < height; ++y)
		{
		  for (std::size_t x = 0; x < width; ++x)
		  {
			 int pix = data[y * width + x];
			 if(pix == 1) {
				 image.set_pixel(x,y,0,0,0);
			 }
			 else
			 {
				 image.set_pixel(x,y,255,255,255);
			 }
		     
		  }
		}
		image.save_image(filename);
	}

	~Raster()
	{
		delete[] data;
	}

	int width;
	int height;
	int* data;
};

struct CommandLineParameter
{
	CommandLineParameter(int argc, char* argv[])
		: width(0)
		, height(0)
		, invasionFactor(0)
		, isTorus(false)
		, maxIterations(20)
	{
		if (argc % 2 == 0)
		{
			std::cerr << "Missing value for " << argv[argc - 1] << std::endl;
			argc--;
		}

		for (int i = 1; i < argc; i += 2)
		{
			if (!strcmp(argv[i], "-w"))
			{
				width = atoi(argv[i + 1]);
			}
			else if (!strcmp(argv[i], "-h"))
			{
				height = atoi(argv[i + 1]);
			}
			else if (!strcmp(argv[i], "-s"))
			{
				seedProbability = atof(argv[i + 1]);
			}
			else if (!strcmp(argv[i], "-p"))
			{
				patternFilename = argv[i + 1];
			}
			else if (!strcmp(argv[i], "-o"))
			{
				outputDirectory = argv[i + 1];
			}
			else if (!strcmp(argv[i], "-iv"))
			{
				invasionFactor = atof(argv[i + 1]);
			}
			else if (!strcmp(argv[i], "-t"))
			{
				isTorus = strcmp(argv[i + 1], "0") != 0;
			}
			else if (!strcmp(argv[i], "-i"))
			{
				maxIterations = atoi(argv[i + 1]);
			}
		}

		if ((width != 0 || height != 0) && !patternFilename.empty())
		{
			std::cout << "Width and height are ignored, because pattern is defined." << std::endl;
		}

		if (width < 0 || height < 0)
		{
			std::cerr << "Width or height has a invalid value." << std::endl;
			width = 0;
			height = 0;
		}
	}

	int width;
	int height;
	float seedProbability;
	std::string patternFilename;
	std::string outputDirectory;
	float invasionFactor;
	bool isTorus;
	int maxIterations;
};

int neighborValue(const Raster &raster, int x, int y, bool isTorus)
{
	bool outside = false;
	int index = y * raster.width + x;
	if(x > raster.width || y > raster.height)
	{
		outside = true;
	}
	if(isTorus)
	{
		if(outside)
		{
			index = (y % raster.height) * raster.width + (x % raster.width);
		}
	}
    return (!isTorus && outside) ? 0 : raster.data[index];
}

int determineState(Raster &raster, int x, int y, bool isTorus)
{
	int dot = neighborValue(raster, x, y, isTorus);
	int prevX = neighborValue(raster, x-1, y, isTorus);
	int nextX = neighborValue(raster, x+1, y, isTorus);
	int prevY = neighborValue(raster, x, y-1, isTorus);
	int nextY = neighborValue(raster, x, y+1, isTorus);
	int topL = neighborValue(raster, x-1, y-1, isTorus);
	int topR = neighborValue(raster, x+1, y-1, isTorus);
	int botL = neighborValue(raster, x-1, y+1, isTorus);
	int botR = neighborValue(raster, x+1, y+1, isTorus);
	
	int sum = prevX + nextX + prevY + nextY + topL + topR + botL + botR;
	
	if(dot == 0)
	{
		if(sum == 3)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		if(sum < 2)
		{
			return 0;
		}
		else if(sum >= 4)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}

void simulateInvasion(Raster &raster, float invasionFactor)
{
	if (invasionFactor <= 0)
	{
		return;
	}
	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution(0.0,1.0);
	for (std::size_t y = 0; y < raster.height; ++y)
	{
		for (std::size_t x = 0; x < raster.width; ++x)
		{

			if(distribution(generator) <= invasionFactor)
			{
				int dot = raster.data[y * raster.width + x];
				if(dot == 0) {
					raster.data[y * raster.width + x] = 1;
				}
				else
				{
					raster.data[y * raster.width + x] = 0;
				}
			}
		}
	}
}

void simulateNextState(Raster &raster, bool isTorus)
{
	int data2[raster.width * raster.height];
	memcpy(&data2, &raster.data, sizeof data2);
	int index;
	for (std::size_t y = 0; y < raster.height; ++y)
	{
		for (std::size_t x = 0; x < raster.width; ++x)
		{
			index = y * raster.width + x;
			data2[index] = determineState(raster, x, y, isTorus);
		}
	}
	for(std::size_t i = 0; i < raster.height * raster.width; ++i)
	{
		raster.data[i] = data2[i];
	}
}

int main(int argc, char* argv[])
{
	Raster* raster = nullptr;

	CommandLineParameter cmd(argc, argv);
	if (!cmd.patternFilename.empty())
	{
		raster = new Raster(cmd.patternFilename);
	}
	else
	{
		raster = new Raster(cmd.width, cmd.height, cmd.seedProbability);
	}

	for (int iteration = 0; iteration <= cmd.maxIterations; iteration++)
	{
		raster->save(cmd.outputDirectory + "game_of_life_" + std::to_string(iteration) + ".bmp");
		simulateInvasion(*raster, cmd.invasionFactor);
		simulateNextState(*raster, cmd.isTorus);
	}

	delete raster;

	return 0;
}