# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/13 13:51:32 by irychkov          #+#    #+#              #
#    Updated: 2024/09/24 22:18:02 by irychkov         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

SRCS = main.c child_process.c checks.c wait_and_print.c \
		free.c errors.c sys_errors.c sys_errors2.c error_filename.c

BONUS_SRCS = $(BONUS_DIR)/main_bonus.c $(BONUS_DIR)/exec_child_bonus.c \
		$(BONUS_DIR)/checks_bonus.c $(BONUS_DIR)/wait_and_print_bonus.c \
		$(BONUS_DIR)/free_bonus.c $(BONUS_DIR)/errors_bonus.c \
		$(BONUS_DIR)/sys_errors_bonus.c $(BONUS_DIR)/sys_errors2_bonus.c \
		$(BONUS_DIR)/here_doc_bonus.c $(BONUS_DIR)/init_pipex_bonus.c \
		$(BONUS_DIR)/error_filename_bonus.c

BONUS_DIR = ./bonus

OBJS = $(SRCS:.c=.o)
BONUS_OBJS = $(BONUS_SRCS:.c=.o)

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

HEADERS = -I. -I$(LIBFT_DIR)
HEADERS_BONUS = -I$(BONUS_DIR) -I$(LIBFT_DIR)

CFLAGS = -Wall -Wextra -Werror
CC = cc

all: mandatory

mandatory: .mandatory

.mandatory: $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)
	@rm -f .bonus
	@touch .mandatory
	@echo "Compiled regular version: $(NAME)"

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

bonus : .bonus

.bonus: $(BONUS_OBJS) $(LIBFT)
		$(CC) $(CFLAGS) $(BONUS_OBJS) $(LIBFT) -o $(NAME)
		@rm -f .mandatory
		@touch .bonus
		@echo "Compiled bonus version: $(NAME)"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(HEADERS)

clean:
	@rm -rf $(OBJS) $(BONUS_OBJS)
	@$(MAKE) clean -C $(LIBFT_DIR)
	@rm -f .bonus .mandatory
	@echo "Cleaned object files"

fclean: clean
	@rm -rf $(NAME)
	@$(MAKE) fclean -C $(LIBFT_DIR)
	@echo "Fully cleaned"

re: fclean all

.PHONY: all clean fclean re bonus libft