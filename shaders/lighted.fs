#version 330 core
out vec4 FragColor;
 
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPosition;

uniform sampler2D diffuseTexture;
uniform sampler2D specularMap;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;
uniform int shininess;

void main()
{
    // material / texture color ------------------------------------------
    vec3 color = vec3(texture(diffuseTexture, TexCoord));
    
    // ambient light -----------------------------------------------------
    vec3 ambientLight = ambientStrength * color * lightColor;
    
    // diffuse light -----------------------------------------------------
    // Normal might not actually be... normal
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(lightPos - FragPosition);
    
    // the "max" is there because if the vertex is facing away from
    // the light, it should just have 0 light, not negative
    vec3 diffuseLight = max(dot(norm, lightDirection), 0.0) * lightColor
        * diffuseStrength * color;

    // special highlight -------------------------------------------------
    vec3 viewDir = normalize(viewPos - FragPosition);
    vec3 reflectDir = reflect(-lightDirection, norm);
    float specularEffect = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specularLight = specularEffect * specularStrength
        * lightColor * texture(specularMap, TexCoord).rgb;
    
    // final color -------------------------------------------------------
    FragColor = vec4(
        (diffuseLight + ambientLight + specularLight), 1.0);
}
