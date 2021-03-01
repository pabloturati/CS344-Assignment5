#! /bin/bash

# Build data
preCompilePackage="component_archive"
# executableFilename="line_processor"
# mainFilename="main.c"
testFiles="
  mykey
  "

# Components
# declare -a componentList=(
#   "constants"
#   "dataProcessingMethods"
#   "bufferHandlers"
#   "threadHandlerMethods"
# )

# Cleans previously compiled files and created folders
function preCompileClean() {
  # Fix remove
  rm -rf $executableFilename *.o *.a $testFiles
}

# Cleans space of temporary compile files
function postCompileClean() {
  rm -f *.o *.a
}

# function generatePrecompiledObject() {
#   gcc --std=gnu99 -c ${1}/${1}.c
# }

# function generateModuleObjectsAndArchive() {
#   # Stores object list
#   precompileObjectList=""

#   # Create precompiled objects and list
#   for component in ${componentList[@]}; do
#     precompileObjectList+=" ${component}.o"
#     generatePrecompiledObject $component
#   done

#   # Creates archive with objects
#   ar -r ${preCompilePackage}.a $precompileObjectList 2>/dev/null
# }

# Compiles main
# function compileMainAndArchive() {
#   gcc --std=gnu99 -pthread -o $executableFilename $mainFilename ${preCompilePackage}.a
# }

function main() {
  # Pre cleaning, removes leftovers from previous runs
  # preCompileClean

  # Compilation step
  # generateModuleObjectsAndArchive
  # compileMainAndArchive

  # Post cleaning to eliminate temporary files
  # postCompileClean

  # Handles parameters to execute.
  # Param triggers:
  #  e -> run executable getting standard in from input1 testfile
  #  v -> run executable with valgrind leak analysis
  #  t -> run executable and then run test script
  #  m -> run executable to take input from stdin and output from stdout
  # while getopts "metv" flag; do
  #   case $flag in
  #   m) ./$executableFilename ;;
  #   e) ./$executableFilename <./testFiles/testInputFiles/input1.txt ;;
  #   t) ./testScript.sh ;;
  #   v) valgrind ./$executableFilename ;;
  #   esac
  #   shift
  # done

  # Compile Keygen
  # gcc --std=gnu99 -c sharedMethods sharedMethods/sharedMethods.c
  # gcc --std=gnu99 -c constants constants/constants.c

  # rm -rf ${preCompilePackage}.a
  gcc --std=gnu99 -c constants/constants.c
  gcc --std=gnu99 -c sharedMethods/sharedMethods.c
  ar -r ${preCompilePackage}.a constants.o sharedMethods.o 2>/dev/null

  gcc --std=gnu99 -o keygen keygen.c ${preCompilePackage}.a
  postCompileClean
  # ./keygen 1544
  ./keygen 256 >mykey
}

# Execute this script passing params to main
main "$@"
