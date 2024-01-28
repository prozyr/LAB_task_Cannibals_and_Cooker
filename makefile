CCOPT=-DLINUX
CC=gcc
#CC=/opt/gnu/bin/gcc

all: msga msgb sema semb forki cannibals

clean: 
	rm msg[ab] sem[ab] forki

msg: msga msgb

msga: msga.c params.h
	$(CC) $(CCOPT) msga.c -o msga

msgb: msgb.c params.h
	$(CC) $(CCOPT) msgb.c -o msgb

sem: sema semb

sema: sema.c params.h
	$(CC) $(CCOPT) sema.c -o sema

semb: semb.c params.h
	$(CC) $(CCOPT) semb.c -o semb

forki: forki.c
	$(CC) $(CCOPT) forki.c -o forki

cannibal: cannibals.c
	$(CC) $(CCOPT) cannibals.c -o cannibals

