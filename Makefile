all:
	gcc -c allegro.c -lallegro -lallegro_font -lallegro_image
	gcc -c cenario.c -lallegro -lallegro_image
	gcc -c player.c -lallegro
	gcc -Wall main.c player.o cenario.o allegro.o -lallegro -lallegro_font -lallegro_image -o main
