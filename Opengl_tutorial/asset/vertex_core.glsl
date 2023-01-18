#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 aCoord;

out vec4 aColor;
out vec2 TexCoord;

uniform mat4 transform; // set in code

void main() {
//    gl_Position = transform * vec4(aPos,1.0);
    gl_Position = vec4(aPos,1.0);
    TexCoord = aCoord;
    aColor = vec4(color,1.0);
}
