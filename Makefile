%.out: %.c
	gcc $< -o $@ -lSDL2 -lm -Wall -Wextra

