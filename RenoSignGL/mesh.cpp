#include "mesh.h"
#include <iostream>

// Implements the UCreateMesh function
void Mesh::CreateMesh(GLMesh& mesh)
{
    // Specifies Normalized Device Coordinates (x,y,z) and color (r,g,b,a) for triangle vertices
    GLfloat verts[] =
    {
        // Vertex Positions    // Colors (r,g,b,a)
           0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f, 1.0f,  // 0 Front-right blue
          -0.5f, -0.5f, 0.5f,   1.0f, 1.0f, 0.0f, 1.0f,  // 1 Front-left yellow
          0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f, 1.0f,   // 2 Back-right cyan
          -0.5f, -0.5f,-0.5f,   1.0f, 0.0f, 1.0f, 1.0f,   // 3 Back- left purple

          0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f, 1.0f   // 4 Vertex Top white
    };

    glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao);

    // Create 2 buffers: first one for the vertex data; second one for the indices
    glGenBuffers(2, mesh.vbos);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Creates a buffer object for the indices
    GLushort indices[] = {
        0, 2, 1, // Triangle 1
        1, 2, 3, // Triangle 2
        0, 1, 4, // Triangle 3
        0, 2, 4, // Triangle 4
        1, 3, 4, // Triangle 5
        2, 3, 4  // Triangle 6
    };
    mesh.nIndices = sizeof(indices) / sizeof(indices[0]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Creates the Vertex Attribute Pointer for the screen coordinates
    const GLuint floatsPerVertex = 3; // Number of coordinates per vertex
    const GLuint floatsPerColor = 4;  // (r, g, b, a)

    // Strides between vertex coordinates is 6 (x, y, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerColor);// The number of floats before each

    // Creates the Vertex Attribute Pointer
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerColor, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);
}

void Mesh::DestroyMesh(GLMesh& mesh)
{
    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteBuffers(2, mesh.vbos);
}
