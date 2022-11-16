headers :=


source :=


shaders := Resources/Shaders/default.vert \
	   Resources/Shaders/default.frag


hlibh := Libraries/include/cglm/cglm.h \
	 Libraries/include/stb_image.h \
	 Libraries/include/glad/glad.h \
	 Rendering/shader.h


hlibc := Libraries/include/stb_image.c \
	 glad.c \
	 Rendering/shader.c


hlibo :=  stb_image.o glad.o cglm.o shader.o mesh.o glerrors.o texture.o project.o

IFLAGS :=  -I./Libraries/include -I./Rendering -I./Utils

LFLAGS := -lm Libraries/lib/libglfw3.a -lwayland-client

project: $(source) $(headers) $(shaders) $(hlibo) makefile
	gcc $(source) $(hlibo) -o project $(LFLAGS) $(IFLAGS)

project.o: project.c
	gcc -c -o project.o project.c $(IFLAGS)

cglm.o: Libraries/include/cglm/cglm.h Libraries/include/cglm.c
	gcc -c -o cglm.o Libraries/include/cglm.c

stb_image.o: Libraries/include/stb_image.h
	gcc -c -o stb_image.o Libraries/include/stb_image.c

glad.o: glad.c Libraries/include/glad/glad.h
	gcc -c -o glad.o glad.c

shader.o: Rendering/shader.h Rendering/shader.c
	gcc -c -o shader.o Rendering/shader.c $(IFLAGS)

mesh.o: Rendering/mesh.h Rendering/mesh.c
	gcc -c -o mesh.o Rendering/mesh.c $(IFLAGS)

glerrors.o: Utils/glerrors.h Utils/glerrors.c
	gcc -c -o glerrors.o Utils/glerrors.c $(IFLAGS)

texture.o: Rendering/texture.h Rendering/texture.c
	gcc -c -o texture.o Rendering/texture.c $(IFLAGS)

run: project
	./project

cr: project
	clear
	./project
