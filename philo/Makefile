CC = cc 
NAME = philo
CFLAGS = -Wall -Wextra -Werror -pthread -g3 

#-fsanitize=address

SRC = check_args.c\
	main.c\
	init.c\
	actions.c\
	forks.c\
	init_utils.c\
	routine.c\
	utils.c\

OBJ = $(SRC:.c=.o)

all: $(NAME)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	
$(NAME): $(OBJ) philosophers.h
	@$(CC) $(CFLAGS) $(OBJ) -o $@
	@echo "Making philo..."
	@echo "Ready !"

clean :
	rm -f $(OBJ)

fclean: clean
	rm -rf $(NAME)

re:	fclean all