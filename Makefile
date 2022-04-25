all:
	gcc -c allegro.c -lallegro -lallegro_font -lallegro_image -lallegro_ttf
	gcc -c cenario.c -lallegro -lallegro_image -lallegro_ttf
	gcc -c player.c -lallegro
	gcc -c game.c -lallegro
	gcc -Wall main.c game.o player.o cenario.o allegro.o -lallegro -lallegro_font -lallegro_image -lallegro_ttf -o main
