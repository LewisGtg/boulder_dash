all:
	gcc -c player.c
	gcc -Wall main.c player.o -lallegro -lallegro_font -lallegro_image -o main
