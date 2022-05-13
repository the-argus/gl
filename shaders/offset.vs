
#version 330 core

layout (location = 0) in vec3 aPos;

uniform vec3 vOffset;

uniform mat4 transform;

void main()
{
   gl_Position = transform * vec4(aPos.x + vOffset.x, aPos.y + vOffset.y,
           aPos.z + vOffset.z, 1.0);
}
