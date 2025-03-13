#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "OTP_utilities.h"

int main()
{
    // test linspace and logspace
    int N_linlogspace_test = 6;
    double x_lin[N_linlogspace_test];
    double x_log[N_linlogspace_test];

    linspace(0.0, 10.0, N_linlogspace_test, x_lin);
    logspace(0.0, 10.0, N_linlogspace_test, x_log);

    for (int i = 0; i < N_linlogspace_test; i++)
    {
        printf("%lf, ", x_lin[i]);
    }
    printf("\n");
    for (int i = 0; i < N_linlogspace_test; i++)
    {
        printf("%e, ", x_log[i]);
    }
    printf("\n");

    // test graphing capabilities
    int N = 1000;
    double x[N];
    double y[N];
    double y2[N];

    linspace(1, 30, N, x);

    for (int i = 0; i < N; i++)
    {
        y[i] = sqrt(x[i]);
        y2[i] = (1.0 + 0.2 * sin(x[i])) * sqrt(x[i]);
    }

    // Make a simple x-y graph of two arrays
    basic_graph(N, x, y, LINEAR, LINEAR, "Square root plot");

    // Graph multiple quantities gainst each other, on the same axes
    int array_lengths[6] = {N, N, N, N, N, N};
    basic_graphs(array_lengths, LOGARITHMIC, LOGARITHMIC, "Multiple lines test", 6, x, y, x, y2, y, y2);

    // Save results to a csv file
    save_csv("results.csv", "x,y,y2", N, 3, x, y, y2);

    // Test integration
    // y=1+sin(x). Integrate between 2 and 10
    // Analytical solution = 8.42292469252931
    for (int i = 0; i < N; i++)
    {
        y[i] = 1.0 + sin(x[i]);
    }

    // double integral_analytical = 8.42292469252931;
    // double integral_numerical = integrate_trapezoid(N, x, y, 2, 10.0);
    // printf("Analytical solution:\t%lf\nNumerical Solution\t%lf\n", integral_analytical, integral_numerical);
    return 0;
}