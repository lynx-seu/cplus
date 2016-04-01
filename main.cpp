#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;
#include "dlx.h"
#include <time.h>

int arr[6][7] =
{
	{1, 0, 0, 1, 0, 0, 1},
	{1, 0, 0, 1, 0, 0, 0},
	{0, 0, 0, 1, 1, 0, 1},
	{0, 0, 1, 0, 1, 1, 0},
	{0, 1, 1, 0, 0, 1, 1},
	{0, 1, 0, 0, 0, 0, 1},
};

void f(int r)
{
	if (r==hxl::Dlx::nullrow) return;
	std::cout << r << " ";
}

int main()
{
	clock_t t1 = clock();
	hxl::Dlx dlx((int *)&arr, 6, 7);
	dlx.Dance(f);
	clock_t dt = clock() - t1;
	std::cout << "\n cost: " << dt << "ms\n";

	return 0;
}


