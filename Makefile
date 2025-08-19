%.out: %.c
	gcc $< -o $@ -lSDL2

