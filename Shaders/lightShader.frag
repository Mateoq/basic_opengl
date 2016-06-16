#version 330 core

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

// uniform vec4 xColor; // Variable from OpenGL code
//in vec2 texCoord;
in vec3 fNormal;
in vec3 fragPos;
in vec3 fLightPos;

out vec4 color;

uniform sampler2D fTexture1;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main() {
  //vec2 fTexCoord = vec2(texCoord.x, 1.0f - texCoord.y);

  // Ambient
  // =======================
  float ambientStrength = 0.1f;
  vec3 ambient = ambientStrength * lightColor;

  // Diffuse
  // =======================
  vec3 norm = normalize(fNormal);
  // Calculate and normalize the direction ligth vector
  vec3 lightDirection = normalize(fLightPos - fragPos);
  // Calculate the diffuse impact
  float diff = max(dot(norm, lightDirection), 0.0);
  vec3 diffuse = diff * lightColor;
  
  // Specular
  // =======================
  // Specular strength
  float specularStrength = 1.0f;
  // Calculate view direction and reflection direction
  vec3 viewDirection = normalize(viewPos - fragPos);
  vec3 reflectDirection = reflect(-lightDirection, norm);
  //  Calculate specular component
  float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), 64);
  vec3 specular = specularStrength * spec * lightColor;

  // Result color
  vec3 result = (ambient + diffuse + specular) * objectColor;
  color = vec4(result, 1.0f);
  // color = mix(texture(fTexture1, fTexCoord), texture(fTexture2, fTexCoord), 0.2);
}

