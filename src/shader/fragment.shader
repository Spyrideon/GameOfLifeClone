#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D Texture;

void main()
{
    float r = texture(Texture, TexCoord).r;
    FragColor = vec4(vec3(r * 255.0), 1.0);
}