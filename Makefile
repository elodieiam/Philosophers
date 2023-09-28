CC = cc 
NAME = philo
CFLAGS = -Wall -Wextra -Werror -pthread -g3 -fsanitize=address -g

#-fsanitize=address

SRC = check_args.c\
	main.c\
	init.c\

OBJ = $(SRC:.c=.o)


all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	
$(NAME): $(OBJ) philosophers.h
	$(CC) $(CFLAGS) $(OBJ) -o $@

clean :
	rm -f $(OBJ)

fclean: clean
	rm -rf $(NAME)

re:	fclean all