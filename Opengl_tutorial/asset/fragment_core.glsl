#version 330 core
out vec4 fragColor;
in vec4 aColor;

void main() {
//    fragColor = vec4(0.31f,0.45f,0.98f,1.0f);
    fragColor = aColor;
}
