#version 450 core

in vec3 fragmentColour;
in vec2 fragmentTextureCoord;

out vec4 colour;

uniform sampler2D tex1;

void main(void)
{
	colour = vec4(fragmentColour, 1.0) * texture(tex1, (fragmentTextureCoord));
}