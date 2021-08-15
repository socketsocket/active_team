# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: seyu <seyu@student.42seoul.kr>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/10/10 23:07:12 by seheon            #+#    #+#              #
#    Updated: 2021/08/15 17:32:26 by seyu             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.SUFFIXES:
.SUFFIXES: .cpp .o .hpp

NAME	=	webserv
SHELL	:=	/bin/zsh

CP		=	cp
RM		=	rm -f
MKDIR	=	mkdir -p
MV		=	mv
ECHO	=	echo
PRINTF	=	printf
PROGRESS=	0

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
# Echo Escape Characters
# ----------------------------------

LINE_CLEAR	= \33[2K\r

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
	@$(eval PROGRESS = $(shell expr $(PROGRESS) + 1))
	@$(PRINTF) "$(LINE_CLEAR)""$(WHITE)Compiling Progress$(NOCOLOR): [%3d%%] [ " $$(expr $(PROGRESS) \* 100 \/ $$($(ECHO) $(SRCS) | wc -w))
	@itr=1; while [[ $${itr} -le $(PROGRESS) ]] ; do \
		$(PRINTF) '\u2588' ; \
		((itr = itr + 1)) ; \
	done
	@itr=$$(expr $(PROGRESS) + 1); while [[ $$itr -le $$($(ECHO) $(SRCS) | wc -w) ]] ; do \
		$(PRINTF) '.' ; \
		((itr = itr + 1)) ; \
	done
	@$(PRINTF) ' ] : $(DARKGRAY)$<$(NOCOLOR)'
	@$(CC) $(CFLAGS) -I$(DIR_INC) -c $< -o $@

$(NAME):	$(DIR_OBJ) $(OBJS)
	@$(PRINTF) "$(LINE_CLEAR)"
	@$(ECHO) "$(WHITE)Compiled$(NOCOLOR) all source files"
	@$(CC) $(CFLAGS) -I$(DIR_INC) $(OBJS) $(CLIBFMW) -o $(NAME)
	@$(ECHO)
	@$(ECHO) "Usage: ./webserv [config file]"

bonus:	$(NAME)

clean:
	@$(RM) -r $(DIR_OBJ)
	@$(ECHO) "$(WHITE)Removed$(NOCOLOR) $(RED)$(DIR_OBJ)$(NOCOLOR)"

fclean:	clean
	@$(RM) -r $(NAME)
	@$(ECHO) "$(WHITE)Removed$(NOCOLOR) $(RED)$(NAME)$(NOCOLOR)"

re:		fclean $(NAME)
