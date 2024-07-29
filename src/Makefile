CC=g++ -std=c++17
CFLAGS=-Wall -Wextra -Werror
TFLAGS=-lgtest -lgtest_main -pthread

all: test

test: clean
	$(CC) $(CFLAGS) tests/*.c** -o test $(TFLAGS)
	./test

clean:
	rm -rf test

# style:
# 	clang-format -i */*.c* */*.h
style:
	clang-format -i */*.c* */*.h -style=Google