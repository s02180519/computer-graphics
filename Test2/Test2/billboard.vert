#version 330 core

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 VertexTexture;

uniform mat4 PVM;
uniform vec3 cameraRight;
uniform vec3 cameraUp;
uniform vec3 billboardPos;
uniform float size;

out vec2 TexCoords;

void main() {
    TexCoords = VertexTexture;
    gl_Position = PVM * vec4(billboardPos + size * (cameraRight * VertexPosition.x + cameraUp * VertexPosition.y),1);
}