#version 460 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Material{
  sampler2D ambientDiffuse;
  sampler2D specular;
  float shininess;
};

struct DirLight{ //Sun type of light
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  vec3 direction;
};

struct PointLight{ //Street light type of light
  vec3 position;
  
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constantTerm;
  float linearTerm;
  float quadraticTerm;
};

struct SpotLight{ //Flashlight type of light
  vec3 position;
  vec3 direction;
  float cutOff;
  float outerCutOff;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constantTerm;
  float linearTerm;
  float quadraticTerm;
};

#define NUMBER_OF_POINTLIGHTS 4

uniform Material material;
uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NUMBER_OF_POINTLIGHTS];
uniform SpotLight spotLight;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(viewPos - FragPos);
  
  vec3 result = calcDirLight(dirLight, norm, viewDir);

  for(int i=0; i<NUMBER_OF_POINTLIGHTS; ++i){
    result += calcPointLight(pointLights[i], norm, FragPos, viewDir);
  }

  result += calcSpotLight(spotLight, norm, FragPos, viewDir);

  FragColor = vec4(result, 1.0);
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir){
  vec3 lightDir = normalize(-light.direction);

  //Calculate ambient
  vec3 ambient = light.ambient * texture(material.ambientDiffuse, TexCoords).rgb;

  //Calculate diffuse
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * texture(material.ambientDiffuse, TexCoords).rgb;

  //Calculate specular
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

  return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
  vec3 lightDir = normalize(light.position - fragPos);

  //Calculate ambient
  vec3 ambient = light.ambient * texture(material.ambientDiffuse, TexCoords).rgb;

  //Calculate diffuse
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * texture(material.ambientDiffuse, TexCoords).rgb;

  //Calculate specular
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

  //Calculate distance and attenuation
  float distance    = length(light.position - FragPos);
  float attenuation = 1.0 / (light.constantTerm + light.linearTerm * distance + light.quadraticTerm * (distance * distance)); 

  ambient  *= attenuation;
  diffuse  *= attenuation;
  specular *= attenuation;

  return (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
  vec3 lightDir = normalize(light.position - fragPos);
  
  //dot product(скал€рний добуток) between light on exact FragPos and general light direction
  float theta     = dot(lightDir, normalize(-light.direction));
  float epsilon   = light.cutOff - light.outerCutOff;
  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

  //ambient
  vec3 ambient = light.ambient * texture(material.ambientDiffuse, TexCoords).rgb;

  //diffuse
  vec3 norm = normalize(Normal);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * texture(material.ambientDiffuse, TexCoords).rgb;

  //specular
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

  //attenuation
  float distance    = length(light.position - FragPos);
  float attenuation = 1.0 / (light.constantTerm + light.linearTerm * distance + light.quadraticTerm * (distance * distance)); 
  //ambient  *= attenuation;
  diffuse  *= attenuation;
  specular *= attenuation;

  //intensity
  diffuse  *= intensity;
  specular *= intensity;

  return (ambient + diffuse + specular);
}