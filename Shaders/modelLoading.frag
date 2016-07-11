#version 330 core

#define NUMBER_POINT_LIGHTS 4

struct Material {
  sampler2D texture_diffuse1;
  sampler2D texture_specular1;
  float shininess;
};

struct DirectionLight {
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight {
  vec3 position;

  float constant;
  float linear;
  float quadratic;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct SpotLight {
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

out vec4 color;

uniform vec3 viewPos;
uniform Material material;
uniform DirectionLight directionLight;
uniform PointLight pointLights[NUMBER_POINT_LIGHTS];
uniform SpotLight spotLight;

vec3 calcDirectionLight(DirectionLight light, vec3 normal, vec3 viewDirection);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDirection);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDirection);

void main() {
  // Properties
  vec3 norm = normalize(fNormal);
  vec3 viewDirection = normalize(viewPos - fragPos);

  // Phase 1: Directional lighting
  vec3 result = calcDirectionLight(directionLight, norm, viewDirection);
  // Phase 2: Point lights
  for(int i = 0; i < NUMBER_POINT_LIGHTS; i++) {
    result += calcPointLight(pointLights[i], norm, viewDirection);
  }
  // Phase 3: Spot light
  result += calcSpotLight(spotLight, norm, viewDirection);

  // Result color
  color = vec4(result, 1.0f);

}

vec3 calcDirectionLight(DirectionLight light, vec3 normal, vec3 viewDirection) {
  vec3 lightDirection = normalize(-light.direction);
  // Diffuse shading
  float diff = max(dot(normal, lightDirection), 0.0f);
  // Specular shading
  vec3 reflectDirection = reflect(-lightDirection, normal);
  float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), material.shininess);
  // Combine results
  vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, fTexCoords));
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, fTexCoords));
  vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, fTexCoords));

  return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDirection) {
  vec3 lightDirection = normalize(light.position - fragPos);
  // Diffuse shading
  float diff = max(dot(normal, lightDirection), 0.0f);
  // Specular shading
  vec3 reflectDirection = reflect(-lightDirection, normal);
  float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), material.shininess);
  // Attenuation
  float lDistance = length(light.position - fragPos);
  float attenuation = 1.0f / (light.constant + light.linear * lDistance +
			      light.quadratic * (lDistance * lDistance));
  
  // Combine results
  vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, fTexCoords));
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, fTexCoords));
  vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, fTexCoords));

  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;

  return (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDirection) {
  vec3 lightDirection = normalize(light.position - fragPos);

  // Diffuse shading
  float diff = max(dot(normal, lightDirection), 0.0);
  
  // Specular shading
  vec3 reflectDirection = reflect(-lightDirection, normal);
  float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), material.shininess);

  // Spotlight (soft edges)
  float theta = dot(lightDirection, normalize(-light.direction));
  float epsilon = light.cutOff - light.outerCutOff;
  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

  // Attenuation
  float lDistance = length(light.position - fragPos);
  float attenuation = 1.0f / (light.constant + light.linear * lDistance +
			      light.quadratic * (lDistance * lDistance));

  // Combine results
  vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, fTexCoords));
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, fTexCoords));
  vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, fTexCoords));

  ambient *= attenuation * intensity;
  diffuse *= attenuation * intensity;
  specular *= attenuation * intensity;

  return (ambient + diffuse + specular);
}

