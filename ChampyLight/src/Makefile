CC      = gcc
CFLAGS  = -Wall -Werror -pedantic -std=c99 -D_BSD_SOURCE #-D_POSIX_SOURCE
LIB     = -lcurl

TARGET  = champylight
OBJECTS = champylight.o dmx.o utils.o curlClient.o dmxOperations.o

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIB) 

clean:
	rm -rf $(TARGET) $(OBJECTS)
