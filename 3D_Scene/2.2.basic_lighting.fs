#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform vec3 lightPos; // Position of the light source
uniform vec4 lightColor; // Color of the light source

uniform vec3 backupLightPos; //position of second light
uniform vec4 backupLightColor; //color of second light

uniform vec3 viewPos;

uniform sampler2D ourTexture;
uniform sampler2D secondTexture;

void main()
{
    // Ambient light 
    float ambientStrength1 = 0.1;
    vec3 ambient1 = ambientStrength1 * lightColor.rgb;

    float ambientStrength2 = 0.1;
    vec3 ambient2 = ambientStrength2 * backupLightColor.rgb;

    // Diffuse light 
    vec3 norm = normalize(Normal);
    vec3 lightDir1 = normalize(lightPos - FragPos);
    float diff1 = max(dot(norm, lightDir1), 0.0);
    vec3 diffuse1 = diff1 * lightColor.rgb;

    norm = normalize(Normal);
    vec3 lightDir2 = normalize(backupLightPos - FragPos);
    float diff2 = max(dot(norm, lightDir2), 0.0);
    vec3 diffuse2 = diff2 * backupLightColor.rgb;

    // Specular light 
    float specularStrength1 = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir1 = reflect(-lightDir1, norm);
    float spec1 = pow(max(dot(viewDir, reflectDir1), 0.0), 32);
    vec3 specular1 = specularStrength1 * spec1 * lightColor.rgb;

    float specularStrength2 = 0.5;
    viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir2 = reflect(-lightDir2, norm);
    float spec2 = pow(max(dot(viewDir, reflectDir2), 0.0), 32);
    vec3 specular2 = specularStrength2 * spec2 * backupLightColor.rgb;

    vec3 texColor = texture(ourTexture, TexCoord).rgb;
    vec3 texColor2 = texture(secondTexture, TexCoord).rgb;

    // Combine the lighting from both sources
    vec3 result1 = (ambient1 + ambient2) * texColor + (diffuse1 + diffuse2) * texColor + (specular1 + specular2) * texColor;
    vec3 result2 = (ambient1 + ambient2) * texColor2 + (diffuse1 + diffuse2) * texColor2 + (specular1 + specular2) * texColor2;
    vec3 finalResult = result1 + result2;
    FragColor = vec4(finalResult, 1.0);
}