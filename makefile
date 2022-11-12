headers := Rendering/shader.h Libraries/include/cglm/cglm.h Rendering/mesh.h Utils/glerrors.h
source := project.c glad.c Rendering/shader.c Rendering/mesh.c Utils/glerrors.c
shaders := Resources/Shaders/default.vert Resources/Shaders/default.frag

project: $(source) $(headers) $(shaders)
	gcc $(source) $(headers) -o project -lwayland-client -lm Libraries/lib/libglfw3.a -I./Libraries/include -I./Rendering -I./Utils
