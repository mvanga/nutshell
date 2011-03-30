obj-m = core.o

MODULE_NAME = core

CFLAGS+=-Wall -fPIC -I../.. -ggdb

all: $(obj-m)
	$(CC) -shared -Wl,-soname,$(MODULE_NAME).nut -o $(MODULE_NAME).nut $^

install:
	cp core.nut ../..

clean:
	rm -rf $(MODULE_NAME).nut $(MODULE_NAME).o
