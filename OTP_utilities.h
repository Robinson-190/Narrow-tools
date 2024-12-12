#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdarg.h>

#define LINEAR 0
#define LOGARITHMIC 1

/*
One-Trick Pony functions.
Generally only useful for exactly what they do.
*/

void linspace(double start,double end, int length, double array_to_fill[length])
{
    double dx = (end-start)/((double) length-1.0);
    array_to_fill[0]=start;

    if(length>=2)
    {
        array_to_fill[length-1]=end;
        for (int i = 1; i < length-1; i++)
        {
            array_to_fill[i]=array_to_fill[i-1]+dx;
        }
    }
}

void logspace(double log10_start,double log10_end, int length, double array_to_fill[length])
{
    double dx = pow(10.0,(log10_end-log10_start)/((double) length-1.0));
    array_to_fill[0]=pow(10.0,log10_start);

    if(length>=2)
    {
        array_to_fill[length-1]=pow(10.0,log10_end);
        for (int i = 1; i < length-1; i++)
        {
            array_to_fill[i]=array_to_fill[i-1]*dx;
        }
    }
}


//Save csv files for an arbitrary (but specified) amount of arrays of doubles. All arrays must be of the same length. 
void save_csv(char *filename,char *header,int array_length,int number_of_arrays,...)
{
    // Declaring pointer to the
    // argument list
    va_list ptr;
    // Initializing argument to the
    // list pointer
    va_start(ptr, number_of_arrays);

    //array of addresses of separate arrays to write to
    double *data[number_of_arrays];
    //save start address of each input argument to array
    for (int i = 0; i < number_of_arrays; i++)
    {
        data[i]=va_arg(ptr, double *);//pass each double array address to an array for storage
    }
    va_end(ptr);
    FILE *file_ptr;
    file_ptr = fopen(filename, "w");
    if (file_ptr == NULL) {
        printf("Error opening file \"%s\"!",filename);
    }
    else{
        fprintf(file_ptr,"%s\n",header);
        //For each entry, write a,b,c,...\n to file
        for (int j = 0; j < array_length; j++)
        {
            for (int i = 0; i < number_of_arrays; i++)
            {
                fprintf(file_ptr,"%e",*(data[i]+j));       
                if(i+1<number_of_arrays) fprintf(file_ptr,",");// don't print comma for last entry
            }
            fprintf(file_ptr,"\n");
        }
        fclose(file_ptr);
    }
}

void basic_graphs(int* array_lengths, int x_axis_scale, int y_axis_scale, char* title,int number_of_arrays,...)
{
    // Declaring pointer to the
    // argument list
    va_list ptr;
    // Initializing argument to the
    // list pointer
    va_start(ptr, number_of_arrays);
    //array of addresses of separate arrays to write to
    double *data[number_of_arrays];
    //save start address of each input argument to array
    for (int i = 0; i < number_of_arrays; i++)
    {
        data[i]=va_arg(ptr, double *);//pass each double array start address to array for storage.
    }
    va_end(ptr);

    FILE *gnuplot = popen("gnuplot -p", "w");
    fprintf(gnuplot, "set style data linespoints\n");
    //fprintf(gnuplot, "set nokey\n");
    fprintf(gnuplot, "set title '%s'\n",title);
    if(x_axis_scale == LOGARITHMIC)
    {
        fprintf(gnuplot, "set logscale x 10\n");
    }
    if(y_axis_scale == LOGARITHMIC)
    {
        fprintf(gnuplot, "set logscale y 10\n");
    }    
    fprintf(gnuplot, "plot '-' title \"Dataset %d\"",0);

    for (int i = 1; i < number_of_arrays/2; i++)
    {
        fprintf(gnuplot, ",'-'title \"Dataset %d\"",i);
    }
    fprintf(gnuplot, "\n");

    for (int i = 0; i < number_of_arrays/2; i++)//IMPORTANT: 
    {
        double* x_ptr = data[2*i];
        double* y_ptr = data[2*i+1];
        for (int j = 0; j < array_lengths[i]; j++)
        {
            fprintf(gnuplot,"%e %e\n",*(x_ptr+j),*(y_ptr+j));
        }
        fprintf(gnuplot,"e\n");
    }
    fprintf(gnuplot,"\n");
    fflush(gnuplot);
}


//Draw a basic graph using gnuplot.
void basic_graph(int array_length, double x_values[array_length], double y_values[array_length], int x_axis_scale, int y_axis_scale, char* title)
{
    FILE *gnuplot = popen("gnuplot -p", "w");
    
    fprintf(gnuplot, "set style data linespoints\n");
    fprintf(gnuplot, "set nokey\n");
    fprintf(gnuplot, "set title '%s'\n",title);

    if(x_axis_scale == LOGARITHMIC)
    {
        fprintf(gnuplot, "set logscale x 10\n");
    }
    if(y_axis_scale == LOGARITHMIC)
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
}
