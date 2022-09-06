#version 330 core
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
    Normal = aNormal;
    FragPosition = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
