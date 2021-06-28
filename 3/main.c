// main.c
// Ertugrul Kalkan
// 348316

#include <stdio.h>   // input & output
#include <stdlib.h>  // atof & atoi
#include <math.h>    // pow, exp

// macros
#define max(a,b) (a>b) ? a:b
#define min(a,b) (a<b) ? a:b

// PI
const double PI = 3.141592653589793238463;

#define PRINT_USAGE() printf("Usage:\n" \
                            "\t%s [double->mean] [double->std_deviation] [double->X] [size_t->iteration (optional)]\n" \
                            , argv[0]);

// PDF of gauss distrubution
double pdf_gauss_dist(const double mean,
                const double standard_deviation,
                const double X);

// gets trapezoidal integral of PDF func. between
// x1 and x2 vals.
double trapezoidal_integral_pdf_gauss_dist(const double mean,
                const double standard_deviation,
                const double x1,
                const double x2,
                const size_t iteration);


// gets CDF value of a given X of
// gauss dist. PDF
double cdf_gauss_dist(const double mean,
                const double standard_deviation,
                const double X,
                const size_t iteration);

int main(int argc, char** argv)
{
    double mean;
    double std_dev;
    double x;
    size_t iteration = 10000;

    if((argc != 5) && (argc != 4)){
        PRINT_USAGE();
        return 1;
    }

    mean = (double) atof(argv[1]);
    std_dev = (double) atof(argv[2]);
    x = (double) atof(argv[3]);

    if(argc == 5){
        iteration = (size_t) atoi(argv[4]);
    }

    printf("mean: %f, std.deviation: %f, X: %f, iteration: %ld\n", mean, std_dev, x, iteration);

    double cdf = cdf_gauss_dist(mean, std_dev, x, iteration);
    printf("CDF(%f): %f\n", x, cdf);

    return 0;
}

double pdf_gauss_dist(const double mean,
                const double standard_deviation,
                const double X)
{
    // function
    return ((1.0 / (standard_deviation * sqrt(2.0 * PI))) * exp((-1.0 * ((X - mean) * (X - mean))) / (2.0 * pow(standard_deviation, 2))));
}

double trapezoidal_integral_pdf_gauss_dist(const double mean,
                const double standard_deviation,
                const double x1,
                const double x2, 
                const size_t iteration)
{
    // l1 is the start
    // l2 is the end
    // mid, fl1, fl2 will be set each iteration
    // step is the width
    double l1, l2, step, sum;
    l1 = min(x1,x2);
    l2 = max(x1,x2);
    step = (l2 - l1) / iteration;

    sum = 0.0;
    for(size_t i = 0; i < iteration; i++){
        
        // trapezoid
        double i_l1 = pdf_gauss_dist(mean, standard_deviation, l1);
        double i_l2 = pdf_gauss_dist(mean, standard_deviation, l1 + step);
        double i_tz = (i_l1 + i_l2) / 2;
        
        // add
        sum += (i_tz * step);

        // increase for next iteration
        l1 += step;
    }

    return sum;
}

double cdf_gauss_dist(const double mean,
                const double standard_deviation,
                const double X,
                const size_t iteration)
{   
    double Xlimit_min = mean - (6.0 * standard_deviation);
    double Xlimit_max = mean + (6.0 * standard_deviation);

    if(X < Xlimit_min)
        // too close to zero
        return 0.0;
    else if(X > Xlimit_max)
        // too close to one
        return 1.0;
    else
        return trapezoidal_integral_pdf_gauss_dist(mean, standard_deviation, Xlimit_min, X, iteration);
}