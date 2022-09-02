#version 330 core
out vec4 FragColor;
 
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPosition;

uniform sampler2D ourTexture;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;
uniform int shininess;

void main()
{
    // ambient light -----------------------------------------------------
    vec3 ambientLight = ambientStrength * lightColor;

    // material / texture color ------------------------------------------
    vec3 color = vec3(texture(ourTexture, TexCoord));
    
    // diffuse light -----------------------------------------------------
    // Normal might not actually be... normal
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(lightPos - FragPosition);
    
    // the "max" is there because if the vertex is facing away from
    // the light, it should just have 0 light, not negative
    vec3 diffuseLight = max(dot(norm, lightDirection), 0.0) * lightColor
        * diffuseStrength;

    // special highlight -------------------------------------------------
    vec3 viewDir = normalize(viewPos - FragPosition);
    vec3 reflectDir = reflect(-lightDirection, norm);
    float specularEffect = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specularLight = specularEffect * specularStrength * lightColor;
    
    // final color -------------------------------------------------------
    FragColor = vec4(
        (diffuseLight + ambientLight + specularLight) * color, 1.0);
}
