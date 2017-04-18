#!/bin/bash

# toggleable variables
BUILD_MODE=Release
STRICT_FLAGS=OFF
TESTING=OFF

CLEAN=false


function printUsage() {
  echo "Usage: ./unixConfigureAndBuild.sh <options>";
  echo "    options:";
  echo "        -sf or --strict-flags -> enable strict compile flags";
  echo "        -t  or --test         -> compile unit tests";
  echo "        -d  or --debug        -> compile in debug mode";
  echo "        -h  or --help         -> print this message";
}

# http://stackoverflow.com/questions/192249/how-do-i-parse-command-line-arguments-in-bash

# Use -gt 1 to consume two arguments per pass in the loop (e.g. each
# argument has a corresponding value to go with it).
# Use -gt 0 to consume one or more arguments per pass in the loop (e.g.
# some arguments don't have a corresponding value to go with it such
# as in the --default example).
# note: if this is set to -gt 0 the /etc/hosts part is not recognized ( may be a bug )
while [[ $# -gt 0 ]]
do
key="$1"

case $key in
    -sf|--strict-flags)
    STRICT_FLAGS=ON
    ;;
    -t|--test)
    TESTING=ON
    ;;
    -d|--debug)
    BUILD_MODE=Debug
    ;;
    -c|--clean)
    CLEAN=true
    ;;
    -h|--help)
    printUsage;
    [[ "${BASH_SOURCE[0]}" != "${0}" ]] && return 0 || exit 0
    ;;
    *)
    echo "Unrecognized option: '$key'" # unknown option
    printUsage;
    [[ "${BASH_SOURCE[0]}" != "${0}" ]] && return 1 || exit 1
    ;;
esac
shift # past argument or value
done


# clean project by removing installed dirs
if [[ "$CLEAN" == true ]]
  then

  cmake -E remove_directory _build
  cmake -E remove_directory bin
  cmake -E remove_directory lib
  cmake -E remove_directory libbin
  cmake -E remove_directory include
  cmake -E remove_directory testbin
  cmake -E remove imgui.ini

  echo "Project clean"
  [[ "${BASH_SOURCE[0]}" != "${0}" ]] && return 0 || exit 0
fi


# current run directory
RUN_DIR=$(pwd)

# create the _build directory
cmake -E make_directory _build

# run cmake from the build directory to configure the project
cmake -E chdir _build cmake -DCMAKE_BUILD_TYPE=$BUILD_MODE \
                            -DSTRICT_FLAGS=$STRICT_FLAGS \
                            -DBUILD_TESTS=$TESTING \
                            -DCMAKE_INSTALL_PREFIX=$RUN_DIR ../..

# run the cmake build command to build the project with the native build system
cmake -E chdir _build cmake --build . --target install --config $BUILD_MODE -- -j12

