CC = gcc
CFLAGS = -O3
TARGET = y9read
SRC = main.c common.c files.c
HEADERS = common.h files.h

all: $(TARGET)

$(TARGET): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
