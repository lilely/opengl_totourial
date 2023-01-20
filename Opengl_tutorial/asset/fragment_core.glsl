#version 330 core
out vec4 fragColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixVal;

void main() {
//    fragColor = vec4(0.31f,0.45f,0.98f,1.0f);
//    fragColor = aColor;
    fragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixVal);
//    fragColor = texture(texture1, TexCoord);
}
