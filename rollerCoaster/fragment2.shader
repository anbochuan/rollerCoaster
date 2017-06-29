#version 330 core



struct DirLight {  // directional light
vec3 direction;

vec3 ambient;
vec3 diffuse;
vec3 specular;
};

struct PointLight { // point light
vec3 position;

float constant;
float linear;
float quadratic;

vec3 ambient;
vec3 diffuse;
vec3 specular;
};

struct SpotLight {  // flash light
vec3 position;
vec3 direction;

float cutOff;
float outerCutOff;

float constant;
float linear;
float quadratic;

vec3 ambient;
vec3 diffuse;
vec3 specular;
};


in vec3 FragPos;
in vec3 Normal;
//in vec2 TexCoord;



out vec4 color;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;
uniform vec3 objectColor;
//uniform sampler2D ourTexture1;

// Functions

// Calculates the color when using a directional light.

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 4);  // shininess = 4
    // Combine results
    vec3 ambientResult = light.ambient * objectColor;
    vec3 diffuseResult = light.diffuse * diff * objectColor;
    vec3 specularResult = light.specular * spec * objectColor;
    return (ambientResult + diffuseResult + specularResult);
}

// Calculates the color when using a point light.

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 4);  // shininess = 4
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // Combine results
    vec3 ambient = light.ambient * objectColor;
    vec3 diffuse = light.diffuse * diff * objectColor;
    vec3 specular = light.specular * spec * objectColor;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// Calculates the color when using a spot light.

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 4); // shininess = 4
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // Spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // Combine results
    vec3 ambient = light.ambient * objectColor;
    vec3 diffuse = light.diffuse * diff * objectColor;
    vec3 specular = light.specular * spec * objectColor;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

void main()
{

    vec3 norm = normalize(Normal);

    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir); // calculate Directional light

    result += CalcPointLight(pointLight, norm, FragPos, viewDir); // calculate Point light

    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);  // calculate flash light

    color = vec4(result, 1.0);

 //color = texture(ourTexture1, TexCoord);

}
