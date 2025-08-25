### Narrow Tools

A set of reusable functions in C. These include 

- `linspace` Writes a set of linearly spaced points into the provided array of doubles. The following stores the values `{0.0,2.0,4.0,6.0,8.0,10.0}` in the array `x_lin`
    ~~~
    double x_lin[6];
    linspace(0.0, 10.0, 6, x_lin);
    ~~~
- `logspace` Writes a set of logarithmically spaced points into the provided array of doubles. The following stores the values `{1e0,1e2,1e4,1e6,1e8,1e10}` in the array `x_log`
    ~~~
    double x_log[6];
    logspace(0.0, 10.0, 6, x_log);
    ~~~
- `save_csv` Writes an arbitrary number of arrays of doubles into a file. Column names can be specified using *`header`* parameter.
    ~~~
    save_csv("results.csv", "x,y,z", N, 3, x, y, z);
    ~~~
- `basic_graphs` Draw a simple x-y plot using gnuplot. Title can be specified and the scale of the x- and y-axis can be specified independently (linear or logarithmic)
- `basic_graphs` Draw an x-y plot using gnuplot. Title can be specified and the scale of the x- and y-axis can be specified independently (linear or logarithmic).\
    Note: the data must be provided as x-y pairs, even  if the x array remains constant, and the *`number_of_arrays`* integer must be the total number of arrays supplied, not the number of lines drawn. For example:
    ~~~
    basic_graphs(array_lengths, LOGARITHMIC, LOGARITHMIC, "Test1", 6, x1, y, x2, y2, x3, y3);
    //or, if `x` is reused
    basic_graphs(array_lengths, LOGARITHMIC, LOGARITHMIC, "Test2", 6, x, y, x, y2, x, y3);
    ~~~
- `interpolate_point` Linearly interpolates between two supplied x-y pairs.