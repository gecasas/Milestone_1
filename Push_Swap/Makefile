NAME = push_swap
BONUS_NAME	= checker

CC = cc
CFLAGS = -Wall -Wextra -Werror
CPPFLAGS = -I libft -I libft/printf
LIBFT = libft/libft.a
FT_PRINTF = libft/printf/libftprintf.a
SRCS = \
	algorithms.c \
	argparser.c \
	push_operations.c \
	push_swap.c \
	reverse_rotate_operations.c \
	rotate_operations.c \
	sort_chunks.c \
	sort_radix.c \
	stack.c \
	flags.c \
	algorithm_parser.c \
	stack_operations.c	
OBJS = $(SRCS:.c=.o)

BONUS_SRCS = \
		checker_bonus.c \
		parse_bonus.c \
		read_bonus.c \
		stack_bonus.c \
		operations_bonus.c 

BONUS_OBJS	= $(BONUS_SRCS:.c=.o)

all: $(NAME)

bonus: $(BONUS_NAME)

$(NAME): $(LIBFT) $(FT_PRINTF) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(FT_PRINTF) -o $(NAME)

$(BONUS_NAME): $(BONUS_OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(BONUS_OBJS) $(LIBFT) -o $(BONUS_NAME)

$(LIBFT):
	$(MAKE) -C libft

$(FT_PRINTF):
	$(MAKE) -C libft/printf

%.o: %.c push_swap.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

%_bonus.o: %_bonus.c checker_bonus.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	rm -f $(BONUS_OBJS)
	$(MAKE) -C libft clean
	$(MAKE) -C libft/printf clean

fclean: clean
	rm -f $(NAME)
	rm -f $(BONUS_NAME)
	$(MAKE) -C libft fclean
	$(MAKE) -C libft/printf fclean

re: fclean all

.PHONY: all bonus clean fclean re
