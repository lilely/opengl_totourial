#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct TextureMaterial {
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

struct DirLight {
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    
    float cutOff;
    float outerCutOff;
    
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
uniform TextureMaterial textMaterial;

uniform int hasTexture;

uniform Light light;
uniform PointLight pointLight;
uniform DirLight dirLight;
uniform SpotLight spotLight;

uniform vec3 viewPos;

vec3 calcPointLight(vec3 norm, vec3 viewDir, vec3 diffMap, vec3 specularMap);
vec3 calcDirLight(vec3 norm, vec3 viewDir, vec3 diffMap, vec3 specularMap);
vec3 calcSpotLight(vec3 norm, vec3 viewDir, vec3 diffMap, vec3 specularMap);

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 diffMap;
    vec3 specularMap;
    if(hasTexture > 0) {
        diffMap = vec3(texture(textMaterial.diffuse, TexCoord));
        specularMap = vec3(texture(textMaterial.specular, TexCoord));
    } else {
        diffMap = material.diffuse;
        specularMap = material.specular;
    }

    fragColor = vec4(calcSpotLight(norm, viewDir, diffMap, specularMap), 1.0);
}

vec3 calcSpotLight(vec3 norm, vec3 viewDir, vec3 diffMap, vec3 specularMap) {
    // ambient
    vec3 ambient = spotLight.ambient * diffMap;
    
    vec3 lightDir = normalize(spotLight.position - FragPos);
    float theta = dot(lightDir, normalize(-spotLight.direction));
    
    if(theta > spotLight.outerCutOff) {
        //diffuse
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = spotLight.diffuse * (diff *diffMap);
        
        // specular
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0) , material.shininess * 128);
        vec3 specular = spotLight.specular * (spec * specularMap);
        
        float intensity = (theta - spotLight.outerCutOff) / (spotLight.cutOff - spotLight.outerCutOff);
        intensity = clamp(intensity, 0.0, 1.0);
        diffuse *= intensity;
        specular *= intensity;
        
        return vec3(ambient + diffuse + specular);
    }
    return ambient;
}

vec3 calcDirLight(vec3 norm, vec3 viewDir, vec3 diffMap, vec3 specularMap) {
    // ambient
    vec3 ambient = dirLight.ambient * diffMap;
    
    //diffuse
    vec3 lightDir = normalize(-dirLight.direction);
    float diff = max(dot(norm,lightDir), 0.0);
    vec3 diffuse = dirLight.diffuse * (diff * diffMap);
    
    // specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0) , material.shininess * 128);
    vec3 specular = dirLight.specular * spec * specularMap;
    
    return vec3(ambient + diffuse + specular);
}

vec3 calcPointLight(vec3 norm, vec3 viewDir, vec3 diffMap, vec3 specularMap) {
 
    vec3 ambient = pointLight.ambient * diffMap;
    
    //diffuse
    vec3 lightDir = normalize(pointLight.position - FragPos);
    float diff = max(dot(norm,lightDir), 0.0);
    vec3 diffuse = pointLight.diffuse * (diff * diffMap);
    
    // specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0) , material.shininess * 128);
    vec3 specular = pointLight.specular * spec * specularMap;
    
    return vec3(ambient + diffuse + specular);
}
