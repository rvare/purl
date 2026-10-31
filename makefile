SRCDIR=./src
OBJDIR=./build
CFLAGS=-c
CC=gcc -Wall

default: ./build/main.o ./build/load_reference.o ./build/process_macro.o
	$(CC) $(OBJDIR)/*.o -o $(OBJDIR)/purl

./build/main.o: $(SRCDIR)/main.c
	$(CC) $(CFLAGS) $(SRCDIR)/main.c -o $(OBJDIR)/main.o

./build/load_reference.o: $(SRCDIR)/load_reference.c
	$(CC) $(CFLAGS) $(SRCDIR)/load_reference.c -o $(OBJDIR)/load_reference.o

./build/process_macro.o: $(SRCDIR)/process_macro.c
	$(CC) $(CFLAGS) $(SRCDIR)/process_macro.c -o $(OBJDIR)/process_macro.o

clean:
	rm ./build/*.o purl.exe
