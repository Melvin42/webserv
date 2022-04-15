NAME = webserv

CC = c++

DEBUG = -g3

CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -MMD

SRC = main.cpp

OBJ = ${SRC:.cpp=.o}

DEP = ${OBJ:.o=.d}

.cpp.o:
	$(CC) $(CPPFLAGS) -c $< -o $@

all : $(NAME)

debug:
	$(CC) $(CPPFLAGS) $(DEBUG) -o $(NAME) $(OBJ)

81 : $(OBJ)
	$(CC) $(CPPFLAGS) -D TEST=8081 -o $(NAME) $(OBJ)

-include $(DEP)

$(NAME) : $(OBJ)
	$(CC) $(CPPFLAGS) -o $(NAME) $(OBJ)

clean :
	rm -f $(OBJ) $(DEP)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY : all clean fclean re .cpp.o
