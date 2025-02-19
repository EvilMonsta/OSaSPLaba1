CC = gcc
CFLAGS = -Wall
OUT_DIR = ../../
OUTPUT = dirwalk

$(OUT_DIR)$(OUTPUT): lab1.c
	$(CC) $(CFLAGS) -o $(OUT_DIR)$(OUTPUT) lab1.c

clean: 
	rm -f $(OUT_DIR)$(OUTPUT)
