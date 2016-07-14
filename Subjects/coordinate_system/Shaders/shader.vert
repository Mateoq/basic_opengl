#version 330 core

layout (location = 0) in vec3 position; // The position variable has attribute position 0
layout (location = 1) in vec2 vTexCoord; // The texture variable has attribute position 2

out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  gl_Position = projection * view * model * vec4(position, 1.0f); // Directly give a vec3 to vec4's constructor
  texCoord = vTexCoord;
}
