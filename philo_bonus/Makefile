SRCS	= $(wildcard *.c)
ODIR	= objs
OBJS	= $(SRCS:%.c=$(ODIR)/%.o)
CFLAGS	= -Wall -Wextra -Werror -Wpedantic -g -pedantic -pthread
CFLAGS	+= -fsanitize=thread
NAME	= philo_bonus

all: $(LIB) $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	$(RM)r $(ODIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

$(ODIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

leak: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME) 5 100 100 300

.PHONY: all clean fclean re
