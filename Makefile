# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gshim <gshim@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/22 17:48:50 by gshim             #+#    #+#              #
#    Updated: 2022/03/26 17:09:41 by gshim            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
#CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address

NAME = minishell

SRCS_DIR = ./src
SRCS =  src/DataStructure/linkedlist1.c src/DataStructure/linkedlist2.c \
	src/DataStructure/tree1.c src/DataStructure/tree2.c \
	src/Parsing/parse_envv.c src/Parsing/check_syntax.c \
	src/Parsing/tokenize.c \
	src/Utilities/utils1.c src/Utilities/utils2.c \
	src/Utilities/utils3.c\
	src/minishell.c src/builtIn.c \
	src/Prompt/readline.c src/Prompt/sighandle.c \
	src/Redirect/redirect.c
OBJS = $(SRCS:.c=.o)

SHELL	= /bin/bash
sp		= /-\|/
idx		= 0


#======================= 외부라이브러리 모음 ==========================
LIB_NAME = ft
LIB_DIR = ./include/libft
LIB = $(addprefix $(LIB_DIR)/, libft.a)

#=================================================================

$(NAME) : $(OBJS)
	$(MAKE) -C $(LIB_DIR) all
	$(CC) $(CFLAGS) -L$(LIB_DIR) -l$(LIB_NAME) \
	-lreadline -L/Users/gshim/.brew/opt/readline/lib $^ -o $@
#libft list 중복이라 bonus로 컴파일 안함
#my mac #-L/opt/homebrew/opt/readline/lib -I/opt/homebrew/opt/readline/include
#cluster#-lreadline -L/Users/gshim/.brew/opt/readline/lib -I/Users/gshim/.brew/opt/readline/include $^ -o $@

$(SRCS_DIR)/%.o : $(SRCS_DIR)/%.c
	$(eval idx = $(shell expr $(idx) + 1))
	$(eval t_idx = $(shell expr $(idx) % 4 + 1))
	$(eval chr = $(shell echo $(sp) | cut -c $(t_idx)))
	@if [ $(idx) = 1 ]; then\
		echo -n "☕️ ☕️ MINISHELL Loading ...\n";\
	fi
	@printf "\b$(chr)"
	@$(CC) $(CFLAGS) -I$(LIB_DIR) -I/Users/gshim/.brew/opt/readline/include -c $< -o $@

all : $(NAME)

clean :
	rm -rf $(OBJS)
	$(MAKE) -C $(LIB_DIR) clean

fclean :
	rm -rf $(NAME) $(OBJS)
	$(MAKE) -C $(LIB_DIR) fclean

re : fclean all




.PHONY : all clean fclean re
