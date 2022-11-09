project: project.c glad.c Rendering/shader.c Rendering/shader.h Libraries/include/cglm/cglm.h
	gcc project.c glad.c Rendering/shader.c Libraries/include/cglm/cglm.h -o project -lwayland-client -lm Libraries/lib/libglfw3.a -I./Libraries/include -I./Rendering -I./Utils
