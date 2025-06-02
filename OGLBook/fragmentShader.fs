#version 460 core
layout(location = 1) in vec3 ourColor;

out vec4 FragColor;

void main()
{
 FragColor = vec4(ourColor, 1.f);
}