SOURCES=shell.o main.o nut.o util.o
CFLAGS += -Wall -Wextra -ggdb -I. -rdynamic
LDFLAGS += -ldl
BIN=nutshell

ifeq ($(READLINE), 1)
CFLAGS += -DWITH_LIBREADLINE
LDFLAGS += -lreadline
endif

all: $(BIN)

$(BIN): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) $(LDFLAGS) -o $(BIN)

clean:
	rm -rf $(SOURCES) $(BIN)
