#version 330 core

layout (location = 0) in vec3 position; // The position variable has attribute position 0
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords; // The texture variable has attribute position 2

out vec2 fTexCoords;
out vec3 fNormal;
out vec3 fragPos;
out vec3 fLightPos;

// We now define th uniform in the vertex shader and pass the "view space" lightPos to the fragment shader.
// light is currently in world space.
uniform vec3 lightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  gl_Position = projection * view * model * vec4(position, 1.0f); // Directly give a vec3 to vec4's constructor
  fragPos = vec3(view * model * vec4(position, 1.0f));
  
  fNormal = mat3(transpose(inverse(view * model))) * normal;
  fLightPos = vec3(view * vec4(lightPos, 1.0f)); // Transform world-space light position to view-space light position

  fTexCoords = texCoords;
}
