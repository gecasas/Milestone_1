NAME = libftprintf.a
CC = cc
CFLAGS = -Wall -Wextra -Werror
CPPFLAGS = -I.
AR = ar
ARFLAGS = rcs

SRCS = ft_printf.c ft_puts.c ft_puts2.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

	
$(NAME): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^
	
%.o: %.c printf.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJS): $(SRCS:.c=.o)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all libft clean fclean re
