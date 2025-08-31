EXS = $(shell ls ex/)
ALL = $(patsubst %, ex/%/, $(EXS))

CFLAGS = -Wall -Wextra -I lib/
LFLAGS = -lm -lSDL2 -lSDL2_gfx

ifneq ($(shell command -v pkg-config;),)
    CFLAGS += $(shell pkg-config --cflags sdl2)
endif

all: $(EXS)
$(EXS): %: ex/%/
$(ALL): %/: %/main.out %/main.gif

%.out: %.c
	gcc $< -o $@ $(CFLAGS) $(LFLAGS)

%.gif.out: %.c lib/gifdef.h lib/msf_gif.h
	gcc $< -o $@ $(CFLAGS) $(LFLAGS) -DNOME_GIF=\"$*.gif\"

%.gif: %.gif.out
	$< && rm $<


.PHONY: all $(ALL) $(EXS)

# compilar códigos extras ilegalm̃
include Args.mk
