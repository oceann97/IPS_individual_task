#include <iostream>
#include <iomanip>
#include <locale>
#include <chrono>
#include <thread>
#include <mutex>
#include <cmath>

# define M_PI           3.14159265358979323846  /* pi, #include <math.h> */
using namespace std;
using namespace std::chrono;

double tmr = 0;
double value;
mutex gmutex;

double f(double x) {
	return 5 / sqrt(8 - 4 * x * x);
}

void compute1(int n) {
	int a = -1;
	int b = 1;
	double h = (double)(b - a) / (double)n;
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	double integral = f(a) + f(b);
	for (int i = 1; i < n - 1; i++)
	{
		if (i % 2 == 0)integral = integral + 2 * f((double)(a + i * h));
		else integral = integral + 4 * f((double)(a + i * h));
	}
	integral = integral * (h / 3);
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	cout << setprecision(25) << integral - 5 * M_PI / 4 << endl;
	duration<double> duration = (t2 - t1);
	cout << "Duration is: " << setprecision(5) << duration.count() << " seconds" << endl;
}

void compute2(double a, double b, int n) {
	double h = (double)(b - a) / (double)n;
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	double integral = f(a) + f(b);
	for (int i = 1; i < n - 1; i++)
	{
		if (i % 2 == 0)integral = integral + 2 * f((double)(a + i * h));
		else integral = integral + 4 * f((double)(a + i * h));
	}
	integral = integral * (h / 3);
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	gmutex.lock();
	value += integral;
	duration<double> duration = (t2 - t1);
	tmr += duration.count();
	gmutex.unlock();
}

int main() {
	setlocale(LC_ALL, "Rus");
	//#pragma loop(no_vector)
	//#pragma loop(hint_parallel(4))
	for (int i = 100; i < 10000001; i *= 10) {
		value = 0;
		tmr = 0;
		cout << "[" << i << "]" << endl;
		cout << "Без потоков: ";
		compute1(i);
		cout << "С потоками: ";
		thread t1(compute2, -1.0, -0.6, i);
		thread t2(compute2, -0.6, -0.2, i);
		thread t3(compute2, -0.2, 0.2, i);
		thread t4(compute2, 0.2, 0.6, i);
		thread t5(compute2, 0.6, 1.0, i);
		t1.join();
		t2.join();
		t3.join();
		t4.join();
		t5.join();
		cout << value - 5 * M_PI / 4 << endl;
		cout << "Duration is:" << tmr << " seconds" << endl;
	}
	return 0;
}
