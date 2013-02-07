@echo off
cmake -DCMAKE_TOOLCHAIN_FILE=i686.tch -Dgtest_disable_pthreads=ON .