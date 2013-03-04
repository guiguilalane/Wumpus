CC = gcc
LDFLAGS = -lpthread
CXXFLAG =

SRC = jeux.c serveur.c
OBJ = $(SRC:.c=.o)

EXEC = serveur

serveur: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)
	
%.o: %.c
	$(CC) -c $^ -o $@
		
clean:
	rm -f *.o
	
mrproper: clean
	rm $(EXEC)
