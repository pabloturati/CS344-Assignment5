# CS344-Assignment5
CS344-Assignment 5: One-Time Pads\
Author: Pablo Turati - turatip@oregonstate.edu\
Licence: MIT. Refer to `LICENSE` for details.

## Project Description

`One-Time Pads` 

Application to encrypt and decrypt using 4 c programs that interact thru a network.

## Compile instructions

This project uses GNU99 compile standards for building the executable.

### Compile instructions using automated shell script

This application includes a bash script to build and deploy the runner (compileApp.sh).  To use it, first make sure that `Makefile` has execution persmissions by excuting the following command:

`$ chmod +x Makefile`

You may execute the automated compile instructions by running from the project root:

`$ ./Makefile`

This will generate the binary executable files named: `enc_server`, `enc_client`, `dec_server`, `dec_client` and `keygen `in the root of the project. Refer to section Compile Commands section below on how to run the application and pass parameters.

### (OPTIONAL) Compile and run in single step (Compile run commands)

`Makefile` accepts the following flags in which case it will compile, generate the executable and immediately run it.  To do this run:

`$ ./compileApp.sh -<FLAG> <PARAM>`

Param is optional depending on the flag

Where following flags can be passed to the compileApp.sh shell script:
    - s - executable getting standard in from input1 testfile >run
    - n - generate key for a certain filename (arg = filename)
    - k - kills both server processes
    - q - do a test run with a testfile. Arg is number 1 thru 5 for each plaintext file
    - f - to do a full clean of the project output and compile files
    - u - tests all plaintext files in path plaintext*
    - z - runs grading script (`./p5testingscript`)

## Version control

 Link to the repository is available [here](https://github.com/pabloturati/CS344-Assignment5).  However, due to this being an Oregon State University assignment, it will remain private and public access will become available after April, 2021.