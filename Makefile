LIB_DIR = lib/
CFLAGS = -Wall -Wextra -I $(LIB_DIR)
LFLAGS = -lm -lSDL2 -L $(LIB_DIR)

%.out: %.c
	gcc $< -o $@ $(CFLAGS) $(LFLAGS)

%.gif.out: %.c $(LIB_DIR)/GIFdef.h $(LIB_DIR)/msf_gif.h
	gcc $< -o $@ $(CFLAGS) $(LFLAGS) -DGIF='"$*.gif"'

%.gif: %.gif.out
	$< && rm $<
