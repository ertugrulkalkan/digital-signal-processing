Assignment -3-
Ertugrul Kalkan
348316@ogr.ktu.edu.tr

requires GNU-make and GNU-gcc

Calculate CDF of a GAUSS DIST.
by using trapezoidal integral
from PDF for a single value.

compile:
    $ make

run (example):

    $ main.bin 0.0 1.0 0.5

    or

    $ main.bin 0.0 1.0 0.5 500 

    |---------|---|---|---|---|
        bin     A   S   X   I

    bin: binary file name
    A: Mean of Gauss Dist.
    S: Std. Deviation of Gauss Dist.
    X: X value to calculate CDF
    I: (optional & default=1000) integral iteration
 