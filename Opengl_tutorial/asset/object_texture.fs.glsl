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

#define MAX_POINT_LIGHTS 20
struct PointLight {
    vec3 position;
    
    float k1;
    float k2;
    float k3;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int noPointLights;

#define MAX_SPOT_LIGHTS 5
struct SpotLight {
    vec3 position;
    vec3 direction;
    
    float k1;
    float k2;
    float k3;
    
    float cutOff;
    float outerCutOff;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform int noSpotLights;

struct DirLight {
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirLight dirLight;

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

uniform vec3 viewPos;

vec3 calcPointLight(int index, vec3 norm, vec3 viewDir, vec3 diffMap, vec3 specularMap);
vec3 calcSpotLight(int index, vec3 norm, vec3 viewDir, vec3 diffMap, vec3 specularMap);
vec3 calcDirLight(vec3 norm, vec3 viewDir, vec3 diffMap, vec3 specularMap);

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
    vec3 ret = calcDirLight(norm, viewDir, diffMap, specularMap);
    for(int i = 0;i < noPointLights;i++) {
        ret += calcPointLight(i, norm, viewDir, diffMap, specularMap);
    }
    
    for(int i = 0;i < noSpotLights;i++) {
        ret += calcSpotLight(i, norm, viewDir, diffMap, specularMap);
    }
//    ret += vec4(calcSpotLight(norm, viewDir, diffMap, specularMap), 1.0);
    fragColor = vec4(ret,1.0);
}

vec3 calcSpotLight(int index, vec3 norm, vec3 viewDir, vec3 diffMap, vec3 specularMap) {
    // ambient
    vec3 ambient = spotLights[index].ambient * diffMap;
    
    vec3 lightDir = normalize(spotLights[index].position - FragPos);
    float theta = dot(lightDir, normalize(-spotLights[index].direction));
    
    if(theta > spotLights[index].outerCutOff) {
        //diffuse
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = spotLights[index].diffuse * (diff *diffMap);
        
        // specular
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0) , material.shininess * 128);
        vec3 specular = spotLights[index].specular * (spec * specularMap);
        
        float intensity = (theta - spotLights[index].outerCutOff) / (spotLights[index].cutOff - spotLights[index].outerCutOff);
        intensity = clamp(intensity, 0.0, 1.0);
        diffuse *= intensity;
        specular *= intensity;
        
        float dist = length(spotLights[index].position - FragPos);
        float attention = 1 / (spotLights[index].k1 + spotLights[index].k2 * dist + spotLights[index].k3 * dist * dist);
        
        return vec3(ambient + diffuse + specular) * attention;
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

vec3 calcPointLight(int index, vec3 norm, vec3 viewDir, vec3 diffMap, vec3 specularMap) {
 
    vec3 ambient = pointLights[index].ambient * diffMap;
    
    //diffuse
    vec3 lightDir = normalize(pointLights[index].position - FragPos);
    float diff = max(dot(norm,lightDir), 0.0);
    vec3 diffuse = pointLights[index].diffuse * (diff * diffMap);
    
    // specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0) , material.shininess * 128);
    vec3 specular = pointLights[index].specular * spec * specularMap;
    
    float dist = length(pointLights[index].position - FragPos);
    float attention = 1 / (pointLights[index].k1 + pointLights[index].k2 * dist + pointLights[index].k3 * dist * dist);
    
    return vec3(ambient + diffuse + specular) * attention;
}
