headers := Rendering/mesh.h \
	   Utils/glerrors.h \
	   Rendering/texture.h


source := project.c \
	  Rendering/mesh.c \
	  Utils/glerrors.c \
	  Rendering/texture.c


shaders := Resources/Shaders/default.vert \
	   Resources/Shaders/default.frag


hlibh := Libraries/include/cglm/cglm.h \
	 Libraries/include/stb_image.h \
	 Libraries/include/glad/glad.h \
	 Rendering/shader.h


hlibc := Libraries/include/stb_image.c \
	 glad.c \
	 Rendering/shader.c


hlibo :=  stb_image.o glad.o cglm.o shader.o

INCLUDE_FLAGS := -lwayland-client -I./Libraries/include -I./Rendering -I./Utils

project: $(source) $(headers) $(shaders) $(hlibo) makefile
	gcc $(source) $(hlibo) Libraries/include/cglm/cglm.h -o project \
	-lm Libraries/lib/libglfw3.a $(INCLUDE_FLAGS)

cglm.o: Libraries/include/cglm/cglm.h Libraries/include/cglm.c
	gcc -c -o cglm.o Libraries/include/cglm.c

stb_image.o: Libraries/include/stb_image.h
	gcc -c -o stb_image.o Libraries/include/stb_image.c

glad.o: glad.c Libraries/include/glad/glad.h
	gcc -c -o glad.o glad.c

shader.o: Rendering/shader.h Rendering/shader.c
	gcc -c -o shader.o Rendering/shader.c $(INCLUDE_FLAGS)




run: project
	./project
