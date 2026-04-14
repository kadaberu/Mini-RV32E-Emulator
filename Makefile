CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99
TARGET = minirvemu
SRC = minirvEUM.c


all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)


clean:
	rm -f $(TARGET)