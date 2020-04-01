#include <cilk/cilk.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <chrono>
#include "tbb/task_scheduler_init.h"
//#include "tbb/parallel_for.h"
//#include "tbb/blocked_range.h"
//#include "tbb/scalable_allocator.h"
//using namespace tbb;
using namespace std;
using namespace std::chrono;

# define M_PI           3.14159265358979323846  /* pi, #include <math.h> */
int a = -1;
int b = 1;
double integral;


double func(double x)
{
	return 5 / sqrt(8 - 4 * x*x);
}

void calc(int i, double h)
{
	if (i % 2 == 0)integral = integral + 2 * func((double)(a + i * h));
	else integral = integral + 4 * func((double)(a + i * h));
}

int main()
{
	int q[5] = { 100, 1000, 10000, 100000, 1000000 };
	double h[5];
	for (int i = 0; i < 5; i++)
		h[i] = (double)(b - a) / (double)q[i];

	cout << " Value " << "|" << "  Error  " << "|" << "Time" << endl;

	for (int j = 0; j < 5; j++)
	{
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		integral = func(a) + func(b);
		cilk_for(int i = 1; i < q[j] - 1; i++)
		{
			calc(i, h[j]);
		}
		integral = integral * (h[j] / 3);
		high_resolution_clock::time_point t2 = high_resolution_clock::now();
		duration<double> duration = (t2 - t1);
		cout << integral << "|" << 5 * M_PI / 4 - integral << "|" << duration.count() << " seconds" << endl;
	}

	return 0;
}