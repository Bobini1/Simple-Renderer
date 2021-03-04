#version 330 core
out vec4 FragColor;
uniform sampler2DArray textureArray;
flat in float Layer;
in vec4 Color;
in vec2 TexCoords;
void main()
{
    FragColor = texture(textureArray, vec3(TexCoords, Layer)) * Color;
}
