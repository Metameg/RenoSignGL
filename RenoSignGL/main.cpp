//#include <windows.h>
#include <glad/glad.h>
#include <iostream>         // cout, cerr
#include <cstdlib>          // EXIT_FAILURE
//#include <GL/glew.h>        // GLEW library
#include <vector>
#include <GLFW/glfw3.h>     // GLFW library
#ifdef __APPLE__
#include <GLUT/glut.h>
#else

#endif
#include <gl/gl.h>		// Header File For The OpenGL32 Library
	// Header File For The GLu32 Library
// GLM Math Header inclusions
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Cylinder.h"
#include "GLMesh.h"
#include "shaderGL.h"
//#include "shapes.h"
#include "Camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"      // Image loading Utility functions
/*Shader program Macro*/
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

using namespace std; // Uses the standard namespace

// Unnamed namespace
namespace
{
    
    const char* const WINDOW_TITLE = "Reno Sign Model GL"; // Macro for window title
   
    // Variables for window width and height
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;
    
    
    GLFWwindow* window = nullptr; // Main GLFW window
    GLMesh pyramidMesh;
    GLMesh cubeMesh;  
    GLMesh planeMesh;  
    GLMesh cylinderMesh;  
    GLMesh icoSphereMesh;  
    GLMesh prismMesh;  

    // Camera Initialization
    Camera gCamera(glm::vec3(0.0f, 1.5f, 20.0f));
    float gLastX = WINDOW_WIDTH / 2.0f;
    float gLastY = WINDOW_HEIGHT / 2.0f;
    bool gFirstMouse = true;
    bool toggleOrtho = false;
    

    // Timing Variables
    float gDeltaTime = 0.0f; // Time between current frame and last frame
    float gLastFrame = 0.0f;


    // Object Translations, Rotations, and Scaling vectors
    
    glm::mat4 bottomBaseCubeModels[2];
    glm::mat4 topBaseCubeModels[2];
    glm::mat4 innerPoleCubeModels[2];
    glm::mat4 outerPoleCubeModels[2];
    glm::mat4 letterCubeModels;
    glm::mat4 starStandModel;

    const int numArchCubes = 6;
    glm::mat4 bottomArchCubeModels[numArchCubes];
    glm::mat4 midArchCubeModels[numArchCubes];
    glm::mat4 topArchCubeModels[numArchCubes];

    const int numDecoraterCubes = 3;
    glm::mat4 leftDecoraterCubeModels[numDecoraterCubes];
    glm::mat4 rightDecoraterCubeModels[numDecoraterCubes];

    const int numPrisms = 2;
    glm::mat4 prismModels[numPrisms];

    const int numPyramids = 60;
    glm::mat4 pyramidModels[numPyramids];

    const int numLeftPoleCylinders = 8;
    glm::mat4 leftPoleCylinderModels[numLeftPoleCylinders];
    const int numRightPoleCylinders = 7;
    glm::mat4 rightPoleCylinderModels[numRightPoleCylinders];
    
    const int numIcoSpheres = 4;
    glm::mat4 icoSphereModels[numIcoSpheres];

    glm::mat4 planeModel;
    glm::mat4 lightModel;

    unsigned int silverDiffuseMap;
    unsigned int silverSpecularMap;
    unsigned int baseDiffuseMap;
    unsigned int roadDiffuseMap;
    unsigned int poleDiffuseMap;
    unsigned int poleSpecularMap;
    unsigned int letterbackDiffuseMap;
    unsigned int letterbackSpecularMap;
    unsigned int redDiffuseMap;
    unsigned int redSpecularMap;

}


/* User-defined Function prototypes to:
 * initialize the program, set the window size,
 * redraw graphics on the window when resized,
 * and render graphics on the screen
 */
bool UInitialize(int, char* [], GLFWwindow** window);
void UResizeWindow(GLFWwindow* window, int width, int height);
void UProcessInput(GLFWwindow* window);
void URender(Shader &lightingShader, Shader &basicShader, Shader &elementsShader);
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos);
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void drawInstances(GLMesh& mesh, int numInstances, int numVertices, GLenum fillMode);
glm::mat4 createModelMatrix(glm::vec3& initPos, glm::vec3& scaleVec, glm::vec3& rotAxis, float rotDeg);
unsigned int UCreateTexture(char const* path);

// Shape init functions
void initCubes();
void initPyramids();
void initPlanes();
void initCylinders();
void initIcospheres();
void initPrisms();

// main function. Entry point to the OpenGL program
int main(int argc, char* argv[])
{
    if (!UInitialize(argc, argv, &window))
        return EXIT_FAILURE;

    // Create the meshes
    PyramidMesh::CreateMesh(pyramidMesh);
    CubeMesh::CreateMesh(cubeMesh);
    PlaneMesh::CreateMesh(planeMesh);
    CylinderMesh::CreateMesh(cylinderMesh);
    PrismMesh::CreateMesh(prismMesh);
    IcosphereMesh::CreateMesh(icoSphereMesh);
    

    Shader lightingShader("shaderfiles/6.multiple_lights.vs", "shaderfiles/6.multiple_lights.fs");
    Shader basicShader("shaderfiles/6.light_cube.vs", "shaderfiles/6.light_cube.fs");
    Shader elementsShader("shaderfiles/Element.multiple_lights.vs", "shaderfiles/Element.multiple_lights.fs");

    silverDiffuseMap =      UCreateTexture("images/dark_silver.jpg");
    silverSpecularMap =     UCreateTexture("images/silver.jpg");
    baseDiffuseMap =        UCreateTexture("images/base_cubes.jpg");
    roadDiffuseMap =        UCreateTexture("images/road.jpg");
    poleDiffuseMap =        UCreateTexture("images/dark_gold.jpg");
    poleSpecularMap =       UCreateTexture("images/gold_specular.jpg");
    letterbackDiffuseMap =  UCreateTexture("images/letterBackground.jpg");
    letterbackSpecularMap = UCreateTexture("images/letterBackground.jpg");
    redDiffuseMap =         UCreateTexture("images/dark_red.jpg");

    
    

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----d
        UProcessInput(window);

        // Render
        // -------
        URender(lightingShader, basicShader, elementsShader);
             // Create Texture Coords ( NEW )
          

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);    // Flips the the back buffer with the front buffer every frame.
        glfwPollEvents();
    }

    exit(EXIT_SUCCESS); // Terminates the program successfully
}


// Initialize GLFW, GLEW, and create a window
bool UInitialize(int argc, char* argv[], GLFWwindow** window)
{
    // GLFW: initialize and configure (specify desired OpenGL version)
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GLFW: window creation
    // ---------------------
    * window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (*window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, UResizeWindow);
    glfwSetCursorPosCallback(*window, UMousePositionCallback);
    glfwSetScrollCallback(*window, UMouseScrollCallback);
    glfwSetMouseButtonCallback(*window, UMouseButtonCallback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // initialize all shape models
    initCubes();
    initPyramids();
    initPlanes();
    initCylinders();
    initIcospheres();
    initPyramids();


    
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void UProcessInput(GLFWwindow* window)
{
    

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Keyboard Movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        gCamera.ProcessKeyboard(FORWARD, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        gCamera.ProcessKeyboard(BACKWARD, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        gCamera.ProcessKeyboard(LEFT, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        gCamera.ProcessKeyboard(RIGHT, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        gCamera.ProcessKeyboard(UP, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        gCamera.ProcessKeyboard(DOWN, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) 
        toggleOrtho = !toggleOrtho;

}

void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos) {
    if (gFirstMouse)
    {
        gLastX = xpos;
        gLastY = ypos;
        gFirstMouse = false;
    }

    float xoffset = xpos - gLastX;
    float yoffset = gLastY - ypos; // reversed since y-coordinates go from bottom to top

    gLastX = xpos;
    gLastY = ypos;

    gCamera.ProcessMouseMovement(xoffset, yoffset);
}

void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    gCamera.ProcessMouseScroll(yoffset);
}

void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void UResizeWindow(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void URender(Shader &lightingShader, Shader &basicShader, Shader &elementsShader)
{
    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);
    
    lightingShader.setFloat("material.shininess", 22.0f);

    glm::vec3 lightPos = glm::vec3(3.0f, 16.5f, 5.0f);
    glm::vec3 spotlightPos = glm::vec3(17.0f, 16.0f, 5.0f);

    // directional light
    lightingShader.setVec3("dirLight.direction", 0.6f, -1.0f, 0.5f);
    lightingShader.setVec3("dirLight.ambient", 0.75f, 0.75f, 0.75f);
    lightingShader.setVec3("dirLight.diffuse", 1.0f, 1.0f, 1.0f);
    lightingShader.setVec3("dirLight.specular", 1.8, 1.8f, 1.5f);
    // point light 
    lightingShader.setVec3("pointLights[0].position", lightPos);
    lightingShader.setVec3("pointLights[0].ambient", 0.8f, 0.8f, 0.3f);
    lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    lightingShader.setVec3("pointLights[0].specular", 3.0f, 3.0f, 3.0f);
    lightingShader.setFloat("pointLights[0].constant", 1.0f);
    lightingShader.setFloat("pointLights[0].linear", 0.09);
    lightingShader.setFloat("pointLights[0].quadratic", 0.032);
    // spotlight
    lightingShader.setVec3("spotLight.position", spotlightPos);
    lightingShader.setVec3("spotLight.direction", -0.5f, -0.3f, -1.0f);
    lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    lightingShader.setVec3("spotLight.diffuse", 0.0f, 0.0f, 0.0f);
    lightingShader.setVec3("spotLight.specular", 0.0f, 0.0f, 0.0f);
    lightingShader.setFloat("spotLight.constant", 1.0f);
    lightingShader.setFloat("spotLight.linear", 0.09);
    lightingShader.setFloat("spotLight.quadratic", 0.032);
    lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));


    // Get Time between current frame and last frame
    float currentFrame = glfwGetTime();
    gDeltaTime = currentFrame - gLastFrame;
    gLastFrame = currentFrame;

    // Enable z-depth
    glEnable(GL_DEPTH_TEST);

    // Sets the background color of the window to black (it will be implicitely used by glClear)
    glClearColor(0.0f, 0.6f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Transforms the camera 
    glm::mat4 view = gCamera.GetViewMatrix();

    
    glm::mat4 projection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
    
     //Creates a orthographic projection
    if (toggleOrtho) {
        projection = glm::ortho(gCamera.Position.x-6.0f, gCamera.Position.x+6.0f, gCamera.Position.y-1.5f, gCamera.Position.y+1.5f, 0.1f, 100.0f);
    }

    // Retrieves and passes transform matrices to the Shader program
    lightingShader.setVec3("viewPos", gCamera.Position);
    lightingShader.setMat4("view", view);
    lightingShader.setMat4("projection", projection);

    // Generate buffers and draw instances for PYRAMIDS
    glGenBuffers(1, &pyramidMesh.vbos[2]);
    glBindBuffer(GL_ARRAY_BUFFER, pyramidMesh.vbos[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidModels), &pyramidModels[0], GL_STATIC_DRAW);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, silverDiffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, silverSpecularMap);
    drawInstances(pyramidMesh, numPyramids, 18, GL_FILL);

    
    // Generate buffers and draw instances for PLANES
    lightingShader.setFloat("material.shininess", 1.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, roadDiffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, silverSpecularMap);
    glGenBuffers(1, &planeMesh.vbos[2]);
    glBindBuffer(GL_ARRAY_BUFFER, planeMesh.vbos[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeModel), &planeModel, GL_STATIC_DRAW);

    drawInstances(planeMesh, 1, 6, GL_FILL);
    
    
    // Generate buffers and draw instances for CUBES

    // Bottom bases
    lightingShader.setFloat("material.shininess", 300.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, baseDiffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, silverSpecularMap);
    glGenBuffers(1, &cubeMesh.vbos[2]);
    glBindBuffer(GL_ARRAY_BUFFER, cubeMesh.vbos[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bottomBaseCubeModels), &bottomBaseCubeModels, GL_STATIC_DRAW);

    drawInstances(cubeMesh, 2, 36, GL_FILL);

    // Top bases
    lightingShader.setFloat("material.shininess", 50.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, baseDiffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, silverSpecularMap);
    
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(topBaseCubeModels), &topBaseCubeModels, GL_STATIC_DRAW);

    drawInstances(cubeMesh, 2, 36, GL_FILL);

    // Inner Poles
    lightingShader.setFloat("material.shininess", 15.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, poleDiffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, poleSpecularMap);
    
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(innerPoleCubeModels), &innerPoleCubeModels, GL_STATIC_DRAW);

    drawInstances(cubeMesh, 2, 36, GL_FILL);

    // Outer Poles
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, poleDiffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, poleSpecularMap);
    
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(outerPoleCubeModels), &outerPoleCubeModels, GL_STATIC_DRAW);

    drawInstances(cubeMesh, 2, 36, GL_FILL);

    // Letter Cube
    lightingShader.setFloat("material.shininess", 20.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, letterbackDiffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, letterbackSpecularMap);  
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(letterCubeModels), &letterCubeModels, GL_STATIC_DRAW);
    drawInstances(cubeMesh, 1, 36, GL_FILL);

    // Left Decorater Cubes
    glBufferData(GL_ARRAY_BUFFER, sizeof(leftDecoraterCubeModels), &leftDecoraterCubeModels, GL_STATIC_DRAW);
    drawInstances(cubeMesh, numDecoraterCubes, 36, GL_FILL);
    // Right Decorater Cubes
    glBufferData(GL_ARRAY_BUFFER, sizeof(rightDecoraterCubeModels), &rightDecoraterCubeModels, GL_STATIC_DRAW);
    drawInstances(cubeMesh, numDecoraterCubes, 36, GL_FILL);
    
    //  Main Arch Cubes

    // bottom
    lightingShader.setFloat("material.shininess", 20.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, silverSpecularMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, letterbackSpecularMap);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(bottomArchCubeModels), &bottomArchCubeModels, GL_STATIC_DRAW);
    drawInstances(cubeMesh, numArchCubes, 36, GL_FILL);

    // Top
    glBufferData(GL_ARRAY_BUFFER, sizeof(topArchCubeModels), &topArchCubeModels, GL_STATIC_DRAW);
    drawInstances(cubeMesh, numArchCubes, 36, GL_FILL);

    // Mid
    lightingShader.setFloat("material.shininess", 200.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, redDiffuseMap);
    glBufferData(GL_ARRAY_BUFFER, sizeof(midArchCubeModels), &midArchCubeModels, GL_STATIC_DRAW);
    drawInstances(cubeMesh, numArchCubes, 36, GL_FILL);
    
    // Star Stand
    lightingShader.setFloat("material.shininess", 22.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, silverDiffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, silverSpecularMap);
    glBufferData(GL_ARRAY_BUFFER, sizeof(starStandModel), &starStandModel, GL_STATIC_DRAW);
    drawInstances(cubeMesh, 1, 36, GL_FILL);




    elementsShader.use();
    // directional light
    elementsShader.setVec3("dirLight.direction", 0.3f, -1.0f, 0.5f);
    elementsShader.setVec3("dirLight.ambient", 0.75f, 0.75f, 0.75f);
    elementsShader.setVec3("dirLight.diffuse", 1.4f, 1.4f, 1.4f);
    elementsShader.setVec3("dirLight.specular", 1.8, 1.8f, 1.5f);  // yellowish
    // point light 
    elementsShader.setVec3("pointLights[0].position", lightPos);
    elementsShader.setVec3("pointLights[0].ambient", 0.8f, 0.8f, 0.3f); // yellowish
    elementsShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    elementsShader.setVec3("pointLights[0].specular", 3.0f, 3.0f, 3.0f);   
    elementsShader.setFloat("pointLights[0].constant", 1.0f);
    elementsShader.setFloat("pointLights[0].linear", 0.09);
    elementsShader.setFloat("pointLights[0].quadratic", 0.032);
    // spotlight
    elementsShader.setVec3("spotLight.position", spotlightPos);
    elementsShader.setVec3("spotLight.direction", -0.5f, -0.3f, -1.0f);
    elementsShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    elementsShader.setVec3("spotLight.diffuse", 0.0f, 0.0f, 0.0f);
    elementsShader.setVec3("spotLight.specular", 0.0f, 0.0f, 0.0f);
    elementsShader.setFloat("spotLight.constant", 1.0f);
    elementsShader.setFloat("spotLight.linear", 0.09);
    elementsShader.setFloat("spotLight.quadratic", 0.032);
    elementsShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    elementsShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

    lightingShader.setVec3("viewPos", gCamera.Position);
    elementsShader.setMat4("view", view);
    elementsShader.setMat4("projection", projection);
    elementsShader.setInt("material.diffuse", 0);
    elementsShader.setInt("material.specular", 1);

    // Draw left Cylinders arch
    elementsShader.setFloat("material.shininess", 45.0f);
    for (unsigned int i = 0; i < numLeftPoleCylinders; i++) {
        elementsShader.setMat4("model", leftPoleCylinderModels[i]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, poleDiffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, poleSpecularMap);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cylinderMesh.vbos[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cylinderMesh.cylIndices), cylinderMesh.cylIndices, GL_STATIC_DRAW);

        glBindVertexArray(cylinderMesh.vao);
        glDrawElements(GL_TRIANGLES, cylinderMesh.nIndices, GL_UNSIGNED_INT, 0);
    }
    // Draw right Cylinders arch
    for (unsigned int i = 0; i < numRightPoleCylinders; i++ ) {
        elementsShader.setMat4("model", rightPoleCylinderModels[i]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, poleDiffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, poleSpecularMap);
        
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cylinderMesh.cylIndices), cylinderMesh.cylIndices, GL_STATIC_DRAW);

        glBindVertexArray(cylinderMesh.vao);
        glDrawElements(GL_TRIANGLES, cylinderMesh.nIndices, GL_UNSIGNED_INT, 0);
    }

    // Draw Prisms
    elementsShader.setFloat("material.shininess", 105.0f);
    for (unsigned int i = 0; i < numPrisms; i++) {
        elementsShader.setMat4("model", prismModels[i]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, poleDiffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, poleSpecularMap);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, prismMesh.vbos[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(prismMesh.prismIndices), prismMesh.prismIndices, GL_STATIC_DRAW);

        glBindVertexArray(prismMesh.vao);
        glDrawElements(GL_TRIANGLES, prismMesh.nIndices, GL_UNSIGNED_INT, 0);
    }

    // Spheres
    elementsShader.setFloat("material.shininess", 20.0f);
    for (unsigned int i = 0; i < numIcoSpheres; i++ ) {
        elementsShader.setMat4("model", icoSphereModels[i]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, letterbackDiffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, letterbackSpecularMap);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, icoSphereMesh.vbos[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(icoSphereMesh.icoIndices), icoSphereMesh.icoIndices, GL_STATIC_DRAW);

        glBindVertexArray(icoSphereMesh.vao);
        glDrawElements(GL_TRIANGLES, icoSphereMesh.nIndices, GL_UNSIGNED_INT, 0);
    }

    
    
    // ----------------------Draws point light cube-------------------------
    // 
    glm::vec3 lightScale = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 lightRot = glm::vec3(0.0f, 1.0f, 0.0f);
    lightModel = createModelMatrix(lightPos, lightScale, lightRot, 0.0f);
    glBindVertexArray(cubeMesh.vao);
    // also draw the lamp object(s)
    basicShader.use();
    basicShader.setMat4("projection", projection);
    basicShader.setMat4("view", view);
    basicShader.setMat4("model", lightModel);
    //glDrawArrays(GL_TRIANGLES, 0, 36);
    
}   

void drawInstances(GLMesh& mesh, int numInstances, int numVertices, GLenum fillMode) {
    
    // set instance data
    glBindVertexArray(mesh.vao);

    for (unsigned int i = 0; i < 4; i++) {
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(i + 2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i));
        glEnableVertexAttribArray(i + 2);
        glVertexAttribDivisor(i + 2, 1);
    }

    // Draw instanced pyramids
    //glDrawElementsInstanced(GL_TRIANGLES, mesh.nIndices, GL_UNSIGNED_SHORT, 0, numInstances);
    glDrawArraysInstanced(GL_TRIANGLES, 0, numVertices, numInstances);
    glBindVertexArray(0);
}

glm::mat4 createModelMatrix(glm::vec3& initPos, glm::vec3& scaleVec, glm::vec3& rotAxis, float rotDeg) {
    // 1. Scales the object
    glm::mat4 scale = glm::scale(scaleVec);
    // 2. Rotates shape 
    glm::mat4 rotation = glm::rotate(glm::radians(rotDeg), glm::vec3(rotAxis));
    // 3. Place the object in 3-space
    glm::mat4 translation = glm::translate(initPos);

    glm::mat4 model = translation * rotation * scale;

    return model;
}

unsigned int UCreateTexture(char const* path) {

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);

    }

    return textureID;
}





// Shape Model init functions
//------------------------------------------------

void initCubes() {
    // TRANSLATIONS
    //---------------
    // Support Poles
    glm::vec3 bottomBaseCubePositions[2] = {
        glm::vec3(-11.9f, 0.0f, 0.15f),
        glm::vec3(11.8f, 0.0f, 0.15f)
    };
    glm::vec3 topBaseCubePositions[2] = {
        glm::vec3(-11.9f, 0.15f, 0.15f),
        glm::vec3(11.8f, 0.15f, 0.15f)
    };
    glm::vec3 innerPoleCubePositions[2] = {
        glm::vec3(-11.0f, 0.75f, 0.15f),
        glm::vec3(11.0f, 0.75f,  0.15f)
    };
    glm::vec3 outerPoleCubePositions[2] = {
        glm::vec3(-12.7f, 0.75f, 0.15f),
        glm::vec3(12.7f, 0.75f, 0.15f)
    };

    // Decorater cubes
    // left
    glm::vec3 leftDecoraterCubePositions[3] = {
        glm::vec3(-4.5f, 8.5f, 0.15f),
        glm::vec3(-6.0f, 8.1f, 0.15f),
        glm::vec3(-8.8f, 7.2f, 0.15f)

    };
    // right
    glm::vec3 rightDecoraterCubePositions[3] = {
        glm::vec3(5.0f, 8.5f, 0.15f),
        glm::vec3(6.5f, 7.9f, 0.15f),
        glm::vec3(9.1f, 7.05f, 0.15f)

    };

    // Arch cubes
    glm::vec3 bottomArchCubePositions[numArchCubes] = {
        glm::vec3(-9.3f, 4.7f, 0.15f),
        glm::vec3(-5.6f, 5.85f, 0.15f),
        glm::vec3(-2.1f,  6.4f, 0.15f),
        glm::vec3(9.3f, 4.7f, 0.15f),
        glm::vec3(5.75f, 5.8f, 0.15f),
        glm::vec3(1.8f, 6.4f, 0.15f)
    };
    glm::vec3 midArchCubePositions[numArchCubes] = {
        glm::vec3(-9.3f, 5.7f, 0.15f),
        glm::vec3(-5.6f, 6.85f, 0.15f),
        glm::vec3(-2.1f,  7.4f, 0.15f),
        glm::vec3(9.3f, 5.7f, 0.15f),
        glm::vec3(5.75f, 6.8f, 0.15f),
        glm::vec3(1.8f, 7.4f, 0.15f)
    };
    glm::vec3 topArchCubePositions[numArchCubes] = {
        glm::vec3(-9.3f, 6.7f, 0.15f),
        glm::vec3(-5.6f, 7.85f, 0.15f),
        glm::vec3(-2.1f, 8.4f, 0.15f),
        glm::vec3(9.3f,  6.7f, 0.15f),
        glm::vec3(5.75f, 7.8f, 0.15f),
        glm::vec3(1.8f,  8.4f, 0.15f)
    };

    // RENO letters
    glm::vec3 letterCubePosition = glm::vec3(0.2f, 9.3f, 0.15f);

    // Star Stand
    glm::vec3 starStandPosition = glm::vec3(0.0f, 11.3f, -0.0f);




    // SCALES
    //-------------

    // Support Poles
    glm::vec3 baseCubeScales[2] = {
        glm::vec3(3.0f, 0.6f, 1.5f),      // lower
        glm::vec3(2.7f, 0.6f, 0.9f)       // upper
    };
    glm::vec3 poleCubeScales[2] = {
        glm::vec3(0.3f, 22.0f, 0.55f),    // inner
        glm::vec3(0.3f, 18.0f, 0.55f)     // outer
    };

    // Decorater Cubes
    glm::vec3 decoraterCubeScales[3] = {
        glm::vec3(0.5f, 0.75f, 0.55f),     // little fat
        glm::vec3(3.0f, 0.2f,  0.5f),    // arch1
        glm::vec3(4.3f, 0.2f,  0.5f)    // arch2

    };

    // RENO letters
    glm::vec3 letterCubeScales = glm::vec3(9.1f, 2.0f, 1.0f);
    // Arch Cubes
    glm::vec3 topArchCubeScale = glm::vec3(4.0f, 0.35f, 0.6f);
    glm::vec3 midArchCubeScale = glm::vec3(4.3f, 1.6f, 0.6f);
    // Star Stand
    glm::vec3 starStandScale = glm::vec3(0.1f, 3.0f, 0.1f);



    // ROTATIONS
    //--------------
    glm::vec3 cubeRotAxesY = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cubeRotAxesZ = glm::vec3(0.0f, 0.0f, 1.0f);



    // MODELS
    //--------------

    // Create Arch Cube models
    // Bottom left
    for (unsigned int i = 0; i < numArchCubes / 2; i++) {
        glm::mat4 model = createModelMatrix(
            bottomArchCubePositions[i],
            topArchCubeScale,
            cubeRotAxesZ, (30.0f - 8.5f * (i + 1)));

        bottomArchCubeModels[i] = model;

    }
    // Bottom right
    for (int i = numArchCubes / 2; i < numArchCubes; i++) {
        glm::mat4 model = createModelMatrix(
            bottomArchCubePositions[i],
            topArchCubeScale,
            cubeRotAxesZ, (-30.0f + 8.5f * (i - (numArchCubes / 2) + 1)));

        bottomArchCubeModels[i] = model;

    }

    // Top left
    for (unsigned int i = 0; i < numArchCubes / 2; i++) {
        glm::mat4 model = createModelMatrix(
            topArchCubePositions[i],
            topArchCubeScale,
            cubeRotAxesZ, (30.0f - 8.5f * (i + 1)));

        topArchCubeModels[i] = model;
    }
    // Top right
    for (int i = numArchCubes / 2; i < numArchCubes; i++) {
        glm::mat4 model = createModelMatrix(
            topArchCubePositions[i],
            topArchCubeScale,
            cubeRotAxesZ, (-30.0f + 8.5f * (i - (numArchCubes / 2) + 1)));

        topArchCubeModels[i] = model;
    }

    // Mid left
    for (unsigned int i = 0; i < numArchCubes / 2; i++) {
        glm::mat4 model = createModelMatrix(
            midArchCubePositions[i],
            midArchCubeScale,
            cubeRotAxesZ, (30.0f - 8.5f * (i + 1)));

        midArchCubeModels[i] = model;

    }

    // Mid right
    for (int i = numArchCubes / 2; i < numArchCubes; i++) {
        glm::mat4 model = createModelMatrix(
            midArchCubePositions[i],
            midArchCubeScale,
            cubeRotAxesZ, (-30.0f + 8.5f * (i - (numArchCubes / 2) + 1)));

        midArchCubeModels[i] = model;
    }

    // Decorater Cube Models
    // left
    float leftDecoraterRotations[numDecoraterCubes] = { 0.0f, 15.0f, 20.5f };
    for (unsigned int i = 0; i < numDecoraterCubes; i++) {
        glm::mat4 model = createModelMatrix(
            leftDecoraterCubePositions[i],
            decoraterCubeScales[i],
            cubeRotAxesZ, leftDecoraterRotations[i]);

        leftDecoraterCubeModels[i] = model;

    }

    // right
    float rightDecoraterRotations[numDecoraterCubes] = { 0.0f, -13.0f, -21.0f };
    for (unsigned int i = 0; i < numDecoraterCubes; i++) {
        glm::mat4 model = createModelMatrix(
            rightDecoraterCubePositions[i],
            decoraterCubeScales[i],
            cubeRotAxesZ, rightDecoraterRotations[i]);

        rightDecoraterCubeModels[i] = model;

    }
    // Create bottom base cube models
    for (unsigned int i = 0; i < 2; i++) {
        glm::mat4 model = createModelMatrix(
            bottomBaseCubePositions[i],
            baseCubeScales[0],
            cubeRotAxesY, 0.0f);

        bottomBaseCubeModels[i] = model;
    }

    // Create bottom base cube models
    for (unsigned int i = 0; i < 2; i++) {
        glm::mat4 model = createModelMatrix(
            topBaseCubePositions[i],
            baseCubeScales[1],
            cubeRotAxesY, 0.0f);

        topBaseCubeModels[i] = model;
    }

    // Create inner-pole cube models
    for (unsigned int i = 0; i < 2; i++) {
        glm::mat4 model = createModelMatrix(
            innerPoleCubePositions[i],
            poleCubeScales[0],
            cubeRotAxesY, 0.0f);

        innerPoleCubeModels[i] = model;
    }

    // Create outer-pole cube models
    for (unsigned int i = 0; i < 2; i++) {
        glm::mat4 model = createModelMatrix(
            outerPoleCubePositions[i],
            poleCubeScales[1],
            cubeRotAxesY, 0.0f);

        outerPoleCubeModels[i] = model;
    }

    // Create letter cube model
    letterCubeModels = createModelMatrix(
        letterCubePosition,
        letterCubeScales,
        cubeRotAxesY, 0.0f);

    // Create star stand cube model
    starStandModel = createModelMatrix(
        starStandPosition,
        starStandScale,
        cubeRotAxesY, 0.0f);
}

void initPyramids() {
    // TRANSLATIONS
    //---------------
    glm::vec3 starPyramidTrans = glm::vec3(0.0f, 13.0f, 0.0f);      // Translation vector
    glm::vec3 axes[4];
    // SCALES
    //---------------
    glm::vec3 starPyramidScale = glm::vec3(0.05f, 1.5f, 0.05f);    // Scale Vector
    // ROTATIONS
    //-------------
    axes[0] = glm::vec3(0.0f, 0.0f, 1.0f);                  // Rotation Axis X
    axes[1] = glm::vec3(0.0f, 1.0f, 0.0f);                  // Rotation Axis Y
    axes[2] = glm::vec3(1.0f, 0.0f, 0.0f);                  // Rotation Axis Z
    axes[3] = glm::vec3(1.0f, 0.0f, 1.0f);                  // Rotation Axis XZ

    int index = 0;
    // Create Models
    for (unsigned int i = 0; i < 4; i++) {
        for (float deg = 0.0f; deg < 360.0f; deg += 30.0f) {

            glm::mat4 model = createModelMatrix(
                starPyramidTrans,
                starPyramidScale,
                axes[i], deg);

            pyramidModels[index++] = model;

        }
    }
}

void initPlanes() {
    glm::vec3 groundPlaneTrans = glm::vec3(0.0f, 0.0f, 0.0f);       // Translation Vector
    glm::vec3 groundPlaneScale = glm::vec3(30.0f, 0.0f, 80.0f);     // Scale Vector
    glm::vec3 groundPlaneRot = glm::vec3(1.0, 1.0f, 1.0f);         // Rotation
    // Create Model
    planeModel = createModelMatrix(
        groundPlaneTrans,
        groundPlaneScale,
        groundPlaneRot, 0.0f);
}

void initCylinders() {
    // TRANSLATIONS
    //----------------
    glm::vec3 leftPoleCylinderPositions[numLeftPoleCylinders] = {
        glm::vec3(-12.7f, 20.25f, 0.15f),
        glm::vec3(-12.7f, 10.0f, 0.15f),
        glm::vec3(-14.9f, 6.7f, 0.15f),
        glm::vec3(-16.2f, 3.1f, 0.15f),
        glm::vec3(-16.5f, -0.8f, 0.15f),
        glm::vec3(-15.8f, -4.7f, 0.15f),
        glm::vec3(-14.1f, -8.2f, 0.15f),
        glm::vec3(-11.55f, -11.1f, 0.15f)
    };
    glm::vec3 rightPoleCylinderPositions[numRightPoleCylinders] = {
        glm::vec3(12.7f, 10.0f, 0.15f),
        glm::vec3(14.9f, 6.7f, 0.15f),
        glm::vec3(16.2f, 3.1f, 0.15f),
        glm::vec3(16.5f, -0.8f, 0.15f),
        glm::vec3(15.8f, -4.7f, 0.15f),
        glm::vec3(14.1f, -8.2f, 0.15f),
        glm::vec3(11.55f, -11.1f, 0.15f)
    };


    // SCALES
    //----------------
    glm::vec3 poleCylinderScale = glm::vec3(0.50f, 0.9f, 0.5f);
    // ROTATIONS
    //----------------
    glm::vec3 poleCylinderRotX = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 poleCylinderRotZ = glm::vec3(0.0f, 0.0f, 1.0f);

    // Create left pole cylinder models
    for (unsigned int i = 1; i < numLeftPoleCylinders; i++) {

        glm::mat4 model = createModelMatrix(
            leftPoleCylinderPositions[i],
            poleCylinderScale,
            poleCylinderRotX, 90.0f);

        glm::mat4 Zrotation = glm::rotate(glm::radians(-15.0f * (i - 1)), glm::vec3(poleCylinderRotZ));
        model = Zrotation * model;


        leftPoleCylinderModels[i] = model;
    }

    // Create right pole cylinder models
    for (unsigned int i = 0; i < numRightPoleCylinders; i++) {

        glm::mat4 model = createModelMatrix(
            rightPoleCylinderPositions[i],
            poleCylinderScale,
            poleCylinderRotX, 90.0f);

        glm::mat4 Zrotation = glm::rotate(glm::radians(15.0f * (i)), glm::vec3(poleCylinderRotZ));
        model = Zrotation * model;

        rightPoleCylinderModels[i] = model;
    }
}

void initIcospheres() {
    glm::vec3 icoSpherePositions[4] = {
        glm::vec3(-3.5f, 10.3f, 0.1f),
        glm::vec3(-1.0f, 10.3f, 0.1f),
        glm::vec3(1.5f, 10.3f, 0.1f),
        glm::vec3(4.0f, 10.3f, 0.1f)
    };

    glm::vec3 icoSphereScale = glm::vec3(1.65f, 1.6f, 0.95f);
    glm::vec3 icoSphereRot = glm::vec3(0.0f, 1.0f, 0.0f);

    // Create pole sphere models
    for (unsigned int i = 0; i < numIcoSpheres; i++) {
        glm::mat4 model = createModelMatrix(
            icoSpherePositions[i],
            icoSphereScale,
            icoSphereRot, 0.0f);

        icoSphereModels[i] = model;
    }
}

void initPrisms() {
    // TRANSLATIONS
    //-----------------
    glm::vec3 prismPositions[2] = {
        glm::vec3(-11.7f, 2.75f, 0.15f),
        glm::vec3(11.7f, 2.75f, 0.15f)
    };
    // SCALES
    //-----------------
    glm::vec3 prismScale = glm::vec3(1.0f, 21.0f, 0.3f);
    // ROTATIONS
    //-----------------
    glm::vec3 prismRot = glm::vec3(0.0f, 1.0f, 0.0f);

    // Create prism models
    for (unsigned int i = 0; i < numPrisms; i++) {
        glm::mat4 model = createModelMatrix(
            prismPositions[i],
            prismScale,
            prismRot, 15.0f + 180.0f * (i + 1));

        prismModels[i] = model;
    }
}