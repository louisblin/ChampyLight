CC=g++
DEAMONOBJS=dmxd.o
LIBOJBS=dmx.o
OBJS=$(LIBOBJS) $(DEAMONOBJS)
DEAMONBIN=dmxd.bin
LIB=libdmx.a
CFLAGS+=
LDFLAGS+=-lusb -lm
INCLUDES+=-I./

all: $(LIB) $(DEAMONBIN) $(TESTBIN) 

%.o: %.c
	@rm -f $@ 
	$(CC) $(CFLAGS) $(INCLUDES) -g -c $< -o $@ -Wno-deprecated-declarations

dmxd.bin: $(DEAMONOBJS)
	$(CC) -o $@ -Wl,--whole-archive $(DEAMONOBJS) $(LDFLAGS) -Wl,--no-whole-archive -rdynamic
	mv dmxd.bin ../deamon


%.a: $(LIBOJBS)
	$(AR) r $@ $^
	mv $(LIB) ../lib
	cp dmx.h ../include

clean:
	for i in $(OBJS); do (if test -e "$$i"; then ( rm $$i ); fi ); done
	@rm -f dmxd.bin $(LIB)