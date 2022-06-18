#version 330 core
out vec4 FragColor;
 
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    // vec4 tint = vec4(1.1);
    // FragColor = texture(ourTexture, TexCoord) * tint;
    FragColor = vec4(1.0);
} 
