CC = g++
EXEC = Chess
FLAGS = -Wall -Werror -pedantic
LIBS = -lsfml-graphics -lsfml-window -lsfml-system
STD = -std=c++17
S = src
O = obj
B = bin

all: $(O) $(B) $(O)/Main.o
	$(CC) $(O)/*.o -o $(B)/$(EXEC) $(LIBS)

$(O)/Main.o: $(O)/Engine.o
	$(CC) $(S)/Main.cpp -c -o $(O)/Main.o $(STD) $(FLAGS)

$(O)/Engine.o: $(O)/Board.o $(O)/Promotion.o
	$(CC) $(S)/sfmlGame/Engine.cpp -c -o $(O)/Engine.o $(STD) $(FLAGS)

$(O)/Promotion.o: $(O)/Piece.o
	$(CC) $(S)/sfmlGame/Promotion.cpp -c -o $(O)/Promotion.o $(STD) $(FLAGS)

$(O)/Board.o: $(O)/Piece.o
	$(CC) $(S)/sfmlGame/Board.cpp -c -o $(O)/Board.o $(STD) $(FLAGS)

$(O)/Piece.o:
	$(CC) $(S)/sfmlGame/Piece.cpp -c -o $(O)/Piece.o $(STD) $(FLAGS)

clean: $(O) $(B)
	rm -r $(O)
	rm -r $(B)

$(O):
	mkdir $(O)

$(B):
	mkdir $(B)
