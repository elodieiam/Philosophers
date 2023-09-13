CC = cc 
NAME = philo
CFLAGS = -Wall -Wextra -Werror -pthread

SRC = check_args.c\
	main.c

OBJ = $(SRC:.c=.o)


all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	
$(NAME): $(OBJ) philosophers.h
	$(CC) $(CFLAGS) $(OBJ) -o $@

clean :
	rm -f $(OBJ)
