SRC = src/*.cc

all:
	g++ -Wall -Wextra -Werror -std=c++17 -pedantic $(SRC)

clean:
	rm a.out
