SRCDIR=./src
OBJDIR=./build
CFLAGS=-c -Wall

default: main.o load_reference.o process_macro.o
	gcc -Wall $(OBJDIR)/*.o -o $(OBJDIR)/a.exe

main.o: $(SRCDIR)/main.c
	gcc $(CFLAGS) $(SRCDIR)/main.c -o $(OBJDIR)/main.o

load_reference.o: $(SRCDIR)/load_reference.c
	gcc $(CFLAGS) $(SRCDIR)/load_reference.c -o $(OBJDIR)/load_reference.o

process_macro.o: $(SRCDIR)/process_macro.c
	gcc $(CFLAGS) $(SRCDIR)/process_macro.c -o $(OBJDIR)/process_macro.o

clean:
	rm ./build/*.o a.exe
