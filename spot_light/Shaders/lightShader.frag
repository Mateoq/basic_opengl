#version 330 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct Light {
  vec3 position;
  vec3 direction; // Spotlight direction
  float cutOff;
  float outerCutOff;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
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

  // Ambient
  // =======================
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, fTexCoords));

  // Diffuse
  // =======================
  vec3 norm = normalize(fNormal);
  // Calculate and normalize the direction ligth vector
  vec3 lightDirection = normalize(light.position - fragPos);
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

  // Spotlight (soft edges)
  // =======================
  // Checking if the lighting is inside the spotlight cone  
  float theta = dot(lightDirection, normalize(-light.direction));
  float epsilon = light.cutOff - light.outerCutOff;
  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

  // We'll leave ambien unaffected so we always have a little light.
  diffuse *= intensity;
  specular *= intensity;
  
  // Attenuation
  // ========================
  float lDistance = length(light.position - fragPos);
  float attenuation = 1.0f / (light.constant + light.linear * lDistance +
			      light.quadratic * (lDistance * lDistance));

  //    ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;

  // Result color
  color = vec4(ambient + diffuse + specular, 1.0f);

}

