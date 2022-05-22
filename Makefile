## Name of Project

NAME = webserv

## List of Directories

SRC_DIR = src
OBJ_DIR = obj
CONFIG_DIR = config
SERVER_DIR = server
HTTP_DIR = http

## Compilating Utilities

INC = $(./$(SRC_DIR)/$(CLASS_DIR)/-I%.hpp)

DEBUG = -g3

CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -MMD -D_REENTRANT #-fsanitize=address

CC = c++ $(CPPFLAGS) $(INC)

CONFIG_FT = Config BlockConfig Location

SERVER_FT = Sockets ClientManager

HTTP_FT = HttpResponse HttpRequest

SRC_FT = main

## List of Utilities

SRC = $(SRC_FT:%=$(SRC_DIR)/%.cpp) \
		$(CONFIG_FT:%=$(SRC_DIR)/$(CONFIG_DIR)/%.cpp) \
		$(SERVER_FT:%=$(SRC_DIR)/$(SERVER_DIR)/%.cpp) \
		$(HTTP_FT:%=$(SRC_DIR)/$(HTTP_DIR)/%.cpp)

OBJ = $(SRC:$(SRC_DIR)%.cpp=$(OBJ_DIR)%.o)

DEP = $(OBJ:$(OBJ_DIR)%.o=$(OBJ_DIR)/%.d)

OBJ_DIRS = $(OBJ_DIR) \
		   $(CONFIG_DIR:%=$(OBJ_DIR)/%) \
		   $(SERVER_DIR:%=$(OBJ_DIR)/%) \
		   $(HTTP_DIR:%=$(OBJ_DIR)/%)

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
