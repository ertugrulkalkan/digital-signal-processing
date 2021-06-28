/*
 * main.c
 * Final Assignment
 * Ertugrul Kalkan
 * 348316
 * 
 * 06/2021
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "wavefile.h"

// dump raw.txt and filtered.txt files flag
#define DUMPFILE 1

// constants
#define MAX_FILTER_SIZE 1023
#define INPUT_BUFFER_SIZE 255

#define PRINT_USAGE() printf("Usage :\n\t%s [wav_file] [filter_file]\n", argv[0])

// check the string is a float
int is_float(char *s, size_t len);

// convolve
int16_t* FIR(int16_t *signal, const size_t signal_len, float *filter, const size_t filter_len);

// main
int main(int argc, char **argv) {
  char *wav_filename = argv[1];
  char *filter_filename = argv[2];

  FILE *wav_file_in;
  FILE *filter_file_in;

  wav_file_header_t wav_header;

  char input_buffer[INPUT_BUFFER_SIZE];

  int16_t **channels;
  size_t sample_count_per_channel;
  char *sample_buffer;

  /*
   * OPEN FILE
   */
  wav_file_in = fopen(wav_filename, "rb");
  
  if(wav_file_in == NULL) {
    PRINT_USAGE();
    exit(EXIT_FAILURE);
  }

  /*
   * READ FILE
   */
  read_wavefile(&wav_header, wav_file_in);
  print_wav_file_header(&wav_header);

  /*
   * CALCULATE SAMPLE COUNT
   */ 
  sample_count_per_channel = 
    wav_header.data_subchunk_header.chunk_size / wav_header.format_subchunk.block_align;
    
  printf("[MESSAGE]There are %lu sample for each %u channels...\n",
    sample_count_per_channel, wav_header.format_subchunk.num_channels);

  sample_buffer = calloc(wav_header.format_subchunk.block_align, sizeof(char));
  channels = calloc(wav_header.format_subchunk.num_channels, sizeof(int16_t*));
  for(int i = 0; i < wav_header.format_subchunk.num_channels; i++) {
    channels[i] = calloc(sample_count_per_channel, sizeof(int16_t));
  }

  /*
   * READ DATA
   */
  for(int i = 0; i < sample_count_per_channel; i++) {
    fread(sample_buffer, sizeof(char), wav_header.format_subchunk.block_align, wav_file_in);
    for(int j = 0; j < wav_header.format_subchunk.num_channels; j++) {
      if(wav_header.format_subchunk.bits_per_sample == 16){
        channels[j][i] = ((int16_t *)sample_buffer)[j];
      } else if(wav_header.format_subchunk.bits_per_sample == 8) {
        channels[j][i] = ((uint8_t *)sample_buffer)[j];
      }
    }
  }

  /*
   * CHECK SAMPLE FREQUENCY OF INPUT FILE
   */
  if(wav_header.format_subchunk.sample_rate != 22050) {
    input_buffer[0] = '\0';
    print_format_subchunk(&(wav_header.format_subchunk));
    printf("---------------------------------------------------------------\n"\
      "[WARNING]WAV file sample rate is not 22050 Hz...\n"\
      "Please make sure the FIR filter that you chose\ndesigned for this spesific sample rate (%d)\n",
      wav_header.format_subchunk.sample_rate);
    while(input_buffer[0] != 'y' && input_buffer[0] != 'n') {
      printf("Do you want to proceed? [y/n]\n");
      scanf("%s", input_buffer);
    }
    if(input_buffer[0] == 'n')
      exit(EXIT_SUCCESS);
  }


#if DUMPFILE
  /*
   * WRITING THE raw.txt FILE
   */
  FILE *dump_file_out;
  dump_file_out = fopen("./out/raw.txt", "w");

  if(dump_file_out != NULL){
    for(int i = 0; i < wav_header.format_subchunk.num_channels; i++) {
      for(int j = 0; j < sample_count_per_channel; j++) {
        fprintf(dump_file_out,"%hd\n",channels[i][j]);
    }
  }
  } else {
    printf("[WARNING]Dump file could not open...\n");
  }
#endif /* DUMPFILE */

  /*
   * OPEN FILTER FILE
   */
  filter_file_in = fopen(filter_filename, "r");
  float *fir_filter;
  size_t fir_filter_len;

  if(filter_file_in == NULL) {
    printf("[ERROR]Filter file could not open...\n");
    PRINT_USAGE();
    exit(EXIT_FAILURE);
  }

  fir_filter = calloc(MAX_FILTER_SIZE, sizeof(float));

  /*
   * READ FILTER FILE
   */
  char *line;
  size_t len;
  size_t index = 0;
  fgets(input_buffer, INPUT_BUFFER_SIZE, filter_file_in);
  while(!feof(filter_file_in)){
    line = strtok(input_buffer, "\n");
    len = strlen(line);
    if(len > 2 && is_float(line, len)){
      fir_filter[index++] = atof(line);
    } else {
      printf("[WARNING]Invalid line in filter file...\n");
    }
    fgets(input_buffer, INPUT_BUFFER_SIZE, filter_file_in);
  }
  fir_filter_len = index;

  /*
   * MAKE FILTERING
   */
  int16_t **fil_channels;
  fil_channels = calloc(sizeof(int16_t*), wav_header.format_subchunk.num_channels);
  for(int i = 0; i < wav_header.format_subchunk.num_channels; i++) {
    fil_channels[i] = FIR(channels[i], sample_count_per_channel, fir_filter, fir_filter_len);
  }

#if DUMPFILE
  /*
   * WRITING THE filtered.txt FILE
   */
  fclose(dump_file_out);
  dump_file_out = fopen("./out/filtered.txt", "w");

  if(dump_file_out != NULL){
    for(int i = 0; i < wav_header.format_subchunk.num_channels; i++) {
      for(int j = 0; j < sample_count_per_channel; j++) {
        fprintf(dump_file_out,"%hd\n",fil_channels[i][j]);
    }
  }
  } else {
    printf("[WARNING]Dump file could not open...\n");
  }
#endif /* DUMPFILE */


  /*
   * OPEN generated.wav FILE
   */
  FILE *out_wav_file;
  out_wav_file = fopen("./out/generated.wav","wb");

  /*
   * WRITE generated.wav FILE DATA
   */
  if(out_wav_file != NULL) {
    fwrite(&wav_header,sizeof(wav_file_header_t),1,out_wav_file);
    for(size_t i = 0; i < sample_count_per_channel; i++) {
      for(size_t j = 0; j < wav_header.format_subchunk.num_channels; j++) {
        if(wav_header.format_subchunk.bits_per_sample == 16) {
          fwrite(&(fil_channels[j][i]), sizeof(int16_t), 1, out_wav_file);
        } else if(wav_header.format_subchunk.bits_per_sample == 8) {
          fwrite(&(fil_channels[j][i]), sizeof(uint8_t), 1, out_wav_file);
        }
      }
    }
  }
  /*
   * FREE THE MEMORY
   */
  for(int i = 0; i < wav_header.format_subchunk.num_channels; i++) {
    free(channels[i]);
    free(fil_channels[i]);
  }
  free(fil_channels);
  free(channels);
  free(sample_buffer);
  free(fir_filter);

  return 0;
}

/*
 * CONVOLUTION OPERATION
 */
int16_t* FIR(int16_t *signal, const size_t signal_len,
              float *filter, const size_t filter_len) {
  float *result;
  size_t i;

  result = calloc(signal_len + filter_len - 1, sizeof(float));

  for (i = 0; i < signal_len + filter_len - 1; i++) {
    size_t jmin, jmax, j;

    result[i] = 0.0;

    jmin = (i >= filter_len - 1) ? i - (filter_len - 1) : 0;
    jmax = (i < signal_len - 1) ? i : signal_len - 1;

    for (j = jmin; j <= jmax; j++) {
      result[i] += signal[j] * filter[i - j];
    }
  }

  // get rid of first invalid samples
  float *r_data = &result[(filter_len - 1) / 2];

  int16_t *filtered_samples = calloc(signal_len, sizeof(int16_t));
  for(i = 0; i < signal_len; i++) {
    filtered_samples[i] = (int16_t)r_data[i];
  }

  free(result);
  return filtered_samples;
}

int is_float(char *s, size_t len) {
  regex_t rexp;
  regmatch_t regmatch;
  int rc,ret;
  
  rc = regcomp(&rexp, "[+-]?([0-9]*[.])?[0-9]+", REG_ICASE | REG_EXTENDED);
  if(rc != 0){
    printf("[ERROR]Regex error...\n");
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