# CS344-Assignment5
CS344-Assignment 5: One-Time Pads\
Author: Pablo Turati - turatip@oregonstate.edu\
Licence: MIT. Refer to `LICENSE` for details.

## Project Description

`One-Time Pads` 

is a simple multi-threaded program that performs the following operations:

- calls an Input Thread, reads in lines of characters from the standard input
- calls a Line Separator Thread, replacing every line separator in the input by a space.
- calls a Plus Sign thread, replacing every pair of plus signs, i.e., "++", by a "^"
- calls the Output Thread, writes this processed data to standard output as lines of exactly 80 characters.

Intercommunication between thereads is done by using the Producer-Consumer approach.

## Compile instructions

This project uses GNU99 compile standards for building the executable.

### Option 1.  Using the included compile script

This application includes a bash script to build and deploy the runner (compileApp.sh).  To use it, first make sure that `compileApp.sh` has execution persmissions by excuting the following command:

`$ chmod +x compileApp.sh`

You may execute the automated compile instructions by running from the project root:

`$ ./compileApp.sh`

This will generate a binary executable file named `line_processor` in the root of the project. Refer to section Compile Commands section below on how to run the application and pass parameters.

### Option 2.  Individual commands

Refer to the contents of `compileApp.sh` for a detailed command list on how to manually compile using command line.  Notice that this project was designed to be compiled with GNU99 standards.

### Option 3.  Compile and run in single step (Compile run commands)

`compileApp.sh` accepts the flag `-e`, in which case it will compile, generate the executable and immediately run it.  To do this run:

`$ ./compileApp.sh -<FLAG>`

Where following flags can be passed to the compileApp.sh shell script:
    -m : (manual) run executable to take input from stdin and output from stdout
    -e : (execute) run executable getting standard in from default input1 (located in ./testfiles/testInputFiles/input1.txt)
    -t : (test) run executable and then run test script (see README section `Testing` for more details)
    -v : (valgrind) run executable with valgrind leak analysis

## Run procedure

Once the application executable has been created `line_processor`. It can be run as follows:

### To feed input from and print output to the terminal window, run the following command
`$ ./line_processor`

### To feed input from a file and to print output to the terminal window, run the following command

`$ ./line_processor < path/to/inpuFile`

Note that if input file does not exist, causes an error.

### To feed input from the terminal window and to print output to file, run the following command

`$ ./line_processor > path/to/outputfile`

Note that if outpufile does not exist, the file gets created.

### To feed input from a file and to print output to file, run the following command

`$ ./line_processor < path/to/inpuFile > path/to/outputfile`

Note that if outpufile does not exist, the file gets created.

To compile and run in a single step run: `$ compileApp.sh -e`

## Testing procedure

This application comes with a built in testing script `testScript.sh`.  To run the script, make sure that `testScript.sh` has execution permissions by running the following command from the project root directory.

`$ chmod +x ./testScript.sh`

This project contains 3 test files with their expected outpu in the `testFiles` directory.  

### To compile and run the test script in a single step

To run the compiler and the test script in a single operation, run the compile app script with the `-t` flag:

`./compileApp.sh -t`

### To compile and then run the test script in a separate step

To first run the compiler and then run the testing script

`./compileApp.sh`

If successful, the `line_processor` binary file will be generated in the project root and then run:

`./testScript.sh`

### Additional information about testing

Testing can be expanded by adding files to the test folders and registering them in the `testScript.sh` file.  File must include an input and an expecte output.

The tests Script uses the shell built in command `cmp` to perform the testing.

## Additional resources

## Project File structure
```
Project root
├── .gitignore
├── Makefile
├── dec_client.c
├── dec_server.c
├── enc_client.c
├── enc_server.c
├── LICENSE
├── README.md
├── constants
│   ├── constants.c
│   └── constants.h
├── sharedMethods
│   ├── sharedMethods.c
│   └── sharedMethods.h
├── sourceFiles
│   ├── dataProcessingMethods.c
│   └── dataProcessingMethods.h
└── sourceFiles
```

## Version control

 Link to the repository is available [here](https://github.com/pabloturati/CS344-Assignment5).  However, due to this being an Oregon State University assignment, it will remain private and public access will become available after April, 2021.