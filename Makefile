# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/13 13:51:32 by irychkov          #+#    #+#              #
#    Updated: 2024/09/18 23:48:00 by irychkov         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex
BONUS_NAME = pipex_bonus

SRCS = main.c child_process.c checks.c wait_and_print.c \
		free.c errors.c sys_errors.c sys_errors2.c

BONUS_SRCS = $(BONUS_DIR)/main_bonus.c $(BONUS_DIR)/exec_child_bonus.c \
		$(BONUS_DIR)/checks_bonus.c $(BONUS_DIR)/wait_and_print_bonus.c \
		$(BONUS_DIR)/free_bonus.c $(BONUS_DIR)/errors_bonus.c \
		$(BONUS_DIR)/sys_errors_bonus.c $(BONUS_DIR)/sys_errors2_bonus.c \
		$(BONUS_DIR)/here_doc_bonus.c

BONUS_DIR = ./bonus

OBJS = $(SRCS:.c=.o)
BONUS_OBJS = $(BONUS_SRCS:.c=.o)

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

HEADERS = -I. -I$(LIBFT_DIR)
HEADERS_BONUS = -I$(BONUS_DIR) -I$(LIBFT_DIR)

CFLAGS = -Wall -Wextra -Werror
CC = cc

all: $(NAME)

bonus: $(BONUS_NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(HEADERS)
	@echo "Compiling: $<"

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(OBJS) $(LIBFT) -o $(NAME)

$(BONUS_NAME): $(LIBFT) $(BONUS_OBJS)
	@$(CC) $(BONUS_OBJS) $(LIBFT) -o $(BONUS_NAME)

clean:
	@rm -rf $(OBJS) $(BONUS_OBJS)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -rf $(NAME) $(BONUS_NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re bonus libft