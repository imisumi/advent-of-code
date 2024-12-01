NAME = AoC

TEST_INPUT = test_input
INPUT = input

all:
	c++ -std=c++20 main.cpp -o $(NAME)

clean:
	rm -f $(NAME)

run: all
	./$(NAME) $(INPUT)

test: all
	./$(NAME) $(TEST_INPUT)