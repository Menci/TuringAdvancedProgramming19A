make && make malloc_debug.so
LD_PRELOAD="./malloc_debug.so" ./main