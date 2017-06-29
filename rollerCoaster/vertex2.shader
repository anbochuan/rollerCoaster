
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
//layout (location = 2) in vec2 texCoord;



uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;


out vec3 Normal;
out vec3 FragPos;
//out vec2 TexCoord;


void main()
{
    vec3 basePosition = position;


    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(basePosition, 1.0);

    FragPos = vec3(model_matrix * vec4(basePosition, 1.0f));

    Normal = vec3(model_matrix * vec4(normal, 0.0)) ;

    //TexCoord = vec2 (texCoord.x, 1.0 - texCoord.y);
     //Normal = normal;
}
