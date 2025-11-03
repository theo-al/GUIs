EXS := $(filter-out 0.2, $(shell ls ex/))
ALL := $(patsubst %, ex/%/, $(EXS))
GIF := $(patsubst %, %/main.gif, $(ALL))

CFLAGS = -Wall -Wextra -I lib/
LFLAGS = -lm -lSDL2 -lSDL2_gfx -lSDL2_image

ifneq ($(shell command -v pkg-config;),)
    CFLAGS += $(shell pkg-config --cflags sdl2)
endif

all: $(EXS)
gifs: $(GIF)

$(EXS): %: ex/%/
$(ALL): %/: %/main.out %/main.gif.out

%.out: %.c lib/cores.h lib/AUX.h lib/TFX.h
	gcc $< -o $@ $(CFLAGS) $(LFLAGS)

%.gif.out: %.c lib/gifdef.h lib/msf_gif.h
	gcc $< -o $@ $(CFLAGS) $(LFLAGS) -DNOME_GIF=\"$*.gif\"

%.gif: %.gif.out
	$< && rm $<


.PHONY: all gifs $(ALL) $(EXS)

# bruxaria pra extras
include Args.mk
