@ECHO OFF

rem current run directory
SET RUN_DIR="%CD%"

rem create the _build directory if it doesn't exist
cmake -E make_directory _build

rem run cmake from the build directory to configure the project
cmake -E chdir _build cmake -DCMAKE_GENERATOR_PLATFORM=x64 -DCMAKE_INSTALL_PREFIX=%RUN_DIR% ..\..

rem run the cmake build command to build the project with the native build system
cmake -E chdir _build cmake --build . --target INSTALL --config Release -- /m
