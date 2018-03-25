#!/usr/bin/env bash
pushd cmake-build-debug
make AirWaves_coverage -j
lcov --directory . --capture --output-file coverage.info # capture coverage info
lcov --remove coverage.info '/usr/*' --output-file coverage.info # filter out system
lcov --remove coverage.info '*/thirdparty/*' --output-file coverage.info # filter out non-project libs
genhtml coverage.info --output-directory cov_report
cd cov_report
xdg-open index.html
popd
