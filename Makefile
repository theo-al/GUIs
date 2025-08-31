ALL = $(shell ls ex/)
SRC = $(patsubst %, %/main.c, $(ALL))
OBJ = $(patsubst %.c, %.out, $(SRC))

CFLAGS = -Wall -Wextra -I lib/
LFLAGS = -lm -lSDL2 -lSDL2_gfx

ifneq ($(shell command -v pkg-config;),)
    CFLAGS += $(shell pkg-config --cflags sdl2)
endif

all: $(ALL)
$(ALL): %: ex/%/main.out ex/%/main.gif

%.out: %.c
	gcc $< -o $@ $(CFLAGS) $(LFLAGS)

%.gif.out: %.c lib/gifdef.h lib/msf_gif.h
	gcc $< -o $@ $(CFLAGS) $(LFLAGS) -DNOME_GIF=\"$*.gif\"

%.gif: %.gif.out
	$< && rm $<

.PHONY: all $(ALL)
