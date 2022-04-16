all:
	gcc -c player.c -lallegro
	gcc -c cenario.c -lallegro -lallegro_image
	gcc -Wall main.c player.o cenario.o -lallegro -lallegro_font -lallegro_image -o main
