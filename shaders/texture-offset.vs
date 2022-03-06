#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform vec3 vOffset;

void main()
{
    gl_Position = vec4(aPos.x + vOffset.x, aPos.y + vOffset.y,
           aPos.z + vOffset.z, 1.0);
    TexCoord = aTexCoord;
}
