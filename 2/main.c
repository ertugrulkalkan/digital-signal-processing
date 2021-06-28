// main.c
// Ertugrul Kalkan
// 348316

// input & output
#include <stdio.h>
// uintX_t types
#include <stdint.h>
// memory operations
#include <stdlib.h>
// sqrt
#include <math.h>

#define max(a,b) (a > b) ? a : b

#define HISTOGRAM_WIDTH 180

#define EXPECTED_BM_MAGIC_VAL 0x4d42
#define BIN_COUNT_8BIT 0x100

#define PRINT_USAGE() printf("Usage:\n" \
                            "\t%s [input BMP file] [output and histogram file]\n" \
                            "\t%s [input BMP file] [output and histogram file] [(optional) output media info file]\n" \
                            , argv[0], argv[0]);

typedef struct
{
    uint16_t bfh_magic_val;
    uint32_t bfh_file_size;
    uint16_t bfh_reserved_1;
    uint16_t bfh_reserved_2;
    uint32_t bfh_offset;
} BITMAPFILEHEADER;

void read_bfh(BITMAPFILEHEADER *bfh, FILE *fp){
    fread(&(bfh->bfh_magic_val), sizeof(bfh->bfh_magic_val), 1, fp);
    fread(&(bfh->bfh_file_size), sizeof(bfh->bfh_file_size), 1, fp);
    fread(&(bfh->bfh_reserved_1), sizeof(bfh->bfh_reserved_1), 1, fp);
    fread(&(bfh->bfh_reserved_2), sizeof(bfh->bfh_reserved_2), 1, fp);
    fread(&(bfh->bfh_offset), sizeof(bfh->bfh_offset), 1, fp);
}

void print_bfh(BITMAPFILEHEADER *bfh, FILE *file){
    fprintf(file, "0x%x\n", bfh->bfh_magic_val);
    fprintf(file, "size:%d\n", bfh->bfh_file_size);
    fprintf(file, "reserved-1:%d\n", bfh->bfh_reserved_1);
    fprintf(file, "reserved-2:%d\n", bfh->bfh_reserved_2);
    fprintf(file, "offset:%d\n", bfh->bfh_offset);
}

typedef struct
{
    uint32_t bih_size;
    uint32_t bih_width;
    uint32_t bih_height;
    uint16_t bih_planes;
    uint16_t bih_bitsperpixel;
    uint32_t bih_compression;
    uint32_t bih_image_size;
    uint32_t bih_xpm;
    uint32_t bih_ypm;
    uint32_t bih_colors_used;
    uint32_t bih_important_colors;
} BITMAPINFOHEADER;

void read_bih(BITMAPINFOHEADER *bih, FILE *fp){
    fread(&(bih->bih_size), sizeof(bih->bih_size), 1, fp);
    fread(&(bih->bih_width), sizeof(bih->bih_width), 1, fp);
    fread(&(bih->bih_height), sizeof(bih->bih_height), 1, fp);
    fread(&(bih->bih_planes), sizeof(bih->bih_planes), 1, fp);
    fread(&(bih->bih_bitsperpixel), sizeof(bih->bih_bitsperpixel), 1, fp);
    fread(&(bih->bih_compression), sizeof(bih->bih_compression), 1, fp);
    fread(&(bih->bih_image_size), sizeof(bih->bih_image_size), 1, fp);
    fread(&(bih->bih_xpm), sizeof(bih->bih_xpm), 1, fp);
    fread(&(bih->bih_ypm), sizeof(bih->bih_ypm), 1, fp);
    fread(&(bih->bih_colors_used), sizeof(bih->bih_colors_used), 1, fp);
    fread(&(bih->bih_important_colors), sizeof(bih->bih_important_colors), 1, fp);
}

void print_bih(BITMAPINFOHEADER *bih, FILE *file){
    fprintf(file, "size:%d\n", bih->bih_size);
    fprintf(file, "width:%d\n", bih->bih_width);
    fprintf(file, "height:%d\n", bih->bih_height);
    fprintf(file, "planes:%d\n", bih->bih_planes);
    fprintf(file, "bits per pixel:%d\n", bih->bih_bitsperpixel);
    fprintf(file, "compression:%d\n", bih->bih_compression);
    fprintf(file, "image size:%d\n", bih->bih_image_size);
    fprintf(file, "xpm:%d\n", bih->bih_xpm);
    fprintf(file, "ypm:%d\n", bih->bih_ypm);
    fprintf(file, "colors used:%d\n", bih->bih_colors_used);
    fprintf(file, "important colors:%d\n", bih->bih_important_colors);
}

// write the histogram as a graph to file
//
// data will be represented by ascii '@' and '>'characters
// each row of output text graph represents X axis
// each column represents Y axis
//
// NOTE: u8_sample_count_per_symbol parameter effects the output resolution
//
void write_histogram_to_file(const uint32_t * u32_histogram_ptr,
                const uint16_t u16_bin_count, const uint8_t u8_sample_count_per_symbol,
                FILE * file)
{
    fprintf(file, "NOTE: each [@] symbol represents %u data and each [>] symbol represents %u to %u data\n",
        u8_sample_count_per_symbol, (u8_sample_count_per_symbol / 2), (u8_sample_count_per_symbol - 1));
    for(int i = 0; i < u16_bin_count; i++){
        fprintf(file, "%u\t: ", i);
        int j = 0;
        while(j < u32_histogram_ptr[i]){
            int count_left = (u32_histogram_ptr[i] - j);
            if(count_left >= u8_sample_count_per_symbol)
                fprintf(file, "@");
            else if(count_left > (u8_sample_count_per_symbol / 2))
                fprintf(file, ">");
            else break;
            j += u8_sample_count_per_symbol;
        }
        fprintf(file, "\n");
    }
}

// calculate sample count of a given histogram
//
// NOTE: for a bmp file, it could be calculated or get
// other ways (from header information etc.) but 
// prefered to calculate by formula for assignment
int calculate_histogram_sample_count(const uint32_t * u32_histogram_ptr,
                const uint16_t u16_bin_count)
{
    int sample_count = 0;
    for(int i = 0; i < u16_bin_count; i++)
        sample_count += u32_histogram_ptr[i];
    return sample_count;
}

// calculate mean of a histogram
// by using the given formula
double calculate_histogram_mean(const uint32_t * u32_histogram_ptr,
                const uint16_t u16_bin_count, const uint32_t u32_sample_count)
{
    double sum_i_times_hi = 0.0;
    for(int i = 0; i < u16_bin_count; i++)
        sum_i_times_hi += ((double)i * u32_histogram_ptr[i]);
    return ((1.0 / u32_sample_count) * sum_i_times_hi);
}

// calculate variance of a histogram
// by using the given formula
double calculate_histogram_variance(const uint32_t * u32_histogram_ptr,
                const uint16_t u16_bin_count, const uint32_t u32_sample_count,const double mean)
{
    double i_minus_mean = 0.0;
    double sum_i_minus_mean_squared_times_hi = 0.0;
    for(int i = 0; i < u16_bin_count; i++){
        i_minus_mean = ((double)i - mean);
        sum_i_minus_mean_squared_times_hi += (i_minus_mean * i_minus_mean) * u32_histogram_ptr[i];
    }
    return sum_i_minus_mean_squared_times_hi * (1.0 / (u32_sample_count - 1));
}


int main(int argc, char** argv)
{
    char *bmp_file_name, *output_file_name, *media_data_file_name;
    
    media_data_file_name = NULL;

    switch(argc){
        case 4:
            media_data_file_name = argv[3];
        case 3:
            bmp_file_name = argv[1];
            output_file_name = argv[2];
            break;
        default:
            printf("wrong arg. count!\n");
            PRINT_USAGE();
            return 1;
            break;
    }

    FILE *file_input_ptr = fopen(argv[1], "rb");
    if(file_input_ptr == NULL){
        printf("file couldn't open in read mode !\n");
        PRINT_USAGE();
        return 2;
    }

    BITMAPFILEHEADER bitmap_file_header;
    // fread(&bitmap_file_header, sizeof(bitmap_file_header), 1, file_input_ptr);
    //
    // fread func. made something wrong about LSB-MSB order
    // just did it manually.
    read_bfh(&bitmap_file_header, file_input_ptr);

    // check file type
    if(bitmap_file_header.bfh_magic_val != EXPECTED_BM_MAGIC_VAL)
    {
        printf("file type is not supported !\n");
        print_bfh(&bitmap_file_header, stdout);
        PRINT_USAGE();
        return 3;
    }
    
    BITMAPINFOHEADER bitmap_info_header;
    // fread(&bitmap_info_header, sizeof(bitmap_info_header), 1, file_input_ptr);
    //
    // fread func. made something wrong about LSB-MSB order
    // just did it manually.
    read_bih(&bitmap_info_header, file_input_ptr);

    // check file format
    if(bitmap_info_header.bih_size != 40
        || bitmap_info_header.bih_bitsperpixel != 8
        || bitmap_info_header.bih_compression != 0)
    {
        printf("file format is not supported !\n");
        print_bih(&bitmap_info_header, stdout);
        PRINT_USAGE();
        return 4;
    }

    uint32_t u32_bitmap_size = (bitmap_info_header.bih_height * bitmap_info_header.bih_width) * sizeof(uint8_t);
    uint8_t *u8_bitmap_ptr = (uint8_t *)malloc(u32_bitmap_size);

    fseek(file_input_ptr, bitmap_file_header.bfh_offset, SEEK_SET);
    fread(u8_bitmap_ptr, sizeof(uint8_t), u32_bitmap_size, file_input_ptr);
    fclose(file_input_ptr);
    
    // it is easier to get it now
    // will be used to scale histogram
    // to write
    uint32_t max_value_of_histogram = 0;
    
    // uint8_t sample -> possible 256 (0x100) different values
    // sample count is variable but 32bits will be enough
    uint32_t *u32_histogram_ptr = (uint32_t *) calloc(BIN_COUNT_8BIT, sizeof(uint32_t));
    for(size_t i = 0; i < u32_bitmap_size; i++){
        // use value of each sample as an index for histogram
        u32_histogram_ptr[(u8_bitmap_ptr[i])] += 1;
        max_value_of_histogram = max(max_value_of_histogram, u32_histogram_ptr[(u8_bitmap_ptr[i])]);
    }

    FILE *file_histogram_out_ptr = fopen(output_file_name, "w");

    int sample_count = calculate_histogram_sample_count(u32_histogram_ptr, BIN_COUNT_8BIT);
    double mean = calculate_histogram_mean(u32_histogram_ptr, BIN_COUNT_8BIT, sample_count);
    double variance = calculate_histogram_variance(u32_histogram_ptr, BIN_COUNT_8BIT, sample_count, mean);
    double standard_deviation = sqrt(variance);
    
    // write data to output file
    fprintf(file_histogram_out_ptr, "mean : %f\n", mean);
    fprintf(file_histogram_out_ptr, "variance : %f\n", variance);
    fprintf(file_histogram_out_ptr, "standard deviation : %f\n", standard_deviation);


    // writing scale of histogram should be calculated
    // otherwise the output file is not understandable
    uint8_t u8_optimum_sample_count_per_symbol = max_value_of_histogram / HISTOGRAM_WIDTH;
    
    // write histogram to output file
    write_histogram_to_file(u32_histogram_ptr, BIN_COUNT_8BIT, u8_optimum_sample_count_per_symbol, file_histogram_out_ptr);
    
    fclose(file_histogram_out_ptr);

    // write bmp header as text file
    if(media_data_file_name != NULL){
        FILE *file_media_data_out_ptr;
        file_media_data_out_ptr = fopen(media_data_file_name, "w");
        print_bfh(&bitmap_file_header, file_media_data_out_ptr);
        print_bih(&bitmap_info_header, file_media_data_out_ptr);
        fclose(file_media_data_out_ptr);
    }

    return 0;
}
