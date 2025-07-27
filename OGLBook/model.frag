#version 460 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;


struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};

struct Light{
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform Light light;
uniform Material material;
uniform vec3 viewPos;

void main()
{
  //ambient

  vec3 ambient = light.ambient * texture(material.texture_diffuse1, TexCoords).rgb;

  //diffuse

  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(-light.direction);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse1, TexCoords).rgb;

  //specular

  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = light.specular * spec * texture(material.texture_specular1, TexCoords).rgb;

  vec3 result = diffuse + ambient + specular;

  FragColor = vec4(result, 1.0);
}