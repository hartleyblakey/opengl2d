project: project.c glad.c Rendering/shader.c Rendering/shader.h
	gcc project.c glad.c Rendering/shader.c -o project -lwayland-client -lm Libraries/lib/libglfw3.a -I./Libraries/include -I./Rendering
