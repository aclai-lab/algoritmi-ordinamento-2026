TARGET = a.out

all:
	gcc -Wall -Wextra -Werror -Iinclude -O3 src/main.c src/benchmark.c src/config.c src/sort.c src/plot.c src/tomlc17.c -o $(TARGET)
