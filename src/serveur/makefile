CC = gcc
LDFLAGS = -lpthread
CFLAGS = -Wall -std=c99

SRC = jeux.c serveur.c
OBJ = $(SRC:.c=.o)

EXEC = serveur

all: clean serveur

serveur: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	
%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@
		
clean:
	rm -f *.o
	
mrproper: clean
	rm $(EXEC)
