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

out vec4 fragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

//uniform sampler2D texture1;
//uniform sampler2D texture2;
//uniform float mixVal;

uniform Material material;
uniform Light light;

uniform vec3 viewPos;

void main() {
//    fragColor = vec4(0.31f,0.45f,0.98f,1.0f);
//    fragColor = aColor;
//    fragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixVal);
    //ambient
    vec3 ambient = light.ambient * material.ambient;
    
    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm,lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoord)));
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0) , material.shininess * 128);

    vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoord)));
    
    fragColor = vec4(vec3(ambient + diffuse + specular), 1.0);
}
