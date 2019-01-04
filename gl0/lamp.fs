#version 330 core
out vec4 FragColor;
uniform vec3 lampColor;
void main()
{
    FragColor = vec4(lampColor,1.0); // set alle 4 vector values to 1.0
}