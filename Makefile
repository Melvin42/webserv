## Name of Project

NAME = webserv

## List of Directories

SRC_DIR = src
OBJ_DIR = obj
CLASS_DIR = class

## Compilating Utilities

INC = $(./$(SRC_DIR)/$(CLASS_DIR)/-I%.hpp)## $(./$(SRC_DIR)/$(CLASS_DIR)/-I%.h)

DEBUG = -g3

CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -MMD -D_REENTRANT

CC = c++ $(CPPFLAGS) $(INC)

CLASS_FT = Sockets HttpResponse HttpRequest Config BlockConfig ClientManager

#SRC_LIST = main.cpp Sockets.cpp HttpResponse.cpp HttpRequest.cpp
#
#SRCS = $(addprefix ${SRC_DIR}, ${SRC_LIST})
#
#OBJ = ${SRCS:.cpp=.o}

SRC_FT = main

## List of Utilities

SRC = $(SRC_FT:%=$(SRC_DIR)/%.cpp) \
		$(CLASS_FT:%=$(SRC_DIR)/$(CLASS_DIR)/%.cpp)

OBJ = $(SRC:$(SRC_DIR)%.cpp=$(OBJ_DIR)%.o)

DEP = $(OBJ:$(OBJ_DIR)%.o=$(OBJ_DIR)/%.d)

OBJ_DIRS = $(OBJ_DIR) \
		   $(CLASS_DIR:%=$(OBJ_DIR)/%)

## Rules of Makefile

all : $(NAME)

-include $(DEP)

$(OBJ_DIRS):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC)
	$(CC) -c $< -o $@

$(NAME): $(OBJ_DIRS) $(SRC) $(INC)
	$(MAKE) -j -s $(OBJ)
	$(CC) $(OBJ) -o $@

debug: $(OBJ_DIRS) $(SRC) $(INC)
	$(MAKE) -j -s $(OBJ)
	$(CC) $(DEBUG) -o $(NAME) $(OBJ)

clean :
	rm -rf $(OBJ_DIR)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY : all clean fclean re debug -include
