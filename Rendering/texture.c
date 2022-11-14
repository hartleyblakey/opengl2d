#include "texture.h"
#include "glerrors.h"

Texture Upload_Texture_2D(const char* filename)
{
    printf("function was called\n");
    Texture tex;
    tex.type = GL_RGB;
    tex.id = 0;

    printf("%s\n", filename);
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1==1);
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 0);
    if (data)
    {
        printf("Width, Height, Channels: %i %i %i\n",width,height,channels);
        printf("stbi loaded data\n");
        switch(channels)
        {
            case 1:
                tex.type = GL_RED;
                break;
            case 2:
                tex.type = GL_RG;
                break;
            case 3:
                tex.type = GL_RGB;
                printf("GL_RGB\n");
                break;
            case 4:
                tex.type = GL_RGBA;
                printf("GL_RGBA\n");
                break;
        }
        printf("%p\n",&tex.id);
        printf("switch statement went through\n");

        PERROR();
        printf("%p\n",&tex.id);
        glGenTextures(1, &tex.id);

        printf("generated textures\n");

        glBindTexture(GL_TEXTURE_2D, tex.id);
        printf("bound textures\n");

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        printf("set params\n");
    //                             output type             source type
        glTexImage2D(GL_TEXTURE_2D, 0, tex.type, width, height, 0, tex.type, GL_UNSIGNED_BYTE, data);
        printf("uploaded data\n");
        glGenerateMipmap(GL_TEXTURE_2D);

        //TEXTURES[TEXTURE_COUNT] = tex;
        //TEXTURE_COUNT++;
    }
    else
    {
        stbi_image_free(data);
        printf("\nERROR: Image upload failed\n");
        exit(4);
    }
    printf("end of upload\n");
    stbi_image_free(data);
    return tex;
}
