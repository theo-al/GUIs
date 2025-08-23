CFLAGS = -Wall -Wextra -I $(LIB_DIR)
LFLAGS = -lm -lSDL2 -L $(LIB_DIR)

%.out: %.c
	gcc $< -o $@ $(CFLAGS) $(LFLAGS)

%.gif.out: %.c lib/gifdef.h lib/msf_gif.h
	gcc $< -o $@ $(CFLAGS) $(LFLAGS) -DNOME_GIF='"$*.gif"'

%.gif: %.gif.out
	$< && rm $<
