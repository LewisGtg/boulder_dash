all:
	gcc -c allegro.c 
	gcc -c cenario.c 
	gcc -c player.c 
	gcc -c game.c 
	gcc -Wall main.c game.o player.o cenario.o allegro.o -lallegro -lallegro_font -lallegro_image -lallegro_ttf -o main
