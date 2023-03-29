# test for cson
add log from https://github.com/rxi/log.c

valgrind --leak-check=yes --show-reachable=yes ./build/test-cson
