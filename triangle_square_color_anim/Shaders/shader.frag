#version 330 core

uniform vec4 xColor; // Variable from OpenGL code
in vec3 vColor; // Variable from vertex shader

out vec4 color;

void main() {
  color = vec4(vColor, 1.0f) + xColor;
}

