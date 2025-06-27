### Narrow Tools

A set of reusable functions in C. These include 

- `linspace` Writes a set of linearly spaced points into the provided array of doubles
- `logspace` Writes a set of logarithmically spaced points into the provided array of doubles
- `save_csv` Writes an arbitrary number of arrays of doubles into a file. Column names can be specified using *`header`* parameter.
    ~~~
    save_csv("results.csv", "x,y,y2", N, 3, x, y, y2);
    ~~~
- `basic_graphs` Draw a simple x-y plot using gnuplot. Title can be specified and the scale of the x- and y-axis can be specified independently (linear or logarithmic)
- `basic_graphs` Draw an x-y plot using gnuplot. Title can be specified and the scale of the x- and y-axis can be specified independently (linear or logarithmic).\
    Note: the data must be provided as x-y pairs, even  if the x array remains constant, and the *`number_of_arrays`* integer must be the total number of arrays supplied, not the number of lines drawn. For example:
    ~~~
    basic_graphs(array_lengths, LOGARITHMIC, LOGARITHMIC, "Test1", 6, x1, y, x2, y2, x3, y3);
    //or
    basic_graphs(array_lengths, LOGARITHMIC, LOGARITHMIC, "Test2", 6, x, y, x, y2, x, y3);
    ~~~
- `interpolate_point` Linearly interpolates between two supplied x-y pairs.