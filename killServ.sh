#! /bin/bash

# Function to get a process id using a server name and port
# arg 1: server name (i.e. enc_server)
# arg 2: variable to return the process id of the server
function gerProcessIdOfServer() {
  rm -f processIdFileTemp*
  ps | grep -m 1 -a "./${1}" >processIdFileTemp$$
  local server_pid
  read server_pid _ _ <processIdFileTemp$$
  eval ${2}="'$server_pid'"
  rm -f processIdFileTemp*
}

# Function to kill both servers
function killServerProcesses() {
  gerProcessIdOfServer enc_server enc_server_pid
  # gerProcessIdOfServer dec_server $decryptionPort dec_server_pid
  # FIX Uncomment
  # kill -SIGTERM $enc_server_pid $dec_server_pid
  # FIX Remove
  # kill -SIGTERM $enc_server_pid $dec_server_pid
  kill -SIGTERM $enc_server_pid
  echo "MAKEFILE: Servers with PID $enc_server_pid has been killed"
  ps
}

killServerProcesses

# rm -f tempReceivedRawTextFile_* tempReceivedKeyFile_* tempEncryptedFile_*
