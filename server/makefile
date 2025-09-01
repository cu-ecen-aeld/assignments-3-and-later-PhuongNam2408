CC := $(CROSS_COMPILE)gcc
SRCS := aesdsocket.c
OBJS := $(SRCS:.c=.o)
EXE := aesdsocket
FLAGS := -Wall -Werror

$(EXE) : $(SRCS)
	$(CC) $(SRCS) $(FLAGS) -o $(EXE)

clean :
	rm -f $(OBJS) $(EXE)