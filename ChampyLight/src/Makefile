CC      = gcc
CFLAGS  = -Wall -Werror -pedantic -std=c99

TARGET  = champylight
OBJECTS = champylight.o dmx.o utils.o

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(TARGET) $(OBJECTS)
