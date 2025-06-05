NAME = webserv

SRCS = $(wildcard src/*.cpp)
INC = $(wildcard includes/*.hpp)

all: $(NAME)

$(NAME): $(SRCS) $(INC)
	c++ -Wall -Wextra -Werror -std=c++98 -Iincludes -g -fsanitize=address $(SRCS) -o $(NAME)

clean:
	rm -f *.o

fclean: clean
	rm -f $(NAME)

re: fclean all