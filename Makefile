NAME = webserv
CC = clang++
CFLAG = #-Wall -Wextra -Werror -std=c++98
SRC_NAME = main\
			ServerManager
SRC = $(addprefix src/,$(addsuffix .cpp, $(SRC_NAME)))
SRCDIR = ./src
INCLUDE = -I ./inc 
MAKE = make

all : $(NAME)

clean :

fclean : clean
	rm -rf $(NAME)

re : fclean all

bonus : all

run : all
	./webserv

${NAME} : $(SRC)
	${CC} ${CFLAG} ${INCLUDE} ${SRC} -o ${NAME}

.PONY : all clean fclean re bonus run