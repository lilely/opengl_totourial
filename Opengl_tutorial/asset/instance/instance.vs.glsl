#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 aCoord;
layout (location = 3) in vec3 aOffset;
layout (location = 4) in vec3 aSize;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec3 pos = vec3(aPos.x * aSize.x, aPos.y * aSize.y, aPos.z * aSize.z) + aOffset;
//    vec3 pos = aPos + aOffset;
    FragPos = vec3(model * vec4(pos, 1.0f));
    Normal = mat3(transpose(inverse(model))) * normal;
    gl_Position = projection * view * vec4(FragPos, 1.0f);
    TexCoord = aCoord;
}
