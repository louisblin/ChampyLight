CdC=gcc

TARGETS=deamon values_handler

all: $(TARGETS)
	for i in $(TARGETS); \
	do ( cd $$i/src; make; cd ../..; ); done

clean:
	for i in $(TARGETS); \
	do ( cd $$i/src; make clean; cd ../..; ); done

