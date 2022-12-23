#include "GLMesh.h"
#include <iostream>
#include <glm/glm.hpp>


void PlaneMesh::CreateMesh(GLMesh& mesh)
{
    // For drawing using indices

    float verts[] = {
        // first triangle
         0.5f,  0.0f, 0.5f,   0.0f, 1.0f, 0.0f,      1.0f, 1.0f,    // top right
         0.5f,  0.0f, -0.5f,  0.0f, 1.0f, 0.0f,      1.0f, 0.0f,    // bottom right
        -0.5f,  0.0f, 0.5f,   0.0f, 1.0f, 0.0f,      0.0f, 1.0f,    // top left 
        // second triangle
         0.5f, 0.0f, -0.5f,  0.0f, 1.0f, 0.0f,     1.0f, 0.0f,   // bottom right
        -0.5f, 0.0f, -0.5f,  0.0f, 1.0f, 0.0f,     0.0f, 0.0f,   // bottom left
        -0.5f, 0.0f, 0.5f,   0.0f, 1.0f, 0.0f,     0.0f, 1.0f    // top left
    };

    //float verts[] = {
    // 1.0f, 0.0f, 1.0f,     1.0f, 1.0f,    // top right
    // 1.0f, 0.0f, -1.0f,    1.0f, 0.0f,   // bottom right
    //-1.0f, 0.0f, -1.0f,    0.0f, 0.0f,   // bottom left
    //-1.0f, 0.0f, 1.0f,     0.0f, 1.0f    // top left 
    //};
    //
    //GLushort indices[] = {  // note that we start from 0!
    //    0, 1, 3,   // first triangle
    //    1, 2, 3    // second triangle
    //};
    //mesh.nIndices = 6;

    glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao);

    // Create 2 buffers: first one for the vertex data; second one for the indices
    glGenBuffers(2, mesh.vbos);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

    // Now that VAOs and VBOs are created and have data stored,
    // we can tell the GPU how to read the data
    setupMesh();
}

void PyramidMesh::CreateMesh(GLMesh& mesh)
{
   
    GLfloat verts[] = {
        // position(x, y, z)    // colors                 // texture(s, t) coordinates
        -1.0f, 0.0f, -1.0f,		0.0f, -1.0f, 0.0f,    0.0f, 0.0f,	// Base Triangle 1
        -1.0f, 0.0f,  1.0f,		0.0f, -1.0f, 0.0f,    0.0f, 1.0f,
         1.0f, 0.0f,  1.0f,		0.0f, -1.0f, 0.0f,    1.0f, 1.0f,

         1.0f, 0.0f,  1.0f,		0.0f, -1.0f, 0.0f,    1.0f, 1.0f,    // Base Triangle 2
         1.0f, 0.0f, -1.0f,		0.0f, -1.0f, 0.0f,    1.0f, 0.0f,
        -1.0f, 0.0f, -1.0f,		0.0f, -1.0f, 0.0f,    0.0f, 0.0f,

        -1.0f, 0.0f, -1.0f,		-1.0f, 1.0f, 0.0f,    0.0f, 0.0f,    // Side 1
        -1.0f, 0.0f,  1.0f,		-1.0f, 1.0f, 0.0f,    1.0f, 0.0f,
         0.0f, 1.0f,  0.0f,		-1.0f, 1.0f, 0.0f,    0.5f, 1.0f,

        -1.0f, 0.0f, -1.0f,		0.0f, 1.0f, -1.0f,    0.0f, 0.0f,    // Side 2
         1.0f, 0.0f, -1.0f,		0.0f, 1.0f, -1.0f,    1.0f, 0.0f,
         0.0f, 1.0f,  0.0f,		0.0f, 1.0f, -1.0f,    0.5f, 1.0f,

         1.0f, 0.0f,  1.0f,		1.0f, 1.0f, 0.0f,     0.0f, 0.0f,    // Side 3
         1.0f, 0.0f, -1.0f,     1.0f, 1.0f, 0.0f,	   1.0f, 0.0f,
         0.0f, 1.0f,  0.0f,		1.0f, 1.0f, 0.0f,     0.5f, 1.0f,

        -1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,     0.0f, 0.0f,     // Side 4
         1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,     1.0f, 0.0f,
         0.0f, 1.0f, 0.0f,		0.0f, 1.0f, 1.0f,     0.5f, 1.0f,
    };
    // Vertex Data for drawing using indexing
    //GLfloat verts[] =
    //{
    //    // Vertex Positions    // Colors (r,g,b,a)
    //       0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 0.0f, 1.0f,  // 0 Front-right blue
    //      -0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 0.0f, 1.0f,  // 1 Front-left yellow
    //      0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f, 1.0f,   // 2 Back-right cyan
    //      -0.5f, -0.5f,-0.5f,   0.0f, 0.0f, 0.0f, 1.0f,   // 3 Back- left purple

    //      0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f   // 4 Vertex Top white
    //};
    //// Creates a buffer object for the indices
    //GLushort indices[] = {
    //    0, 2, 1, // Triangle 1
    //    1, 2, 3, // Triangle 2
    //    0, 1, 4, // Triangle 3
    //    0, 2, 4, // Triangle 4
    //    1, 3, 4, // Triangle 5
    //    2, 3, 4  // Triangle 6
    //};


    //mesh.nIndices = 18;

    glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao);

    // Create 1 buffers: first one for the vertex data
    glGenBuffers(1, mesh.vbos);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/
    // Now that VAOs and VBOs are created and have data stored,
    // we can tell the GPU how to read the data
    setupMesh();

    
}

// Implements the UCreateMesh function
void CubeMesh::CreateMesh(GLMesh& mesh)
{

    float verts[] = {
        //Positions                               //Texture Coordinates
     -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,     0.0f, 0.0f, 
      0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,     1.0f, 0.0f,
      0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,     1.0f, 1.0f,
      0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,     1.0f, 1.0f,
     -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,     0.0f, 1.0f,
     -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,     0.0f, 0.0f,

     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,      0.0f, 0.0f,
      0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,      1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  1.0f,      1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  1.0f,      1.0f, 1.0f,
     -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  1.0f,      0.0f, 1.0f,
     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,      0.0f, 0.0f,

     -0.5f,  0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,      1.0f, 0.0f,
     -0.5f,  0.5f, -0.5f,  -1.0f, 0.0f,  0.0f,      1.0f, 1.0f,
     -0.5f, -0.5f, -0.5f,  -1.0f, 0.0f,  0.0f,      0.0f, 1.0f,
     -0.5f, -0.5f, -0.5f,  -1.0f, 0.0f,  0.0f,      0.0f, 1.0f,
     -0.5f, -0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,      0.0f, 0.0f,
     -0.5f,  0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,      1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,   1.0f, 0.0f,  0.0f,       1.0f, 0.0f,
      0.5f,  0.5f, -0.5f,   1.0f, 0.0f,  0.0f,       1.0f, 1.0f,
      0.5f, -0.5f, -0.5f,   1.0f, 0.0f,  0.0f,       0.0f, 1.0f,
      0.5f, -0.5f, -0.5f,   1.0f, 0.0f,  0.0f,       0.0f, 1.0f,
      0.5f, -0.5f,  0.5f,   1.0f, 0.0f,  0.0f,       0.0f, 0.0f,
      0.5f,  0.5f,  0.5f,   1.0f, 0.0f,  0.0f,       1.0f, 0.0f,

     -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,      0.0f, 1.0f,
      0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,      1.0f, 1.0f,
      0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,      1.0f, 0.0f,
      0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,      1.0f, 0.0f,
     -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,      0.0f, 0.0f,
     -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,      0.0f, 1.0f,
                                         
     -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,  0.0f,      0.0f, 1.0f,
      0.5f,  0.5f, -0.5f,   0.0f, 1.0f,  0.0f,      1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,   0.0f, 1.0f,  0.0f,      1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,   0.0f, 1.0f,  0.0f,      1.0f, 0.0f,
     -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,  0.0f,      0.0f, 0.0f,
     -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,  0.0f,      0.0f, 1.0f
    };


    // Vertex Data for drawing using indexing

   //// Position and Color data
   // GLfloat verts[] = {
   //     // Vertex Positions    // Colors (r,g,b,a)
   //      0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f, // Top-Right Vertex 0
   //      0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f, // Bottom-Right Vertex 1
   //     -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f, // Bottom-Left Vertex 2
   //     -0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 1.0f, 1.0f, // Top-Left Vertex 3

   //      0.5f, -0.5f, -1.0f,  0.5f, 0.5f, 1.0f, 1.0f, // 4 br  right
   //      0.5f,  0.5f, -1.0f,  1.0f, 1.0f, 0.5f, 1.0f, //  5 tl  right
   //     -0.5f,  0.5f, -1.0f,  0.2f, 0.2f, 0.5f, 1.0f, //  6 tl  top
   //     -0.5f, -0.5f, -1.0f,  1.0f, 0.0f, 1.0f, 1.0f  //  7 bl back
   // };
   // // Index data to share position data
   // GLushort indices[] = {
   //     0, 1, 3, // Triangle 1
   //     1, 2, 3, // Triangle 2
   //     0, 1, 4, // Triangle 3
   //     0, 4, 5, // Triangle 4
   //     0, 5, 6, // Triangle 5
   //     0, 3, 6, // Triangle 6
   //     4, 5, 6, // Triangle 7
   //     4, 6, 7, // Triangle 8
   //     2, 3, 6, // Triangle 9
   //     2, 6, 7, // Triangle 10
   //     1, 4, 7, // Triangle 11
   //     1, 2, 7  // Triangle 12
   // };

   // mesh.nIndices = 36;

    glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao);

    // Create 1 buffers: first one for the vertex data
    glGenBuffers(1, mesh.vbos);
    
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Now that VAOs and VBOs are created and have data stored,
    // we can tell the GPU how to read the data
    setupMesh();
}

void IcosphereMesh::CreateMesh(GLMesh& mesh) {

    Icosphere icoSphere(0.5f, 1, true);

    const int n = 504;
    float vertsArray[n];
    mesh.nIndices = 240;



    for (int i = 0; i < n; i++) {
        vertsArray[i] = icoSphere.interleavedVertices.at(i);

    }
    for (int i = 0; i < mesh.nIndices; i++) {
        mesh.icoIndices[i] = icoSphere.indices.at(i);
    }


    /*std::cout << icoSphere.interleavedVertices.size() << " " << icoSphere.indices.size();*/

    glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao);

    // Create 2 buffers: first one for the vertex data; second one for the indices
    glGenBuffers(2, mesh.vbos);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertsArray), vertsArray, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mesh.icoIndices), mesh.icoIndices, GL_STATIC_DRAW);

    // Now that VAOs and VBOs are created and have data stored,
    // we can tell the GPU how to read the data
    // Creates the Vertex Attribute Pointer for the screen coordinates
    const GLuint floatsPerVertex = 3; // Number of coordinates per vertex
    //const GLuint floatsPerColor = 4;  // (r, g, b, a)
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;
    // Strides between vertex coordinates is 6 (x, y, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerUV + floatsPerNormal);// The number of floats before each

    // Creates the Vertex Attribute Pointer
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);
}

void CylinderMesh::CreateMesh(GLMesh& mesh) {
    
    Cylinder cylinder(0.4f, 0.4f, 1.0f,
        8, 4, true);

   
    const int n = 504;
    float vertsArray[n];
    mesh.nIndices = 240;


    
    // build verts array for cylinder
    for (int i = 0; i < n; i++) {
        vertsArray[i] = cylinder.interleavedVertices.at(i);
        
    }
    for (int i = 0; i < mesh.nIndices; i++) {
        mesh.cylIndices[i] = cylinder.indices.at(i);
    }

    //std::cout << cylinder.interleavedVertices.size() << " " << cylinder.indices.size();

    glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao);

    // Create 2 buffers: first one for the vertex data; second one for the indices
    
    glGenBuffers(2, mesh.vbos);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertsArray), vertsArray, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mesh.cylIndices), mesh.cylIndices, GL_STATIC_DRAW);

    // Now that VAOs and VBOs are created and have data stored,
    // we can tell the GPU how to read the data
    // Creates the Vertex Attribute Pointer for the screen coordinates
    const GLuint floatsPerVertex = 3; // Number of coordinates per vertex
    //const GLuint floatsPerColor = 4;  // (r, g, b, a)
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;
    // Strides between vertex coordinates is 6 (x, y, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerUV + floatsPerNormal);// The number of floats before each

    // Creates the Vertex Attribute Pointer
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);
}

void PrismMesh::CreateMesh(GLMesh& mesh) {

    Cylinder prism(0.4f, 0.4f, 1.0f, 3, 8, true);

    const int n = 352;
    float vertsArray[n];
    mesh.nIndices = 162;


    // build verts array for prism
    for (int i = 0; i < n; i++) {
        vertsArray[i] = prism.interleavedVertices.at(i);
    }
    for (int i = 0; i < mesh.nIndices; i++) {
        mesh.prismIndices[i] = prism.indices.at(i);
    }
    //std::cout << prism.interleavedVertices.size() << " " << prism.indices.size();

    glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao);

    // Create 2 buffers: first one for the vertex data; second one for the indices

    glGenBuffers(2, mesh.vbos);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertsArray), vertsArray, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mesh.prismIndices), mesh.prismIndices, GL_STATIC_DRAW);

    // Now that VAOs and VBOs are created and have data stored,
    // we can tell the GPU how to read the data
    // Creates the Vertex Attribute Pointer for the screen coordinates
    const GLuint floatsPerVertex = 3; // Number of coordinates per vertex
    //const GLuint floatsPerColor = 4;  // (r, g, b, a)
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;
    // Strides between vertex coordinates is 6 (x, y, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerUV + floatsPerNormal);// The number of floats before each

    // Creates the Vertex Attribute Pointer
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);
}

void Mesh::setupMesh() {
    // Creates the Vertex Attribute Pointer for the screen coordinates
    const GLuint floatsPerVertex = 3; // Number of coordinates per vertex
    //const GLuint floatsPerColor = 4;  // (r, g, b, a)
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;
    // Strides between vertex coordinates is 6 (x, y, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerUV + floatsPerNormal );// The number of floats before each

    // Creates the Vertex Attribute Pointer
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(6, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(6);
}

void Mesh::DestroyMesh(GLMesh& mesh)
{
    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteBuffers(2, mesh.vbos);
}


// Images are loaded with Y axis going down, but OpenGL's Y axis goes up, so let's flip it
//void Mesh::flipImageVertically(unsigned char* image, int width, int height, int channels)
//{
//    for (int j = 0; j < height / 2; ++j)
//    {
//        int index1 = j * width * channels;
//        int index2 = (height - 1 - j) * width * channels;
//
//        for (int i = width * channels; i > 0; --i)
//        {
//            unsigned char tmp = image[index1];
//            image[index1] = image[index2];
//            image[index2] = tmp;
//            ++index1;
//            ++index2;
//        }
//    }
//}
