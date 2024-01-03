# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall

# Lista dos source-files
SOURCES = lexer.l parser.y main.c

# Nome do compilador (ficheiro executável)
EXECUTABLE = sigma

#-------------------------------------------------------------------------
# make
all: $(EXECUTABLE)

$(EXECUTABLE): lex.yy.c parser.tab.c main.c
	$(CC) $(CFLAGS) lex.yy.c parser.tab.c main.c -o $(EXECUTABLE) -lfl

lex.yy.c: lexer.l
	flex lexer.l

parser.tab.c: parser.y
	bison -d parser.y
#-------------------------------------------------------------------------
# make run
run: $(EXECUTABLE)
	./$(EXECUTABLE)
#-------------------------------------------------------------------------
# make clean
clean:
	rm -f lex.yy.c parser.tab.c parser.tab.h $(EXECUTABLE)
#-------------------------------------------------------------------------