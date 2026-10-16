all: Flood-It

API_Gene_instance.o: API_Gene_instance.c API_Gene_instance.h
	gcc -c API_Gene_instance.c `sdl2-config --cflags`

API_Grille.o: API_Grille.c API_Grille.h
	gcc -c API_Grille.c `sdl2-config --cflags`

Liste_case.o: Liste_case.c Liste_case.h
	gcc -c Liste_case.c `sdl2-config --cflags`

Flood-It.o: Flood-It.c
	gcc -c Flood-It.c `sdl2-config --cflags`

S_Zsg.o: S_Zsg.c S_Zsg.h Liste_case.h
	gcc -c S_Zsg.c `sdl2-config --cflags`

Flood-It: Flood-It.o Liste_case.o API_Grille.o API_Gene_instance.o S_Zsg.o
	gcc -o Flood-It Flood-It.o Liste_case.o API_Grille.o API_Gene_instance.o S_Zsg.o `sdl2-config --libs`

clean:
	rm -f *.o Flood-It
