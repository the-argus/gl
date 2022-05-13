#version 330 core
out vec4 FragColor;

in vec3 vColor;
in vec2 TexCoord;

uniform sampler2D texture1;

void main()
{
	vec4 col = texture(texture1, TexCoord);
    FragColor = col * vec4(vColor, 0.0f);
}
