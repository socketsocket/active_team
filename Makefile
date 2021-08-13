# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: seyu <seyu@student.42seoul.kr>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/10/10 23:07:12 by seheon            #+#    #+#              #
#    Updated: 2021/08/09 23:08:52 by seyu             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.SUFFIXES:
.SUFFIXES: .cpp .o .hpp

NAME	=	webserv

CP		=	cp
RM		=	rm -f
MKDIR	=	mkdir -p
MV		=	mv
ECHO	=	echo

# ----------------------------------
# Echo Colors
# ----------------------------------

NOCOLOR		= \033[0m
RED			= \033[0;31m
GREEN		= \033[0;32m
ORANGE		= \033[0;33m
BLUE		= \033[0;34m
PURPLE		= \033[0;35m
CYAN		= \033[0;36m
LIGHTGRAY	= \033[0;37m
DARKGRAY	= \033[1;30m
LIGHTRED	= \033[1;31m
LIGHTGREEN	= \033[1;32m
YELLOW		= \033[1;33m
LIGHTBLUE	= \033[1;34m
LIGHTPURPLE	= \033[1;35m
LIGHTCYAN	= \033[1;36m
WHITE		= \033[1;37m

# ----------------------------------
# Mandatory source files
# ----------------------------------

SRCS	=	main.cpp

SRCS	+=	\
			CGI.cpp \
			Client.cpp \
			EventHandler.cpp \
			FDManager.cpp \
			Location.cpp \
			PortManager.cpp \
			Request.cpp \
			RequestReader.cpp \
			Resource.cpp \
			Response.cpp \
			ResponseWriter.cpp \
			Server.cpp \



# ----------------------------------
# Object files
# ----------------------------------

OBJS	=	$(addprefix $(DIR_OBJ)/, $(SRCS:%.cpp=%.o))

# ----------------------------------
# Directories and Paths
# ----------------------------------

DIR_INC	=	./inc
DIR_SRC	=	./src
DIR_OBJ	=	./obj

SUBDIRS	=	\

VPATH	=	$(DIR_SRC) $(addprefix $(DIR_SRC)/, $(SUBDIRS))

# ----------------------------------
# Compiler and flags
# ----------------------------------

CC		=	clang++
CFLAGS	=	-Wall -Wextra -Werror -g3 -fsanitize=address
CLIBFMW	=

# ----------------------------------
# Rules
# ----------------------------------

all: $(NAME)

$(DIR_OBJ):
	@$(MKDIR) $@

$(DIR_OBJ)/%.o: %.cpp
	@$(CC) $(CFLAGS) -I$(DIR_INC) -c $< -o $@
	@$(ECHO) "Compiled $(ORANGE)" $< " $(GREEN)Successfully$(NOCOLOR)"

$(NAME):	$(DIR_OBJ) $(OBJS)
	@$(CC) $(CFLAGS) -I$(DIR_INC) $(OBJS) $(CLIBFMW) -o $(NAME)
	@$(ECHO) "Maked $(BLUE)$(NAME)$(NOCOLOR) $(GREEN)Successfully$(NOCOLOR)"
	@$(ECHO) "Usage: ./$(NAME) [config_file]"

bonus:	$(NAME)

clean:
	@$(RM) -r $(DIR_OBJ)
	@$(ECHO) "Clean up $(RED)$(NAME)$(NOCOLOR)"

fclean:	clean
	@$(RM) -r $(NAME)
	@$(ECHO) "Removed $(RED)$(NAME)$(NOCOLOR)"

re:		fclean $(NAME)
