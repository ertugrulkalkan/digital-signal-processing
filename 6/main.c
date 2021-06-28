// main.c
// Ertugrul Kalkan
// 348316

// input & output
#include <stdio.h>
#include <stdint.h>
// memory operations
#include <stdlib.h>
// string ops
#include <string.h>
// sqrt
#include <math.h>
// regex
#include <regex.h>

#define SIGNAL_BUFFER_SIZE 1024
#define INPUT_BUFFER_SIZE 64

#define PI 3.1415926

#define PRINT_USAGE() printf("Usage:\n" \
                            "\t%s [input file]\n" \
                            , argv[0]);

int is_float(char *s, size_t len);

int main(int argc, char** argv)
{
    FILE *signal_file;
    char *input_buffer = calloc(64, sizeof(char));
    char * line;
    size_t len;
    float *sample_buffer = calloc(SIGNAL_BUFFER_SIZE, sizeof(float));
    size_t sample_count;
    size_t index = 0;

    if(argc != 2){
        PRINT_USAGE();
        exit(EXIT_FAILURE);
    }

    signal_file = fopen(argv[1], "r");
    if(signal_file == NULL){
        PRINT_USAGE();
        exit(EXIT_FAILURE);
    }

    fgets(input_buffer, INPUT_BUFFER_SIZE, signal_file);
    while(!feof(signal_file)){
        line = strtok(input_buffer, "\n");
        len = strlen(line);
        if(is_float(line, len)){
            sample_buffer[index++] = atof(line);
        } else {
            printf("invalid line\n");
        }
        fgets(input_buffer, INPUT_BUFFER_SIZE, signal_file);
    }

    sample_count = index;

    // reals
    float * Rs = malloc(sample_count * sizeof(float));
    // imags
    float * Is = malloc(sample_count * sizeof(float));
    // powers
    float * Ps = malloc(sample_count * sizeof(float));

    // needed indices
    size_t i,j;

    // data loop
    for(i = 0; i < sample_count; i++){
        Rs[i] = 0.0f;
        Is[i] = 0.0f;
        for(j = 0; j < sample_count; j++)
            Rs[i] += sample_buffer[j] * cos(2.0f * i * j * PI / sample_count);
        for(j = 0; j < sample_count; j++)
            Is[i] -= sample_buffer[j] * sin(2.0f * i * j * PI / sample_count);
        Ps[i] = powf(Rs[i], 2.0f) + powf(Is[i], 2.0f);
    }

    printf("\tsample***\t|\treals***\t|\timags***\t|\tpowers***\n");
    for(i = 0; i < sample_count; i++)
        printf("\t%f\t|\t%f\t|\t%f\t|\t%f\n", sample_buffer[i], Rs[i], Is[i], Ps[i]);

    return 0;
}

// check if a string is parsable as a float or not
int is_float(char *s, size_t len)
{
    regex_t rexp;
    regmatch_t regmatch;
    int rc,ret;
    
    rc = regcomp(&rexp, "[+-]?([0-9]*[.])?[0-9]+", REG_ICASE | REG_EXTENDED);
    if(rc != 0){
        printf("error complile regex\n");
        exit(EXIT_FAILURE);
    }

    rc = regexec(&rexp, s, 1, &regmatch, 0);
    if(rc != 0 || regmatch.rm_so != 0 || regmatch.rm_eo != len){
        ret = 0;
    } else {
        ret = 1;
    }

    regfree(&rexp);
    // if the input string is a valid float number
    return ret;
}