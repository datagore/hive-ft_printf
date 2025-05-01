SOURCES := ft_printf.c \

OBJECTS := $(SOURCES:%.c=%.o)

HEADERS := ft_printf.h
NAME := libftprintf.a
CFLAGS = -Wall -Wextra -Werror

$(NAME): $(OBJECTS)
	ar -rcs $@ $^

%.o: %.c $(HEADERS)
	cc -c $< -o $@ $(CFLAGS)

all: $(NAME)

re: fclean all

clean:
	rm -f $(OBJECTS) $(BONUS_OBJECTS) test

fclean: clean
	rm -f $(NAME)

test: test.c $(NAME)
	cc $^ -o $@ $(CFLAGS)
	./$@

.PHONY: all re fclean clean bonus test
.SECONDARY: $(OBJECTS)
