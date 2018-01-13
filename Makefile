#!/usr/bin/make -f

.DEFAULT_GOAL = build
.PHONY = clean build debug

TARGET := usr/bin/inet_addrs
BUILD  := build
OBJS   := $(BUILD)/inet_addrs.o

LDFLAGS := -Wall -Wextra -Werror -pedantic $(LDFLAGS)
CFLAGS  := -std=c11 -D_GNU_SOURCE -D_FORTIFY_SOURCE=2 -Wall -Wextra -Werror -pedantic -Isrc/ $(CFLAGS)

build: CFLAGS += -O2 -s
build: LDFLAGS += -O2 -s
build: $(TARGET)

debug: CFLAGS += -O0 -g -DDEBUG
debug: LDFLAGS += -O0 -g
debug: $(TARGET)

static: CFLAGS += -Os -s
static: LDFLAGS += -Os -s -static
static: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -vp $(dir $@)
	$(CC) $(LDFLAGS) -o $@ $<

build/%.o : src/%.c
	@mkdir -vp $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

manifest:
	true

clean:
	rm -Rf $(OBJS) $(TARGET)
	-rmdir --parents --ignore-fail-on-non-empty --verbose $(BUILD) $(dir $(TARGET))
