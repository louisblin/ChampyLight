CC      = gcc
CFLAGS  = -Wall -Werror -pedantic -std=c99 -D_BSD_SOURCE -lcurl #-D_POSIX_SOURCE

TARGET  = champylight
OBJECTS = champylight.o dmx.o utils.o curlClient.o

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(TARGET) $(OBJECTS)
