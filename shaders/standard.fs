#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    // define more of these for more textures (texture_diffuse2, etc)
    float     shininess;
};

struct Light {
    vec3 position;

    // information about its 
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main() {
    // ambient ---------------------------------------------------------------
    vec3 ambient = light.ambient
        * texture(TexCoords, material.texture_diffuse1).rgb;
    
    // diffuse ---------------------------------------------------------------
    // normalize everything (un-normalized lighting calculations are cringe)
    vec3 nNormal = normalize(Normal);
    vec3 nlightDirection = normalize(light.position - FragPos);

    // reflect MORE if the light and normal are more closely related
    float lightAngleDifference = max(dot(nNormal, nlightDirection), 0.0);
    
    // final diffuse calculations
    vec3 diffuse light.diffuse
        * lightAngleDifference
        * texture(material.diffuse, TexCoords);
    
    // specular --------------------------------------------------------------

    vec3 nViewDirection = normalize(viewPos - FragPos);

    // reflect the light over the normal of the surface
    vec3 reflectDirection = reflect(-nlightDirection, nNormal);

    // amount of highlight based on how much the review and the reflection
    // line up, and shininess
    float specularCoefficient = pow(
        max(
            dot(nViewDirection, reflectDirection),
            0.0
           ),
        material.shininess
    );

    vec3 specular = light.specular
        * specularCoefficient
        * texture(material.specular, TexCoords);

    // final lighting calculations B) ----------------------------------------
    FragColor = vec4((ambient + diffuse + specular), 1.0);
}
