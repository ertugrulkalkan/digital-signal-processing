/*
 * wavefile.h
 * Final Assignment
 * Ertugrul Kalkan
 * 348316
 * 
 * 06/2021
 */

#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define RIFF 0x46464952 // {'F', 'F', 'I', 'R'}
#define WAVE 0x45564157 // {'E', 'V', 'A', 'W'}
#define fmt_ 0x20746d66 // {' ', 't', 'm', 'f'}
#define data 0x61746164 // {'a', 't', 'a', 'd'}

#define NOT_COMPRESSED_AUDIO_FORMAT 0x01

/*
 * First 12 bytes of WAV file
 */
typedef struct {
  
  /*
   * must be equal to [RIFF]
   */
  uint32_t chunk_id;
  
  /*
   * [total bytes in this file] - 4(chunk_id) - 4(chunk_size)
   */
  uint32_t chunk_size;
  
  /*
   * must be equal to [WAVE]
   */
  uint32_t format;

} chunk_descriptor_t;

/*
 * Format Chunk
 */
typedef struct {

  /*
   * must be equal to [fmt_]
   */
  uint32_t chunk_id;
  
  /*
   * [total bytes in format_subchunk_t] - 4(chunk_id) - 4(chunk_size)
   */
  uint32_t chunk_size;

  /*
   * Any value other than 1 indicates some compression
   */
  uint16_t audio_format;
  
  /*
   * number of channels [1 = MONO, 2 = STEREO etc.]
   */
  uint16_t num_channels;
  
  /*
   * samples per second
   */
  uint32_t sample_rate;

  /*
   * bytes per second
   */
  uint32_t byte_rate;

  /*
   * size of one sample for all channels
   */
  uint16_t block_align;

  /*
   * bits per one sample for each channels
   */
  uint16_t bits_per_sample;

} format_subchunk_t;

typedef struct {

  /*
   * must be equal to [data]
   */
  uint32_t chunk_id;

  /*
   * bytes of real data
   */
  uint32_t chunk_size;
  
} data_subchunk_header_t;

/*
 * commonly used 44 bytes header
 */
typedef struct {
  chunk_descriptor_t chunk_descriptor;
  format_subchunk_t format_subchunk;
  data_subchunk_header_t data_subchunk_header;
} wav_file_header_t;

void print_chunk_descriptor(chunk_descriptor_t *ch);

void print_format_subchunk(format_subchunk_t *fsc);

void print_data_subchunk_header(data_subchunk_header_t *dsh);

void print_wav_file_header(wav_file_header_t *wfh);

void read_wavefile(wav_file_header_t *wfh, FILE *f);