SRCDIR=./src
OBJDIR=./build

default: $(SRCDIR)/main.c
	gcc -Wall $(SRCDIR)/main.c -o $(OBJDIR)/a.exe
