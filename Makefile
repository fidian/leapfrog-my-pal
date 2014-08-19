all: read_flash wipe_flash

read_flash: read_flash.c
	$(CC) -o $@ $?

wipe_flash: wipe_flash.c
	$(CC) -o $@ $?
