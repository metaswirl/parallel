all: game_of_life

game_of_life:
	gcc *.c -o game_of_life

clean:
	rm game_of_life
