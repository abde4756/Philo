FILS = main.c utils.c input.c state.c philo.c

OBJS = $(FILS:.c=.o)

NAME = philosopher

CC = cc -g

SUP = rm -f

CFLAGS = -Wall -Wextra -Werror 

all : $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ 

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean : 
	$(SUP) $(OBJS)

fclean : clean
	$(SUP) $(NAME)

re : fclean clean all