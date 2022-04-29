OBJETOS = allegro.o cenario.o player.o inimigo.o game.o
DEPENDENCIAS = -lallegro -lallegro_font -lallegro_image -lallegro_ttf -lallegro_audio -lallegro_acodec

all: main

# Executavel
main: $(OBJETOS) main.c
	gcc -Wall main.c $(OBJETOS) $(DEPENDENCIAS) -o main

# Arquivos objeto
allegro.o: allegro.c allegro.h
	gcc -c -Wall allegro.c

cenario.o: cenario.c cenario.h
	gcc -c -Wall cenario.c

player.o: player.c player.h
	gcc -c -Wall player.c

inimigo.o: inimigo.h inimigo.c
	gcc -c -Wall inimigo.c

game.o: allegro.h cenario.h player.h inimigo.h game.c
	gcc -c -Wall game.c


# Limpeza de arquivos
clean:
	rm -f *.o

purge: clean
	rm -f main