#include "glerrors.h"

unsigned int TOTAL_ERRORS = 0;

void PERROR()
{
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        // Process/log the error.
        printf("OPENGL ERROR: %i\n", err);
        TOTAL_ERRORS++;
        if(TOTAL_ERRORS > 10)
            exit(6);
    }
}
