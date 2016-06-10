#version 330 core

// uniform vec4 xColor; // Variable from OpenGL code
in vec2 texCoord;

out vec4 color;

uniform sampler2D fTexture1;
uniform sampler2D fTexture2;

void main() {
  vec2 fTexCoord = vec2(texCoord.x, 1.0f - texCoord.y);
  color = mix(texture(fTexture1, fTexCoord), texture(fTexture2, fTexCoord), 0.2);
}

