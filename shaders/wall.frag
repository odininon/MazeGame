#version 330 core

out vec4 FragColor;

struct Material {
    float diffuse;
    float specular;
    float shininess;
};

struct Light {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform vec3 color;

void main()
{
    // ambient
    vec3 ambient = light.ambient * material.diffuse * color;

    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse * color;

    //specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * material.specular * color;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}