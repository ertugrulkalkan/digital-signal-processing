/*
 * wavefile.c
 * Final Assignment
 * Ertugrul Kalkan
 * 348316
 * 
 * 06/2021
 */

#include "wavefile.h"

void print_chunk_descriptor(chunk_descriptor_t *ch) {
  printf("Chunk Descriptor:\n"\
        "\tchunkID: %x\n"\
        "\tchunkSize: %d\n"\
        "\tFormat: %x\n",
        ch->chunk_id,
        ch->chunk_size,
        ch->format);
}

void print_format_subchunk(format_subchunk_t *fsc) {
  printf("Format Subchunk:\n"\
        "\tchunkID: %x\n"\
        "\tchunkSize: %d\n"\
        "\taudioFormat: %d\n"\
        "\tnumChannels: %d\n"\
        "\tsampleRate: %d\n"\
        "\tbyteRate: %d\n"\
        "\tblockAlign: %d\n"\
        "\tbitsPerSample: %d\n",
        fsc->chunk_id,
        fsc->chunk_size,
        fsc->audio_format,
        fsc->num_channels,
        fsc->sample_rate,
        fsc->byte_rate,
        fsc->block_align,
        fsc->bits_per_sample);
}

void print_data_subchunk_header(data_subchunk_header_t *dsh) {
  printf("Data Chunk Header:\n"\
        "\tchunkID: %x\n"\
        "\tchunkSize: %d\n",
        dsh->chunk_id,
        dsh->chunk_size);
}

void print_wav_file_header(wav_file_header_t *wfh) {
  printf("WAV FILE:\n");
  print_chunk_descriptor(&(wfh->chunk_descriptor));
  print_format_subchunk(&(wfh->format_subchunk));
  print_data_subchunk_header(&(wfh->data_subchunk_header));
}

void read_wavefile(wav_file_header_t *wfh, FILE *f) {
  
  fread((char *)&(wfh->chunk_descriptor), sizeof(chunk_descriptor_t), 1, f);
  if(wfh->chunk_descriptor.chunk_id != RIFF || wfh->chunk_descriptor.format != WAVE){
    print_chunk_descriptor(&(wfh->chunk_descriptor));
    printf("[ERROR]Not a wav file or unsupported format...\n");
    exit(EXIT_FAILURE);
  }

  fread((char *)&(wfh->format_subchunk), sizeof(format_subchunk_t), 1, f);
  if(wfh->format_subchunk.chunk_id != fmt_) {
    print_format_subchunk(&(wfh->format_subchunk));
    printf("[ERROR]Not a valid format subchunk...\n");
    exit(EXIT_FAILURE);
  }

  if(wfh->format_subchunk.audio_format != NOT_COMPRESSED_AUDIO_FORMAT) {
    print_format_subchunk(&(wfh->format_subchunk));
    printf("[ERROR]Unsupported audio format...\n");
    exit(EXIT_FAILURE);
  }
  fread((char *)&(wfh->data_subchunk_header), sizeof(data_subchunk_header_t), 1, f);
}
