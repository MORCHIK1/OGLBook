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

struct Light{
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

uniform Light light;
uniform Material material;
uniform vec3 viewPos;

void main()
{
  vec3 lightDir = normalize(light.position - FragPos);
  
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

  vec3 viewDir = normalize(viewPos - FragPos);
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

  //result
  vec3 result = diffuse + ambient + specular;
  FragColor = vec4(result, 1.0);
}