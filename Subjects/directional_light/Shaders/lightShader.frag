#version 330 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct Light {
  //  vec3 position; // No longer necessary when using directional lights
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

// uniform vec4 xColor; // Variable from OpenGL code
in vec2 fTexCoords;
in vec3 fNormal;
in vec3 fragPos;
//in vec3 fLightPos;

out vec4 color;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main() {
  //vec2 fTexCoord = vec2(texCoord.x, 1.0f - texCoord.y);

  // Ambient
  // =======================
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, fTexCoords));

  // Diffuse
  // =======================
  vec3 norm = normalize(fNormal);
  // Calculate and normalize the direction ligth vector
  vec3 lightDirection = normalize(-light.direction);
  // Calculate the diffuse impact
  float diff = max(dot(norm, lightDirection), 0.0);
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fTexCoords));
  
  // Specular
  // =======================
  // Calculate view direction and reflection direction
  vec3 viewDirection = normalize(viewPos - fragPos);
  vec3 reflectDirection = reflect(-lightDirection, norm);
  //  Calculate specular component
  float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), material.shininess);
  vec3 specular = light.specular * spec * vec3(texture(material.specular, fTexCoords));

  // Result color
  color = vec4(ambient + diffuse + specular, 1.0f);
  // color = mix(texture(fTexture1, fTexCoord), texture(fTexture2, fTexCoord), 0.2);
}

