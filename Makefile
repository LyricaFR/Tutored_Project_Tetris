# $@ : the current target
# $^ : the current prerequisites
# $< : the first current prerequisite

CC = gcc
CFLAGS = -Wall -ansi -std=gnu11
LDFLAGS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer
SRC_DIR = src
HEADER_DIR = include
BIN_DIR = bin
OBJ = Main.o Block.o Piece.o Queue.o Game.o Versus.o Graphic.o Rotation.o Movement.o Input.o AI.o Audio.o

Tetris: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
	mv $(OBJ) $(BIN_DIR)
	mv $@ $(BIN_DIR)

Main.o: $(SRC_DIR)/Main.c
	$(CC) -c $< $(CFLAGS) $(LDFLAGS)

Block.o: $(SRC_DIR)/Block.c $(HEADER_DIR)/Block.h
	$(CC) -c $< $(CFLAGS) $(LDFLAGS)

Piece.o: $(SRC_DIR)/Piece.c $(HEADER_DIR)/Piece.h
	$(CC) -c $< $(CFLAGS) $(LDFLAGS)

Queue.o: $(SRC_DIR)/Queue.c $(HEADER_DIR)/Queue.h
	$(CC) -c $< $(CFLAGS) $(LDFLAGS)

Game.o: $(SRC_DIR)/Game.c $(HEADER_DIR)/Game.h
	$(CC) -c $< $(CFLAGS) $(LDFLAGS)

Versus.o: $(SRC_DIR)/Versus.c $(HEADER_DIR)/Versus.h
	$(CC) -c $< $(CFLAGS) $(LDFLAGS)

Graphic.o: $(SRC_DIR)/Graphic.c $(HEADER_DIR)/Graphic.h
	$(CC) -c $< $(CFLAGS) $(LDFLAGS)

Rotation.o: $(SRC_DIR)/Rotation.c $(HEADER_DIR)/Rotation.h
	$(CC) -c $< $(CFLAGS) $(LDFLAGS)

Movement.o: $(SRC_DIR)/Movement.c $(HEADER_DIR)/Movement.h
	$(CC) -c $< $(CFLAGS) $(LDFLAGS)

Input.o: $(SRC_DIR)/Input.c $(HEADER_DIR)/Input.h
	$(CC) -c $< $(CFLAGS) $(LDFLAGS)
	
AI.o: $(SRC_DIR)/AI.c $(HEADER_DIR)/AI.h
	$(CC) -c $< $(CFLAGS) $(LDFLAGS)

Audio.o: $(SRC_DIR)/Audio.c $(HEADER_DIR)/Audio.h
	$(CC) -c $< $(CFLAGS) $(LDFLAGS)

clean:
	cd $(BIN_DIR) && rm -f $(OBJ)
	rm -f *.o

cleanall:
	cd $(BIN_DIR) && rm -f *
	rm -f *.o