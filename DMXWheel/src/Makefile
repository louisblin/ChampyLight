CC=g++
CFLAGS+=

LDFLAGS+= -lgtk-3 -lgdk-3 -latk-1.0 -lgio-2.0 -lpangocairo-1.0 \
          -lgdk_pixbuf-2.0 -lcairo-gobject -lpango-1.0 -lcairo \
          -lgobject-2.0 -lglib-2.0 -lusb -lm \
          -L../../dmx/lib -ldmx

INCLUDES+= -I./ -I../../dmx/include \
           -I/usr/include/glib-2.0 -I/usr/include/gtk-3.0 \
           -I/usr/lib/arm-linux-gnueabihf/glib-2.0/include \
           -I/usr/include/pango-1.0 -I/usr/include/cairo \
           -I/usr/include/pixman-1 -I/usr/include/gdk-pixbuf-2.0 \
           -I/usr/include/atk-1.0 

OBJS=DMXWheel.o
BIN=DMXWheel.bin

all: $(BIN)

%.o: %.c
	@rm -f $@ 
	$(CC) $(CFLAGS) $(INCLUDES) -g -c $< -o $@ -Wno-deprecated-declarations

%.bin: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS) -Wl,--no-whole-archive -rdynamic
	@cp $@ ../bin

clean:
	for i in $(OBJS); do (if test -e "$$i"; then ( rm $$i ); fi ); done
	@rm -f $(BIN)