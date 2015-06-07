CC      = gcc
CFLAGS  = -Wall -Werror -pedantic -std=c99 -D_BSD_SOURCE #-D_POSIX_SOURCE

TARGET  = champylight
OBJECTS = champylight.o dmx.o utils.o tcpclient.o

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(TARGET) $(OBJECTS)
