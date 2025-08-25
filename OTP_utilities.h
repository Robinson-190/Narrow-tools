#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define LINEAR 0
#define LIN LINEAR
#define LOGARITHMIC 1
#define LOG LOGARITHMIC

#define RED 31
#define BLUE 36
#define MAGENTA 35
#define BOLD 44
#define WARNING 41

#define special_print(colour, fmt, ...)                                                                                \
    printf("\033[0;%dm", colour);                                                                                      \
    printf(fmt, ##__VA_ARGS__);                                                                                        \
    printf("\033[0m\n");

#define bold_print(fmt, ...)                                                                                           \
    printf("\033[1;32;44m");                                                                                           \
    printf(fmt, ##__VA_ARGS__);                                                                                        \
    printf("\033[0m\n");

#define print_error(fmt, ...)                                                                                          \
    printf("\033[1;32;41m");                                                                                           \
    printf(fmt, ##__VA_ARGS__);                                                                                        \
    printf("\033[0m\n");

/*
One-Trick Pony functions.
Generally only useful for exactly what they do.
*/

/**
 * @brief generate a set of linearly spaced points between `start` and `end`. Result will be stored in the given array.
 *
 * @param start initial point
 * @param end end point
 * @param length how many points
 * @param array_to_fill array in which to store points
 */
void linspace(double start, double end, int length, double array_to_fill[length])
{
    double dx = (end - start) / ((double)length - 1.0);
    array_to_fill[0] = start;

    if (length >= 2)
    {
        array_to_fill[length - 1] = end;
        for (int i = 1; i < length - 1; i++)
        {
            array_to_fill[i] = array_to_fill[i - 1] + dx;
        }
    }
    else if (length == 1)
    {
        array_to_fill[0] = start + 0.5 * dx;
    }
}

/**
 * @brief generate a set of logarithmically spaced points between 10^`start` and 10^`end`. Result will be stored in the
 * given array.
 *
 * @param start logarithm of initial point
 * @param end logarithm of end point
 * @param length how many points
 * @param array_to_fill array in which to store points
 */
void logspace(double log10_start, double log10_end, int length, double array_to_fill[length])
{
    double dx = pow(10.0, (log10_end - log10_start) / ((double)length - 1.0));
    array_to_fill[0] = pow(10.0, log10_start);

    if (length >= 2)
    {
        array_to_fill[length - 1] = pow(10.0, log10_end);
        for (int i = 1; i < length - 1; i++)
        {
            array_to_fill[i] = array_to_fill[i - 1] * dx;
        }
    }
}

/**
 * @brief Save csv files for an arbitrary (but specified) amount of arrays of doubles. All arrays must be of the same
 * length.
 *
 * Example use:
 * `save_csv("data.csv","x,y_1,y_2,y_3",100,4,x_arr,y1_arr,y12_arr,y3_arr);`
 *
 * @param filename File name
 * @param header First line of the file
 * @param array_length How many entries are in each array
 * @param number_of_arrays Total number of arrays
 * @param ... Pointers to each array to store
 */
void save_csv(char *filename, char *header, int array_length, int number_of_arrays, ...)
{
    // Declaring pointer to the
    // argument list
    va_list ptr;
    // Initializing argument to the
    // list pointer
    va_start(ptr, number_of_arrays);

    // array of addresses of separate arrays to write to
    double *data[number_of_arrays];
    // save start address of each input argument to array
    for (int i = 0; i < number_of_arrays; i++)
    {
        data[i] = va_arg(ptr, double *); // pass each double array address to an array for storage
    }
    va_end(ptr);
    FILE *file_ptr;
    file_ptr = fopen(filename, "w");
    if (file_ptr == NULL)
    {
        printf("Error opening file \"%s\"!", filename);
    }
    else
    {
        fprintf(file_ptr, "%s\n", header);
        // For each entry, write a,b,c,...\n to file
        for (int j = 0; j < array_length; j++)
        {
            for (int i = 0; i < number_of_arrays; i++)
            {
                fprintf(file_ptr, "%e", *(data[i] + j));
                if (i + 1 < number_of_arrays)
                    fprintf(file_ptr, ","); // don't print comma for last entry
            }
            fprintf(file_ptr, "\n");
        }
        fclose(file_ptr);
    }
}

/**
 * @brief Draw graphs in gnuplot with multiple lines.
 *
 * @param array_lengths Array of the number of entries in each pair of data arrays.
 * Note: for a graph of  (`a` vs `b`) and (`c` vs `d`), only the length of `a` and `c` need be specified.
 * @param x_axis_scale The scale of the x-axis, specified to be either `LINEAR`/`LIN` or `LOGARITHMIC`/`LOG`.
 * @param y_axis_scale The scale of the y-axis, specified to be either `LINEAR`/`LIN` or `LOGARITHMIC`/`LOG`.
 * @param title The title to be included in the graph.
 * @param number_of_arrays How many arrays are to be plotted. For each x-y pair this must increase by 2, not 1.
 * @param ... Pointers to each array to plot.
 */
void basic_graphs(int *array_lengths, int x_axis_scale, int y_axis_scale, char *title, int number_of_arrays, ...)
{
    // Declaring pointer to the
    // argument list
    va_list ptr;
    // Initializing argument to the
    // list pointer
    va_start(ptr, number_of_arrays);
    // array of addresses of separate arrays to write to
    double *data[number_of_arrays];
    // save start address of each input argument to array
    for (int i = 0; i < number_of_arrays; i++)
    {
        data[i] = va_arg(ptr, double *); // pass each double array start address to array for storage.
    }
    va_end(ptr);

    FILE *gnuplot = popen("gnuplot -p", "w");
    fprintf(gnuplot, "set style data linespoints\n");
    // fprintf(gnuplot, "set nokey\n");
    fprintf(gnuplot, "set title '%s'\n", title);
    if (x_axis_scale == LOGARITHMIC)
    {
        fprintf(gnuplot, "set logscale x 10\n");
    }
    if (y_axis_scale == LOGARITHMIC)
    {
        fprintf(gnuplot, "set logscale y 10\n");
    }
    fprintf(gnuplot, "plot '-' title \"Dataset %d\"", 0);

    for (int i = 1; i < number_of_arrays / 2; i++)
    {
        fprintf(gnuplot, ",'-'title \"Dataset %d\"", i);
    }
    fprintf(gnuplot, "\n");

    for (int i = 0; i < number_of_arrays / 2; i++) // IMPORTANT:
    {
        double *x_ptr = data[2 * i];
        double *y_ptr = data[2 * i + 1];
        for (int j = 0; j < array_lengths[i]; j++)
        {
            fprintf(gnuplot, "%e %e\n", *(x_ptr + j), *(y_ptr + j));
        }
        fprintf(gnuplot, "e\n");
    }
    fprintf(gnuplot, "\n");
    fflush(gnuplot);
    pclose(gnuplot);
}

/**
 * @brief Draw a basic graph using gnuplot.
 *
 * @param array_length length of the x and y arrays
 * @param x_values array of x values
 * @param y_values array of y values
 * @param x_axis_scale The scale of the x-axis, specified to be either `LINEAR`/`LIN` or `LOGARITHMIC`/`LOG`.
 * @param y_axis_scale The scale of the y-axis, specified to be either `LINEAR`/`LIN` or `LOGARITHMIC`/`LOG`.
 * @param title The title to be included in the graph.
 */
void basic_graph(int array_length, double x_values[array_length], double y_values[array_length], int x_axis_scale,
                 int y_axis_scale, char *title)
{
    FILE *gnuplot = popen("gnuplot -p", "w");

    fprintf(gnuplot, "set style data linespoints\n");
    fprintf(gnuplot, "set nokey\n");
    fprintf(gnuplot, "set title '%s'\n", title);

    if (x_axis_scale == LOGARITHMIC)
    {
        fprintf(gnuplot, "set logscale x 10\n");
    }
    if (y_axis_scale == LOGARITHMIC)
    {
        fprintf(gnuplot, "set logscale y 10\n");
    }
    fprintf(gnuplot, "plot '-'\n");

    for (int i = 0; i < array_length; i++)
    {
        fprintf(gnuplot, "%e %e\n", x_values[i], y_values[i]);
    }
    fprintf(gnuplot, "with lines e\n");
    fflush(gnuplot);
    pclose(gnuplot);
}

/**
 * @brief Linearly interpolate between two points
 *
 * @param x_target The point to return a y value for
 * @param x1 Horizontal location of point 1
 * @param x2 Horizontal location of point 2
 * @param y1 Vertical location of point 1
 * @param y2 Vertical location of point 2
 * @return double the y value corresponding to the given x value
 */
double interpolate_point(double x_target, double x1, double x2, double y1, double y2)
{
    return x1 + (x_target - x1) * (y2 - y1) / (x2 - x1);
}

/**
 * @brief Trapezoidal
 *
 * @param array_length
 * @param x
 * @param y
 * @param start
 * @param stop
 * @return double
 */
/*
double integrate_trapezoid(int array_length, double x[array_length], double y[array_length], double start, double stop)
{
    double I = 0.0;
    for (int i = 0; i < array_length - 1; i++)
    {
        if (x[i + 1] < start || x[i] >= stop)
        {
            continue;
        }
        if (x[i] >= start && x[i + 1] <= stop)
        {
            I += (x[i + 1] - x[i]) * (y[i + 1] + y[i]);
        }
        else if (x[i] <= start && x[i + 1] >= start)
        {
            printf("Interpolate:start\n");
            I += (start - x[i]) * (y[i] + interpolate_point(start, x[i], x[i + 1], y[i], y[i + 1]));
        }
        else if (x[i] <= stop && x[i + 1] >= stop)
        {
            printf("Interpolate:stop\n");
            I += (x[i + 1] - stop) * (y[i + 1] + interpolate_point(stop, x[i], x[i + 1], y[i], y[i + 1]));
        }
    }
    I /= 2.0;
    return I;
}
*/