#include <iostream>
#include <fstream>
#include <string>
#include "spline_kernel.h"
#include "interval.h"
#include "cardinal_spline.h"
#include "spline.h"
using namespace std;
using namespace sml;


template<typename T>
void SaveFunctionToFile(T f, sml::interval interval, int grid_size, string path) 
{
	const auto [a, b] = interval;

	ofstream myfile(path + "\\example.txt");
	if (myfile.is_open())
	{
		
		for (int i = 0; i < grid_size; i++)
		{
			auto x = a + i * interval.get_step(grid_size);
			auto y = f(x);
			myfile << x << " " << y << std::endl;
		}


		myfile.close();
	}
	else cout << "Unable to open file";
}



#include <Eigen/Dense>

int main()
{
	Eigen::Vector2f vec(0, 1);
	using sml::spline;

	sml::interval interval(1.4, 1.0);
	sml::interval i = {0.0, 1.0};
	auto [a, b] = i;


	std::cout << a << " " << b << std::endl;
	std::cout << interval.length() << std::endl;

    std::cout << "Hello World! " << meaning_of_life() << std::endl;

	sml::cardinal::api_bsplvb(0.25, 3);

	const int size = 10;
	sml::spline<double, 2u> f(size, { 0., 1. }), g(size, { 0., 1. });


	for (size_t i = 0; i < f.size(); i++)
	{
		f[i] = i + 1u;
	}

	auto h = f + g;

	std::cerr << "h(0.5) = " << h(0.5) << std::endl;
}
