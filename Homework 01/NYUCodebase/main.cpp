#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "Matrix.h"
#include <OpenGL/gl.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

//functino to load a texture with certian file name
//note to access it, add the image to the NYUCodebase folder
GLuint LoadTexture(const char *filePath) {
    int w,h,comp;
    unsigned char* image = stbi_load(filePath, &w, &h, &comp, STBI_rgb_alpha);
    
    if (image == NULL) {
        std::cout << "Unable to load image.\n";
        assert(false);
    }
    
    GLuint retTexture;
    glGenTextures(1,& retTexture);
    glBindTexture(GL_TEXTURE_2D, retTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    stbi_image_free(image);
    return retTexture;
    
}

int main(int argc, char *argv[])
{
    
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Chandrika's Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 360);
    ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GLuint planeGreenTexture = LoadTexture(RESOURCE_FOLDER"planeGreen1.png");
    GLuint coloredLandTexture = LoadTexture(RESOURCE_FOLDER"colored_land.png");
    GLuint spaceshipTexture = LoadTexture(RESOURCE_FOLDER"playerShip3_blue.png");
    GLuint spacelightTexture = LoadTexture(RESOURCE_FOLDER"fire15.png");
    
    //background
    Matrix projectionMatrix0;
    Matrix modelMatrix0;
    Matrix viewMatrix0;
    modelMatrix0.Scale(2.0, 2.0, 0.0);
    
    
    //green cart
    Matrix projectionMatrix;
    Matrix modelMatrix;
    Matrix viewMatrix;
    modelMatrix.Scale(0.5, 0.5, 0.0);
    modelMatrix.Translate(0, -0.5, 0);
    
    //green spaceship
    Matrix projectionMatrix2;
    Matrix modelMatrix2;
    Matrix viewMatrix2;
    modelMatrix2.Scale(0.5, 0.5, 0);
    modelMatrix2.Translate(-3, 1.0, 0);

    //spacelight
    Matrix projectionMatrix3;
    Matrix modelMatrix3;
    Matrix viewMatrix3;
    modelMatrix3.Scale(0.5, 0.5,0);
    modelMatrix3.Translate(-3, 0, 0);
    
    projectionMatrix.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    float lastFrameTicks = 0.0f;
    
    
    SDL_Event event;
    bool done = false;
    while (!done) {
        float ticks = (float)SDL_GetTicks()/1000.0f;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        
        float angle = 0;
        angle+=elapsed;
        
        
        //event loop
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
        }
        
        glClear(GL_COLOR_BUFFER_BIT);

        
        //creating colored_land.png as the background
        program.setModelMatrix(modelMatrix0);
        program.setProjectionMatrix(projectionMatrix0);
        program.setViewMatrix(viewMatrix0);
        glBindTexture(GL_TEXTURE_2D, coloredLandTexture);
        float vertices0[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices0);
        glEnableVertexAttribArray(program.positionAttribute);
        float texCoords0[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
        glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords0);
        glEnableVertexAttribArray(program.texCoordAttribute);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(program.positionAttribute);
        glDisableVertexAttribArray(program.texCoordAttribute);
        
        
        //creating planeGreen.png in the center
        program.setModelMatrix(modelMatrix);
        program.setProjectionMatrix(projectionMatrix);
        program.setViewMatrix(viewMatrix);
        //move from side to middle of the screen
        if (ticks > 12.4 && ticks<20) {
            modelMatrix.Translate(0, 1*angle, 0);
            modelMatrix.Scale(0.99, 0.99, 0.5);
        }
        
        if (ticks > 6.1 && ticks < 12.4) {
            modelMatrix.Rotate(-angle);
        }
        
        
        
        
        glBindTexture(GL_TEXTURE_2D, planeGreenTexture);
        float vertices[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
        glEnableVertexAttribArray(program.positionAttribute);
        float texCoords[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
        glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
        glEnableVertexAttribArray(program.texCoordAttribute);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(program.positionAttribute);
        glDisableVertexAttribArray(program.texCoordAttribute);
        

        //creating playerSpaceship3_blue.png in the center
        program.setModelMatrix(modelMatrix2);
        program.setProjectionMatrix(projectionMatrix2);
        program.setViewMatrix(viewMatrix2);
        if (ticks < 6) {
            modelMatrix2.Translate(elapsed*0.5, 0, 0);
        }
        if (ticks > 18) {
            modelMatrix2.Translate(0, elapsed*1, 0);
        }
    
        
        
        glBindTexture(GL_TEXTURE_2D, spaceshipTexture);
        float vertices2[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices2);
        glEnableVertexAttribArray(program.positionAttribute);
        float texCoords2[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
        glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords2);
        glEnableVertexAttribArray(program.texCoordAttribute);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(program.positionAttribute);
        glDisableVertexAttribArray(program.texCoordAttribute);
        
        
        //creating spacelight from the spaceship
        program.setModelMatrix(modelMatrix3);
        program.setProjectionMatrix(projectionMatrix3);
        program.setViewMatrix(viewMatrix3);
        if (ticks < 6) {
            modelMatrix3.Translate(elapsed*0.5, 0, 0);
        }
        
        if (ticks > 17) {
            //modelMatrix.Translate(0, 1*angle, 0);
            modelMatrix3.Scale(0.9, 0.9, 2);
        }
        
        glBindTexture(GL_TEXTURE_2D, spacelightTexture);
        float vertices3[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices3);
        glEnableVertexAttribArray(program.positionAttribute);
        float texCoords3[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
        glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords3);
        glEnableVertexAttribArray(program.texCoordAttribute);
        
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(program.positionAttribute);
        glDisableVertexAttribArray(program.texCoordAttribute);
        
        
        
        SDL_GL_SwapWindow(displayWindow);
        
    }
    
    SDL_Quit();
    return 0;
}
