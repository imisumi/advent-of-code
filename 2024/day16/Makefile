NAME = AoC

TEST_INPUT = example_input
INPUT = input

all:
	c++ -std=c++23 -g main.cpp -o $(NAME)

clean:
	rm -f $(NAME)

run: all
	./$(NAME) $(INPUT)

test: all
	./$(NAME) $(TEST_INPUT)