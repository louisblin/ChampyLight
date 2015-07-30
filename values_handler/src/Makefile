CC      = gcc
CFLAGS  = -Wall -Werror -pedantic -std=c99 -D_XOPEN_SOURCE -D_BSD_SOURCE #-D_POSIX_SOURCE
LDFLAGS = -lm
LIB     = -lcurl

TARGET  = champylight
OBJECTS = champylight.o dmx.o utils.o curlClient.o dmxOperations.o

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIB) 

clean:
	rm -rf $(TARGET) $(OBJECTS)
