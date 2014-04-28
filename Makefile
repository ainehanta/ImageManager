TARGET = imgman
SRCS = main.c DxMock.c ImageManager.c
OBJS = $(SRCS:.c=.o)

CC = gcc

all : $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

.c.o :
	$(CC) -c $<

clean :
	rm -f *.o $(TARGET)

DxMock.c : DxMock.h
ImageManager.c : ImageManager.h
