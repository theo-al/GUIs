CFLAGS = -Wall -Wextra -I lib/
LFLAGS = -lm -lSDL2 -lSDL2_gfx

ifneq ($(shell command -v pkg-config;),)
    CFLAGS += $(shell pkg-config --cflags sdl2)
endif

%.out: %.c
	gcc $< -o $@ $(CFLAGS) $(LFLAGS)

%.gif.out: %.c lib/gifdef.h lib/msf_gif.h
	gcc $< -o $@ $(CFLAGS) $(LFLAGS) -DNOME_GIF=\"$*.gif\"

%.gif: %.gif.out
	$< && rm $<
