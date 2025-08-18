
CC := $(CROSS_COMPILE)gcc
SRCS := writer.c
OBJS := $(SRCS:.c=.o)
EXE := writer
FLAGS := 

all : $(EXE)
	$(CC) $(SRCS) -o $(EXE)
	
clean :
	rm -f $(OBJS) $(EXE)