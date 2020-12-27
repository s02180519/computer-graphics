#version 330 core

in float Transp;
//in vec3 Position;

uniform sampler2D texture1;

layout (location = 0) out vec4 FragColor;

void main() {
	vec4 color = texture(texture1, gl_PointCoord);
	color.a *= Transp;
	FragColor = color;
}