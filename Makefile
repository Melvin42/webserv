NAME = webserv

CC = c++

DEBUG = -g3

CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -MMD #-fsanitize=address

SRC_DIR = src/

SRC_LIST = main.cpp Sockets.cpp HttpResponse.cpp HttpRequest.cpp

SRCS = $(addprefix ${SRC_DIR}, ${SRC_LIST})

OBJ = ${SRCS:.cpp=.o}

DEP = ${OBJ:.o=.d}

.cpp.o:
	$(CC) $(CPPFLAGS) -c $< -o $@

all : $(NAME)

debug: $(OBJ)
	$(CC) $(CPPFLAGS) $(DEBUG) -o $(NAME) $(OBJ)

-include $(DEP)

$(NAME) : $(OBJ)
	$(CC) $(CPPFLAGS) -o $(NAME) $(OBJ)

clean :
	rm -f $(OBJ) $(DEP)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY : all clean fclean re .cpp.o
