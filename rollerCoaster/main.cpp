
// COMP371-Assignment-2    NAME: BOCHUAN AN    ID:  27878745
#include <iostream>
#define GLEW_STATIC  // define GLEW_STATIC since we're using the static version of the GLEW library.
#include <GL/glew.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <GLFW/glfw3.h>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "SOIL2/SOIL2.h"
//#include "unistd.h"

using namespace std;
enum RenderMode { POINTS, LINES };
// Camera
glm::vec3 cameraPos;
glm::vec3 cameraFront;
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);
RenderMode renderMode;
bool reset = false;
bool turnOn = true;
bool part2 = false;
int c;
int counter = 1;
static int pos = 0;
bool stop = false;
// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame
Shader* shader;

glm::mat4 projection_matrix;

// 3D environment
float vertices[] = {
    // positions          // Normal coords    Texture coords
    0.5f,  0.0f, -0.5f,   0.0f, 1.0f, 0.0f,   // 1.0f, 1.0f,              // top right
    0.5f, 0.0f, 0.5f,     0.0f, 1.0f, 0.0f,   // 1.0f, 0.0f,               // bottom right
    -0.5f, 0.0f, 0.5f,    0.0f, 1.0f, 0.0f,   // 0.0f, 0.0f,              // bottom left
    -0.5f,  0.0f, -0.5f,  0.0f, 1.0f, 0.0f,    // 0.0f, 1.0f               // top left
};
unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

GLfloat rails[] = {
    -1.0f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    1.0f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    1.0f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    1.0f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -1.0f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -1.0f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    
    -1.0f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    1.0f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    1.0f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    1.0f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -1.0f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -1.0f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    
    -1.0f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -1.0f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -1.0f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -1.0f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -1.0f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -1.0f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    
    1.0f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    1.0f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    1.0f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    1.0f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    1.0f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    1.0f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    
    -1.0f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    1.0f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    1.0f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    1.0f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -1.0f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -1.0f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    
    -1.0f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    1.0f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    1.0f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    1.0f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -1.0f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -1.0f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 800;

GLfloat yaw   = -90.0f;
GLfloat pitch =   0.0f;
GLfloat lastX =  WIDTH  / 2.0;
GLfloat lastY =  HEIGHT / 2.0;

vector<glm::vec3> user_clicks; // store the user's clicks points
vector<glm::vec3> Tangents;  // store the cameraFront coords
vector<glm::vec3> line;

GLuint VAO, VBO, VAO2, VBO2, VAO3, VBO3, EBO;


// Window clicks
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    
    //marks control points in the window
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if(counter <= c)
        {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            glm::vec3 new_position = glm::vec3((-1.0f + 2 *(float)xpos/width), (1.0f - 2*(float)ypos/height), 0.0f);
            user_clicks.push_back(new_position);
            counter++;
        }
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, user_clicks.size() * sizeof(glm::vec3), &user_clicks[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind for good measure
    }
}

// pass 4 points p0 p1 p2 p3 and u value  return the interpolate point coordinates x, y ,z

glm::vec3 catmullRom(float u, glm::mat4x3 control, float s = 0.5) {
    
    glm::mat4 basis = glm::mat4(
                                -s, (2 - s), (s - 2), s,
                                (2 * s), (s - 3), (3 - (2 * s)), -s,
                                -s, 0, s, 0,
                                0, 1, 0, 0
                                );
    
    glm::vec4 parameter = glm::vec4(u*u*u, u*u, u, 1);
    
    return control * basis * parameter;
}

// use the subdivision technique
void subdivideSpline(float u0, float u1, float minimun_U, glm::mat4x3 control, std::vector<glm::vec3>* outcome) {
    
    float u_Middle = (u0 + u1) / 2;
    
    glm::vec3 v0 = catmullRom(u0, control);
    glm::vec3 v1 = catmullRom(u1, control);
    
    
    if (glm::length(v1 - v0) > 0.1f) {
        subdivideSpline(u0, u_Middle, minimun_U, control, outcome);
        subdivideSpline(u_Middle, u1, minimun_U, control, outcome);
    }
    else {
        outcome->push_back(v1);
    }
    
}

// For extra points: subdivision algorithm taking into account the curvature
//
//    void subdivideSpline(float u0, float u1, glm::vec3 tangent_0, glm::vec3 tangent_1,  glm::mat4x3 control, std::vector<glm::vec3>* outcome) {
//
//
//    float u_Middle = (u0 + u1) / 2;
//
//    glm::vec3 v0 = catmullRom(u0, control);
//    glm::vec3 v1 = catmullRom(u1, control);
//
//
//       
//        float angle = glm::degrees (acos((glm::dot(glm::normalize(tangent_0), glm::normalize(tangent_1))) / ((glm::length(glm::normalize(tangent_0))) * (glm::length(glm::normalize(tangent_1))))));
//
//
//    glm::vec3 t_Middle = glm::normalize(v1 - v0);
//
//    if (angle > 1.00f && (glm::length(v1-v0) > 0.03)) {
//        subdivideSpline(u0, u_Middle, tangent_0, t_Middle,  control, outcome);
//        subdivideSpline(u_Middle, u1, t_Middle, tangent_1,  control, outcome);
//    }
//    else {
//        outcome->push_back(v1);
//    }
//
//}

// generate spline Vertices
std::vector<glm::vec3> generateSplines(std::vector<glm::vec3> inputVertices) {
    
    std::vector<glm::vec3> splineVertices;
    
    if (inputVertices.size() > 3) {
        
        // Push the second vertex
        splineVertices.push_back(inputVertices[1]);
        
        // Generate a spline for each 4 control points
        for (int i = 0, n = user_clicks.size(); i < (n - 3); i++) {
            glm::mat4x3 control = glm::mat4x3(
                                              user_clicks[i].x,			user_clicks[i].y,			user_clicks[i].z,
                                              user_clicks[i + 1].x,		user_clicks[i + 1].y,		user_clicks[i + 1].z,
                                              user_clicks[i + 2].x,		user_clicks[i + 2].y,		user_clicks[i + 2].z,
                                              user_clicks[i + 3].x,		user_clicks[i + 3].y,		user_clicks[i + 3].z
                                              );
            // Pushes middle vertices for equal distance
           subdivideSpline(0.0f, 1.0f, 0.1f, control, &splineVertices); // for general subdivision
            
            // Pushes middle vertices for taking into account the curvature
           //glm::vec3 tangent_0 = glm::normalize(user_clicks[i+2] - user_clicks[i]);
           // glm::vec3 tangent_1 = glm::normalize(user_clicks[i+3] - user_clicks[i+1]);
            
           // subdivideSpline(0.0f, 1.0f, tangent_0, tangent_1,  control, &splineVertices); // for extra points
            
            
            
            
        }
    }
    return splineVertices;
}




// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    std::cout << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    
    //resets the application i.e. removes all control points, etc, and asks the user for a new  number of points N
    if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS) {
        if(part2 != true)
        {
            user_clicks.clear();
            Tangents.clear();
            reset = true;
            glfwSetWindowShouldClose(window, GL_TRUE);
            counter = 1;
        }
        else
            pos = 0;
       
        
    }
    
    // compute and draw the spline based on the control points entered so far
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS){
        
        if(part2 != true)
        {
        glm::vec3 first = user_clicks[0];
        glm::vec3 last = user_clicks[user_clicks.size()-1];
        user_clicks = generateSplines(user_clicks);
        line = user_clicks;
        user_clicks.push_back(last);
        user_clicks.insert(user_clicks.begin(), first);
        
       
        
        // rails   calculate and store the tangent coords
        glm::vec3 previous_rail;
        glm::vec3 tangent;
        float angle = 0;
        glm::vec3 rot_axis = glm::vec3(0,1,0);
        for(int i = 0; i < user_clicks.size(); i++)
        {
            glm:: vec3 rail_position;
            rail_position.x = user_clicks[i].x * 12;
            rail_position.z = -user_clicks[i].y * 12;
            rail_position.y = 0.0f;
            
            if(i != 0)
            {
                tangent = rail_position - previous_rail;
                tangent = glm::normalize(tangent);
                
                Tangents.push_back(tangent);
            }
            
            previous_rail = rail_position;
        }

        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, user_clicks.size() * sizeof(glm::vec3), &user_clicks[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
    }
        else
            turnOn = ! turnOn;
            
}

    // rendering mode controls
    
    if(key == GLFW_KEY_P)
    {
        renderMode = POINTS;
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, user_clicks.size() * sizeof(glm::vec3), &user_clicks[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    if(key == GLFW_KEY_L)
    {
        renderMode = LINES;
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, line.size() * sizeof(glm::vec3), &line[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
    }
    
    if(key == GLFW_KEY_B && action == GLFW_PRESS)
    {
        stop = ! stop;
    }
    
   
   
    
    
    if(key == GLFW_KEY_SPACE)
    {
        part2 = true;
        shader = new Shader("vertex2.shader", "fragment2.shader");
        shader->use();
        
        //ground
        glBindBuffer(GL_ARRAY_BUFFER, VBO2);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        
        
        // rail
        glBindBuffer(GL_ARRAY_BUFFER, VBO3);
        glBufferData(GL_ARRAY_BUFFER, sizeof(rails), rails, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        // normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3* sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    
    //test
    if (key == GLFW_KEY_S  )
    {
        cameraPos.z -= 1.0f;
    }
    if (key == GLFW_KEY_W  )
    {
        cameraPos.z += 1.0f;
    }
    if (key == GLFW_KEY_A  )
    {
        cameraPos.x -= 1.0f;
    }
    if (key == GLFW_KEY_D  )
    {
        cameraPos.x += 1.0f;
    }



}


void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    projection_matrix = glm::perspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 10000.0f);
    
}



// The MAIN function, from here we start the application and run the game loop
int main()
{
    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;

   
    
    // ask user for inputs
    
    
do {
        reset = false;
    
        c;
        do {
           
            std::cout << "Please enter the number of points (N >= 4) for the spline" << std::endl;
            std::cin >> c;
        } while (c <= 3);
  
    
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );// on Mac OS X initialization code for it to work
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Assignment-2 Roller Coster", nullptr, nullptr);
    
    
    
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    
    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    
    glViewport(0, 0, width, height);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
     //Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
     //Enable backface culling
    glEnable(GL_CULL_FACE);
    
    
    
    glGenVertexArrays(1, &VAO);
    glGenVertexArrays(1, &VAO2);
    glGenVertexArrays(1, &VAO3);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &VBO3);
    glGenBuffers(1, &EBO);
    
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glBufferData(GL_ARRAY_BUFFER, user_clicks.size() * sizeof(glm::vec3), &user_clicks[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    
    // position attribute
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
  
    
    //glVertexAttribDivisor(0, 1); //update this attribute per instance, not per vertex.
    //glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
    
    ////////////////////////
    glBindVertexArray(VAO2);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3* sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    // texture attribute
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(6* sizeof(GLfloat)));
//    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //////////////////////////
    
//    GLuint texture;
//    int wid, hig;
//    glGenTextures(1, &texture);
//    glBindTexture(GL_TEXTURE_2D, texture);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    
//    unsigned char *image = SOIL_load_image("grass.jpg", &wid, &hig, 0, SOIL_LOAD_RGBA);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, wid, hig, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
//    glGenerateMipmap(GL_TEXTURE_2D);
//    SOIL_free_image_data(image);
//    glBindTexture(GL_TEXTURE_2D, 0);
    
    

    
    
    /////////////////////////
     // rails
    glBindVertexArray(VAO3);
   
    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rails), rails, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3* sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    /////////////////////////////////
    
    //
    projection_matrix = glm::perspective(45.0f, (GLfloat)width/(GLfloat)height, 0.01f, 10000.0f);
  
   
    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        
        
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        
        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
//        GLuint projectionMatrixLoc = glGetUniformLocation(shaderProgram, "projection_matrix");
//        glm::mat4 projection_matrix;
//        //projection_matrix = glm::ortho(0.0f, 800.0f, 0.0f, 800.0f, 0.01f, 100.0f);
//        glUniformMatrix4fv(projectionMatrixLoc,  1, GL_FALSE, glm::value_ptr(projection_matrix));
        
        if(part2 == true)
        {
            shader = new Shader("vertex2.shader", "fragment2.shader");
            shader->use();
            
            glClearColor(0.0f, 0.3f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            // ground transform
            glm::mat4 model_matrix;
            glm::mat4 view_matrix;
            //glm::mat4 projection_matrix;
         
            
        
           // projection_matrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.01f, 10000.0f);
            // Get matrix's uniform location and set matrix
            GLuint modelMatrixLoc = glGetUniformLocation(shader->program, "model_matrix");
            GLuint viewMatrixLoc = glGetUniformLocation(shader->program, "view_matrix");
            GLuint projectionMatrixLoc = glGetUniformLocation(shader->program, "projection_matrix");
            
         
            
            for(int i = 0; i < 10000; i++)
            {
                for(int j = 0; j < 10000; j++)
                {
                    
                }
            }
            
            if (!stop)
            {
                
                ++pos;
                
                if (pos == Tangents.size()) {
                    pos = 0;
                }
                
                if (Tangents[pos].x == 0 && Tangents[pos].y == 0 && Tangents[pos].z == 0) {
                    ++pos;
                }
                
                cameraPos.x = user_clicks[pos].x * 12;    // set the camera position each time
                cameraPos.z = -user_clicks[pos].y * 12;
                cameraPos.y = 0.25f;
                cameraFront = Tangents[pos];
                
            }
            
        
            view_matrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            glUniformMatrix4fv(viewMatrixLoc,  1, GL_FALSE, glm::value_ptr(view_matrix));
            
            
            if(turnOn == true)
            {
            // Directional light
            glUniform3f(glGetUniformLocation(shader->program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
            
            glUniform3f(glGetUniformLocation(shader->program, "dirLight.ambient"), 0.05f, 0.05f, 0.1f);
            
            glUniform3f(glGetUniformLocation(shader->program, "dirLight.diffuse"), 0.2f, 0.2f, 0.7f);
            
            glUniform3f(glGetUniformLocation(shader->program, "dirLight.specular"), 0.7f, 0.7f, 0.7f);
            
            // Point light 
            glUniform3f(glGetUniformLocation(shader->program, "pointLight.position"), 0.3f,  0.3f,  0.7f);
            
            glUniform3f(glGetUniformLocation(shader->program, "pointLight.ambient"), 0.6f, 0.6f, 0.6f);
            
            glUniform3f(glGetUniformLocation(shader->program, "pointLight.diffuse"), 0.3f,  0.3f,  0.7f);
            
            glUniform3f(glGetUniformLocation(shader->program, "pointLight.specular"), 0.3f,  0.3f,  0.7f);
            
            glUniform1f(glGetUniformLocation(shader->program, "pointLight.constant"), 1.0f);
            
            glUniform1f(glGetUniformLocation(shader->program, "pointLight.linear"), 0.09);
            
            glUniform1f(glGetUniformLocation(shader->program, "pointLight.quadratic"), 0.032);
            
            // SpotLight
            glUniform3f(glGetUniformLocation(shader->program, "spotLight.position"), cameraPos.x, cameraPos.y, cameraPos.z );
            
            glUniform3f(glGetUniformLocation(shader->program, "spotLight.direction"), cameraFront.x, cameraFront.y, cameraFront.z );
            
            glUniform3f(glGetUniformLocation(shader->program, "spotLight.ambient"), 0.7f, 0.7f, 0.7f);
            
            glUniform3f(glGetUniformLocation(shader->program, "spotLight.diffuse"), 0.8f, 0.8f, 0.8f);
            
            glUniform3f(glGetUniformLocation(shader->program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
            
            glUniform1f(glGetUniformLocation(shader->program, "spotLight.constant"), 1.0f);
            
            glUniform1f(glGetUniformLocation(shader->program, "spotLight.linear"), 0.09);
            
            glUniform1f(glGetUniformLocation(shader->program, "spotLight.quadratic"), 0.032);
            
            glUniform1f(glGetUniformLocation(shader->program, "spotLight.cutOff"), glm::cos(glm::radians(10.0f)));
            
            glUniform1f(glGetUniformLocation(shader->program, "spotLight.outerCutOff"), glm::cos(glm::radians(13.0f)));
                
            }
            
            GLint viewPosLoc = glGetUniformLocation(shader->program, "viewPos");
            glUniform3f(viewPosLoc, 0.0f, 0.0f, -1.0f);
            
        
            // ground
           // glActiveTexture(GL_TEXTURE0);
          //  glBindTexture(GL_TEXTURE_2D, texture);
          //  glUniform1i(glGetUniformLocation(shader->program, "ourTexture"), 0);
            
            glBindBuffer(GL_ARRAY_BUFFER, VBO2);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
            
            
            // rail
            glBindBuffer(GL_ARRAY_BUFFER, VBO3);
            glBufferData(GL_ARRAY_BUFFER, sizeof(rails), rails, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            
            
            // normal attribute
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3* sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
                       
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            
            glBindVertexArray(VAO2);
            model_matrix = glm::scale(model_matrix, glm::vec3(50000.0f, 50000.0f, 50000.0f));
            glUniformMatrix4fv(modelMatrixLoc,  1, GL_FALSE, glm::value_ptr(model_matrix));
            
            glUniformMatrix4fv(projectionMatrixLoc,  1, GL_FALSE, glm::value_ptr(projection_matrix));
            
            GLint objectColorLoc = glGetUniformLocation(shader->program, "objectColor");
            glUniform3f(objectColorLoc, 0.6f, 0.3f, 0.2f);
            
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
            
            glBindVertexArray(VAO3);
           
            //glUniformMatrix4fv(viewMatrixLoc,  1, GL_FALSE, glm::value_ptr(view_matrix));
            glUniformMatrix4fv(projectionMatrixLoc,  1, GL_FALSE, glm::value_ptr(projection_matrix));
            objectColorLoc = glGetUniformLocation(shader->program, "objectColor");
            glUniform3f(objectColorLoc, 0.2f, 0.5f, 0.7f);
            
            // draw rails
            for(int i = 0; i < user_clicks.size(); i++)
            {
                // rail transform
                 glm::mat4 model_matrix_rail;
                 model_matrix_rail = glm::translate(model_matrix_rail, glm::vec3((user_clicks[i].x * 12) , 0.0f, -(user_clicks[i].y * 12)) );
                
//                glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
//                glm::vec3 check = glm::normalize( glm::vec3((user_clicks[i+1].x * 12 - user_clicks[i].x * 12), 0.0f, -(user_clicks[i+1].y * 12 - user_clicks[i].y * 12)));
//                if(check.z > 0)
//                                    {
//                                        
//                                        float diff = glm::dot(check, xAxis);
//                                        float angle = glm::degrees( glm::acos( diff) );
//                                        model_matrix_rail = glm::rotate(model_matrix_rail, -angle-90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
//                                    }
//                                    if(check.z < 0)
//                                    {
//                                       
//                                        float diff = glm::dot(check, xAxis);
//                                       float angle = glm::degrees( glm::acos( diff) );
//                                        model_matrix_rail = glm::rotate(model_matrix_rail, angle + 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
//                                    }
//                                    
//                                    if(check.z == 0)
//                                    {
//                                        
//                                        model_matrix_rail = glm::rotate(model_matrix_rail, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
//                                    }

                
            

                
                if(Tangents[i].z > 0)
                {
                    glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
                    float diff = glm::dot(Tangents[i], xAxis);
                    float angle = glm::degrees( glm::acos( diff)) ;
                    model_matrix_rail = glm::rotate(model_matrix_rail, -angle-90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                }
                if(Tangents[i].z < 0)
                {
                    glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
                    float diff = glm::dot(Tangents[i], xAxis);
                    float angle = glm::degrees( glm::acos( diff) + 3.14f) ;
                    model_matrix_rail = glm::rotate(model_matrix_rail, angle + 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                }
                
                if(Tangents[i].z == 0)
                {
                    
                    model_matrix_rail = glm::rotate(model_matrix_rail, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                }
                
                
                 model_matrix_rail = glm::scale(model_matrix_rail, glm::vec3(0.4f, 0.05f, 0.3f));
                 glUniformMatrix4fv(modelMatrixLoc,  1, GL_FALSE, glm::value_ptr(model_matrix_rail));
                 glDrawArrays(GL_TRIANGLES, 0, 36);
            }
           
            glBindVertexArray(0);
        }
       
        if(part2 != true)
        {
            shader = new Shader("vertex.shader", "fragment.shader");
            shader->use();
            
            glBindVertexArray(VAO);
            if(renderMode == POINTS)
            {
                glDrawArrays(GL_POINTS, 0, user_clicks.size());
                glPointSize(4.0);
            }
            
            else if(renderMode == LINES)
            {
                glDrawArrays(GL_LINE_STRIP, 0, line.size());
            }
            
            
            glBindVertexArray(0);
        }
        
        
        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    
 } while (reset);
return 0;
}
