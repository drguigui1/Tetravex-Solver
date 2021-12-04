SRC = src/*.cc

all:
	g++ -Wall -Wextra -Werror -pedantic $(SRC)

clean:
	rm a.out
