#version 330 core
out vec4 FragColor;
 
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPosition;

uniform sampler2D ourTexture;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform float ambientStrength;
uniform float diffuseStrength;

void main()
{
    // ambient light -----------------------------------------------------
    vec3 ambientLight = ambientStrength * lightColor;

    // material / texture color ------------------------------------------
    vec3 color = vec3(texture(ourTexture, TexCoord));
    
    // diffuse light -----------------------------------------------------
    // Normal might not actually be... normal
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(lightPos) - normalize(FragPosition);
    
    // the "max" is there because if the vertex is facing away from
    // the light, it should just have 0 light, not negative
    vec3 diffuseLight = max(dot(norm, lightDirection), 0.0) * lightColor
        * diffuseStrength;
    
    // final color -------------------------------------------------------
    FragColor = vec4((diffuseLight + ambientLight) * color, 1.0);
}
