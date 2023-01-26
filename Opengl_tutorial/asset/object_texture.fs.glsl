#version 330 core
struct Material {
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 fragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

//uniform sampler2D texture1;
//uniform sampler2D texture2;
//uniform float mixVal;

uniform Material material;
uniform Light light;
uniform PointLight pointLight;

uniform vec3 viewPos;

vec3 calcPointLight(vec3 norm, vec3 lightDir, vec3 diffMap, vec3 specularMap);

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(pointLight.position - FragPos);
    vec3 diffMap = vec3(texture(material.diffuse, TexCoord));
    vec3 specularMap = vec3(texture(material.specular, TexCoord));
    
    fragColor = vec4(calcPointLight(norm, viewDir, diffMap, specularMap), 1.0);
}

vec3 calcPointLight(vec3 norm, vec3 lightDir, vec3 diffMap, vec3 specularMap) {
 
    vec3 ambient = pointLight.ambient * diffMap;
    
    //diffuse
    float diff = max(dot(norm,lightDir), 0.0);
    vec3 diffuse = pointLight.diffuse * (diff * diffMap);
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0) , material.shininess * 128);
    vec3 specular = pointLight.specular * spec * specularMap;
    
    return vec3(ambient + diffuse + specular);
}
