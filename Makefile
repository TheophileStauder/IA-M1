# Warnings
CFLAGS += -Wall 

# Libmath
LIBS += -lm 


puissance4_executable: puissance4.o
	$(CC) $(CFLAGS) puissance4.o  -o puissance4 $(LIBS)

puissance4.o: puissance4.c
	$(CC) $(CFLAGS) -c puissance4.c 

clean: 
	rm *.o 
	rm puissance4
