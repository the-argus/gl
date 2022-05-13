#version 330 core

out vec4 FragColor;
uniform vec4 timeColor;

in vec2 TexCoord;

uniform sampler2D jaypehg;

void main()
{
    FragColor = timeColor * texture(jaypehg, TexCoord);
};
