#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D jaypehg;

void main()
{
    // sample a value at TexCoord from jaypehg texture
    FragColor = texture(jaypehg, TexCoord);
}
