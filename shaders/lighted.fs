#version 330 core
out vec4 FragColor;
 
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec3 lightColor;

void main()
{
    vec4 tint = vec4(lightColor, 1.0);
    FragColor = texture(ourTexture, TexCoord) * tint;
}
