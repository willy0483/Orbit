#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aNormals;

out vec3 color;
out vec2 texCoord;
out vec3 normals;
out vec3 currentPos;

uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
    currentPos = vec3(model * vec4(aPos, 1.0f));
    gl_Position = camMatrix * vec4(currentPos, 1.0f);
    color = aColor;
    texCoord = aTexCoord;
    normals = aNormals;
}
