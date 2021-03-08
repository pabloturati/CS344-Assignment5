#! /bin/bash

# Test data
keyFilename="mykey"
encryptedTextFilename="encryptedText"
decryptedTextFilename="decryptedText"
testFilesPrefix="plaintext"

# Build data
preCompilePackage="component_archive"

applicationList="
  keygen 
  enc_server 
  dec_server 
  enc_client 
  dec_client
  "
outputFiles="
  $keyFilename
  $encryptedTextFilename
  $decryptedTextFilename
  tempReceivedRawTextFile_*
  tempReceivedKeyFile_* 
  tempEncryptedFile_*
  key20
  key70000
  "

# Components
declare -a componentList=(
  "constants"
  "sharedMethods"
  "encryptionMethods"
  "serverNetworkMethods"
  "clientNetworkMethods"
  "clientDataMethods"
  "serverDataMethods"
)

# Port ranges
minPort=49152
maxPort=65535

# minPort=491
# maxPort=655

# Ports initialized to empty
encryptionPort=""
decryptionPort=""

# Function to calculate a random port
function assignRandomPorts() {
  portRange=$((maxPort - minPort + 1))
  encryptionPort=$((RANDOM % portRange + minPort))
  decryptionPort=$((RANDOM % portRange + minPort))
}

function initializePorts() {
  # Do-while loop to initialize port with different guaranteed numbers
  assignRandomPorts
  while [[ $encryptionPort == $decryptionPort ]]; do
    assignRandomPorts
  done
}

# Cleans previously compiled files and created folders
function preCompileClean() {
  # Fix remove
  rm -rf $applicationList *.o *.a $outputFiles
}

# Cleans space of temporary compile files
function postCompileClean() {
  rm -f *.o *.a
}

# Creates a precompiled object file "*.o" to be used by archive
function generatePrecompiledObject() {
  gcc --std=gnu99 -c ${1}/${1}.c
}

# Takes in all object files "*.o" and creates a precompiled archive
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

# Compiles each app
function compileApplications() {
  # gcc --std=gnu99 -pthread -o $executableFilename $mainFilename ${preCompilePackage}.a
  for app in ${applicationList[@]}; do
    gcc --std=gnu99 -o $app ${app}.c ${preCompilePackage}.a
  done
}

# Deploys both encryption and decryption servers in background
function deployServers() {
  echo "MAKEFILE: Deploying servers encryptionPort: $encryptionPort decryptionPort: $decryptionPort"
  # Run enc_server in background
  ./enc_server $encryptionPort &
  # Run enc_server in background
  ./dec_server $decryptionPort &
}

# Function to calculate a file size.
# arg 1: filename(path)
# arg 2: variable name to return the size
function calculateFileCharCount() {
  # Create a local temp file to store count result
  rm -rf fileSizeTemp*
  wc -m ${1} >fileSizeTemp$$
  # Save char count to variable
  local characters
  read characters _ <fileSizeTemp$$
  # Perform cleanup
  rm -rf fileSizeTemp*
  # If file is not empty, decrease the count of the endline character
  if [[ characters -gt 0 ]]; then
    let "characters -= 1"
  else
    echo "MAKEFILE: Error: File is empty"
  fi
  # Return char count
  eval $2="'$characters'"
}

# Function to trigger a key generation of the same char count as the file
# arg 1: source filename
# arg 2: destination filename
function crateKeyForFile() {
  local __keySize
  # Get the number of characters
  calculateFileCharCount ${1} __keySize
  # Create a key of a certain size
  ./keygen $__keySize >${2}
}

# Function to get a process id using a server name and port
# arg 1: server name (i.e. enc_server)
# arg 2: port number
# arg 3: variable to return the process id of the server
function gerProcessIdOfServer() {
  rm -f processIdFileTemp*
  ps | grep -m 1 -a "./${1} ${2}" >processIdFileTemp$$
  local server_pid
  read server_pid _ _ <processIdFileTemp$$
  eval ${3}="'$server_pid'"
  rm -f processIdFileTemp*
}

# Function to kill both servers
function killServerProcesses() {
  gerProcessIdOfServer enc_server $encryptionPort enc_server_pid
  gerProcessIdOfServer dec_server $decryptionPort dec_server_pid
  kill -SIGTERM $enc_server_pid $dec_server_pid
  echo "MAKEFILE: Servers with PID $enc_server_pid and $dec_server_pid have been killed"
}

# Function to clean post a quickrun
function postQuickRunClean() {
  rm -f $keyFilename $encryptedTextFilename $decryptedTextFilename
}

# Compares output file with expected output file
# Arg list
# 1 - original file
# 2 - output file to compare the original file
# 3 - result is expected to fail. If param is passed, test PASES when cmp is NOT the same
function runTest() {
  echo "*******************"
  if cmp -s $1 $2; then
    printf "MAKEFILE Test $1: PASS -> $1 is identical to $2 \n"
  else
    printf "MAKEFILE Test $1: FAIL -> $1 does not match $2\n"
  fi
  echo "*******************"
}

# Runs full run creating a key, encrypting and decrypting
# Arg list
# 1 - Raw text file
function runCryptoProcess() {
  crateKeyForFile $1 $keyFilename
  ./enc_client $1 $keyFilename $encryptionPort >$encryptedTextFilename
  ./dec_client $encryptedTextFilename $keyFilename $decryptionPort >$decryptedTextFilename
}

# Does a quick run based on a file passed
# arg 1: filename for quickrun
function completerRun() {
  deployServers
  testFile="$testFilesPrefix$1"
  runCryptoProcess $testFile
  runTest $testFile $decryptedTextFilename
  killServerProcesses $testFile
}

function testAll() {
  deployServers
  resultsFile="results$$"
  for i in $(seq 1 5); do
    filePath="$testFilesPrefix$i"
    runCryptoProcess $filePath >>$resultsFile
  done
  cat $resultsFile
  rm -f $resultsFile
  killServerProcesses
}

function fullClean() {
  preCompileClean
  postQuickRunClean
}

function main() {
  # Pre cleaning, removes leftovers from previous runs
  fullClean
  initializePorts

  # Compilation step
  generateModuleObjectsAndArchive
  compileApplications

  # Post cleaning to eliminate temporary files
  postCompileClean

  # Run
  # Handles parameters to execute.
  # Param triggers:
  # s - executable getting standard in from input1 testfile >run
  # n - generate key for a certain filename (arg = filename)
  # k - kill both server processes
  # q - do a test run with a testfile. Arg is number 1 thru 5 for each plaintext file
  # f - to do a full clean of the project output and compile files
  # u - tests all plaintext files in path testfiles/plaintext*
  # z - runs grading script
  while getopts "sn:kq:1fzu" flag; do
    case $flag in
    s) deployServers ;;
    n) crateKeyForFile $OPTARG mykey ;;
    k) killServerProcesses ;;
    q) completerRun $OPTARG ;;
    f) fullClean ;;
    u) testAll ;;
    z) ./p5testscript $encryptionPort $decryptionPort ;;
    esac
    shift
  done
}

# Execute this script passing params to main
main "$@"
