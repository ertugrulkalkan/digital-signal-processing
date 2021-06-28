// main.cpp
// Ertugrul Kalkan
// 348316

// input & output
#include <iostream>
// precision
#include <iomanip>
// file operatoins
#include <fstream>
// parse operations etc...
#include <string>
#include <regex>
// squareroot
#include <cmath>

void printUsage(char * cp_Name);
bool isFloat(std::string &s_float);
float calculateAverage(const float f_sample, const float f_average, const size_t n_sampleCount);
float calculateVariance(const float f_sumOfSamples, const float f_sumOfSquaresOfSamples, const size_t n_sampleCount);


int main(int argc, char** argv)
{

    if(argc != 2){
        printUsage(argv[0]);
        exit(EXIT_FAILURE);
    }
    
    std::cout << std::setprecision(6) << std::fixed;
    std::ifstream file;
    file.open(argv[1]);

    if(!file.is_open()){
        printUsage(argv[0]);
        exit(EXIT_FAILURE);
    }

    std::string s_line;

    size_t n_sampleCount = 0,
            n_lineNumber = 0;
    float f_sample,
            f_average,
            f_variance,
            f_standardDeviation,
            f_sumOfSamples = 0.0,
            f_sumOfSquaresOfSamples = 0.0;

    while (!file.eof())
    {
        file >> s_line;
        n_lineNumber++;
        if(isFloat(s_line)){
            f_sample = std::stof(s_line);
            n_sampleCount++;
            f_sumOfSamples += f_sample;
            f_sumOfSquaresOfSamples += (f_sample * f_sample);
            f_average = calculateAverage(f_sample, f_average, n_sampleCount);
            f_variance = calculateVariance(f_sumOfSamples, f_sumOfSquaresOfSamples, n_sampleCount);
            f_standardDeviation = sqrt(f_variance);
            std::cout << "sample : " << f_sample
                    << "\taverage : "<< f_average
                    << "\tstandard deviation : "<< f_standardDeviation
                    << "\tvariance : "<< f_variance << std::endl;
        } else {
            std::cout << "file error at line " << n_lineNumber << std::endl;
        }
        s_line = "";
    }
    
    file.close();
    return 0;
}

void printUsage(char * cp_Name)
{
    std::cout << "usage:" << std::endl
            << "\t" << cp_Name << " [filename or path]" << std::endl ;
}

bool isFloat(std::string &s_float)
{
    std::regex rexp("[+-]?([0-9]*[.])?[0-9]+");
    // if the input string is a valid float number
    return(std::regex_match(s_float, rexp));
}

float calculateAverage(const float f_sample, const float f_average, const size_t n_sampleCount)
{
    // if not the first sample calculate
    // if it is just return it 
    return (n_sampleCount > 1) ?
        (((f_average * (n_sampleCount - 1)) + f_sample) / (n_sampleCount))
        : (f_sample);
}

float calculateVariance(const float f_sumOfSamples, const float f_sumOfSquaresOfSamples, const size_t n_sampleCount)
{
    // if not the first sample calculate
    // if it is just return zero
    return (n_sampleCount > 1) ?
        ((1.0 / (n_sampleCount - 1)) * (f_sumOfSquaresOfSamples - ((1.0 / n_sampleCount) * (f_sumOfSamples * f_sumOfSamples))))
        : (0.0);
}
