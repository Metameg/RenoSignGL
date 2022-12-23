
#ifndef MESH_H
#define MESH_H
#include <glad/glad.h>
#include "Cylinder.h"
#include "IcoSphere.h"
//#include <GL/glew.h>        // GLEW library
#include <iostream>
struct GLMesh {
    GLuint vao;         // Handle for the vertex array object
    GLuint vbos[3];     // Handles for the vertex buffer objects
    GLuint nIndices;    // Number of indices of the mesh
    unsigned int cylIndices[240];
    unsigned int icoIndices[240];
    unsigned int prismIndices[352];
    
};

class Mesh {
public:
    // Declare Mesh create and destroy functions
    void static DestroyMesh(GLMesh& mesh);


protected:
    void static setupMesh();
    static void flipImageVertically(unsigned char* image, int width, int height, int channels);
};

class PlaneMesh : public Mesh {
public:
    void static CreateMesh(GLMesh& mesh);
};

class PyramidMesh : public Mesh {
public:
    void static CreateMesh(GLMesh& mesh);
};

class CubeMesh : public Mesh {
public:
    void static CreateMesh(GLMesh& mesh);
};

class CylinderMesh : public Mesh {
public:
    void static CreateMesh(GLMesh& mesh);
    
    
};

class PrismMesh : public Mesh {
public:
    void static CreateMesh(GLMesh& mesh);
    
    
};

class IcosphereMesh : public Mesh {
public:
    void static CreateMesh(GLMesh& mesh);
};




#endif
