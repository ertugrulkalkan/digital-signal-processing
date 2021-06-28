# Final Assignment
## Ertugrul Kalkan
### 348316@ogr.ktu.edu.tr

requires GNU-make and GNU-g++
tools requires python3 and mathplotlib

compile:

    $ make

run:

    $ ./main.bin [wav file] [filter file(as text)]

clean the directory:

    $ make clean

# WAV FILE

 * must be 16bits or 8 bits per sample
 * no compression

# FILTER FILE

 * must contain all coefficients of FIR filter (program does not mirror it)
 * must contain one coefficient per line as float
 * no scientific notation supported
 * must contain one and ONLY ONE newline character at the end of filter file

# INCLUDED SAMPLE FILES

The program is testable by included sample files.

## Filters
  * there are 4 different filters designed using
  http://t-filter.engineerjs.com
  online tool under the path "filters"
  * names of filter files contains building parameters of them
  * graphs of filters could be found under the path "filters/graphs"

## Samples
  * there are 4 different wav samples
  * names of wav files contains most necessary information about them

## Tools
 * to use python scripts, you should install python 3 and mathplotlib library.
 * differ.py plots input wav file samples and output wav file samples in the same canvas to see the differance
 * single.py plots only the input file for single channel file
 * dual_channel.py plots only the input file for dual channel file
 * you must compile and run the main program before use them.

## Output Files
 * When you run the program correctly, it generates 3 files in "./out/" path:
   * generated.wav : FIR filtered output wav file
   * raw.txt : samples read from source file
   * filtered.txt : samples wrote to the destination file

## Limitations
 * Length limit of filter is set to 1023, can be changed from main.c (definition)

## copy-paste

    $ make

    $ ./main.bin samples/8bit-1channels-22257sps.wav filters/freq22257-pass0t3800-stop4500t11128.txt

    $ ./main.bin samples/16bit-2channels-44100sps.wav filters/freq44100-pass0t3800-stop4500t22050.txt

    $ ./main.bin samples/16bit-1channels-22050sps.wav filters/freq22050-pass0t3800-stop4500t8000.txt

    $ ./main.bin samples/16bit-1channels-22050sps-pinkpanther.wav filters/freq22050-pass0t3800-stop4500t11025.txt