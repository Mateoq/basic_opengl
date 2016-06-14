#version 330 core

// uniform vec4 xColor; // Variable from OpenGL code
//in vec2 texCoord;
in vec3 fNormal;
in vec3 fragPos;

out vec4 color;

uniform sampler2D fTexture1;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main() {
  //vec2 fTexCoord = vec2(texCoord.x, 1.0f - texCoord.y);
  // Normalize the normal vector
  vec3 norm = normalize(fNormal);
  // Calculate and normalize the direction ligth vector
  vec3 lightDirection = normalize(lightPos - fragPos);

  // Calculate the diffuse impact
  float diff = max(dot(norm, lightDirection), 0.0);
  vec3 diffuse = diff * lightColor;

  // Calculate ambient light
  float ambientStrength = 0.1f;
  vec3 ambient = ambientStrength * lightColor;

  // Result color
  vec3 result = (ambient + diffuse) * objectColor;
  color = vec4(result, 1.0f);
  // color = mix(texture(fTexture1, fTexCoord), texture(fTexture2, fTexCoord), 0.2);
}

