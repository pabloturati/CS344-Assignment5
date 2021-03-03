#! /bin/bash

# Build data

# Programs
keygen="keygen"
enc_server="enc_server"
enc_client="enc_client"
dec_server="dec_server"
dec_client="dec_client"

preCompilePackage="component_archive"

executableFilenames="
  keygen 
  enc_server 
  dec_server 
  enc_client 
  dec_client
  "
outputFiles="
  mykey
  encryptedText
  decryptedText
  "

# Components
declare -a componentList=(
  "constants"
  "sharedMethods"
  "encryptionMethods"
)

encryptionPort=57171
decryptionPort=86771

# declare programList=(
#   "keygen"
#   "enc_server"
# )

# Cleans previously compiled files and created folders
function preCompileClean() {
  # Fix remove
  rm -rf $executableFilenames *.o *.a $outputFiles
}

# Cleans space of temporary compile files
function postCompileClean() {
  rm -f *.o *.a
}

function generatePrecompiledObject() {
  gcc --std=gnu99 -c ${1}/${1}.c
}

function generateModuleObjectsAndArchive() {
  # Stores object list
  precompileObjectList=""

  # Create precompiled objects and list
  for component in ${componentList[@]}; do
    precompileObjectList+=" ${component}.o"
    generatePrecompiledObject $component
  done

  # Creates archive with objects
  ar -r ${preCompilePackage}.a $precompileObjectList 2>/dev/null
}

# Compiles main
# function compileMainAndArchive() {
#   gcc --std=gnu99 -pthread -o $executableFilename $mainFilename ${preCompilePackage}.a
# }

function main() {
  # Pre cleaning, removes leftovers from previous runs
  preCompileClean

  # Compilation step
  generateModuleObjectsAndArchive
  # compileMainAndArchive
  # Compile Keygen
  gcc --std=gnu99 -o $keygen ${keygen}.c ${preCompilePackage}.a
  # Compile enc_server
  gcc --std=gnu99 -o $enc_server ${enc_server}.c ${preCompilePackage}.a
  # Compile enc_client
  gcc --std=gnu99 -o $enc_client ${enc_client}.c ${preCompilePackage}.a
  # Compile dec_server
  gcc --std=gnu99 -o $dec_server ${dec_server}.c ${preCompilePackage}.a
  # Compile dec_client
  gcc --std=gnu99 -o $dec_client ${dec_client}.c ${preCompilePackage}.a

  # Post cleaning to eliminate temporary files
  postCompileClean

  # Run enc_server in background
  ./enc_server $encryptionPort &

  # Run enc_server in background
  ./dec_server $decryptionPort &

  # Create a key of a certain size
  ./keygen 36 >mykey

  # Run
  ./enc_client sourceFiles/plaintext1 mykey $encryptionPort >encryptedText

  ./dec_client encryptedText mykey $decryptionPort >decryptedText

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
}

# Execute this script passing params to main
main "$@"
