port=57171
preCompilePackage="component_archive"
enc_server="enc_server"

generateModuleObjectsAndArchive

gcc --std=gnu99 -o $enc_server ${enc_server}.c ${preCompilePackage}.a
./enc_server $port
