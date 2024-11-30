CC=gcc
CFLAGS=\
	-std=gnu17 \
	-pedantic-errors \
	-Wall \
	-Wextra \
	-Wunused \
	-Wuninitialized \
	-MD \
	-g

OUTPUT=server

# Do not edit below
################################################################################

CFILES := $(shell cd src && find -L * -type f -name '*.c' | LC_ALL=C sort)
CFILE_DEPS := $(addprefix obj/, $(CFILES:.c=.c.d))
OBJS := $(addprefix obj/, $(CFILES:.c=.c.o))

.PHONY: all
all: bin/$(OUTPUT)

bin/$(OUTPUT): bin obj $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

-include $(CFILE_DEPS)

obj/%.c.o: src/%.c
	$(CC) $(CFLAGS) $(LIBS) -c $< -o $@

bin obj:
	mkdir -p $@

.PHONY: clean
clean:
	@rm -rfv bin obj
