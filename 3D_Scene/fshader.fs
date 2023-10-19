#version 330 core
in vec4 ourColor;
in vec2 fragTexCoor;

out vec4 FragColor;

uniform sampler2D ourTexture;
uniform sampler2D secondTexture;

void main()
{
    //FragColor = vec4(ourColor);
    FragColor = mix(texture(ourTexture, fragTexCoor), texture(secondTexture, fragTexCoor), 0.2);
 }