#ifndef MESH_H
#define MESH_H
#include <GL/glew.h>        // GLEW library
//#include <GLFW/glfw3.h>     // GLFW library

struct GLMesh {
    GLuint vao;         // Handle for the vertex array object
    GLuint vbos[2];     // Handles for the vertex buffer objects
    GLuint nIndices;    // Number of indices of the mesh
};

class Mesh {
public: 
    
    // Declare Mesh create and destroy functions
    void static CreateMesh(GLMesh &mesh);
    void static DestroyMesh(GLMesh &mesh);
};

#endif
