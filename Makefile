SOURCES := ft_printf.c write.c
OBJECTS := $(SOURCES:%.c=%.o)
HEADERS := ft_printf.h
NAME := libftprintf.a
CFLAGS = -Wall -Wextra -Werror

$(NAME): $(OBJECTS)
	$(AR) -rcs $@ $^

%.o: %.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

all: $(NAME)

re: fclean all

clean:
	$(RM) $(OBJECTS) $(BONUS_OBJECTS) test

fclean: clean
	$(RM) $(NAME)

test: test.c $(NAME)
	$(CC) $^ -o $@ $(CFLAGS)
	./$@

.PHONY: all re fclean clean bonus test
.SECONDARY: $(OBJECTS)
