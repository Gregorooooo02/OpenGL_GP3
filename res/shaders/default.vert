#version 330 core
// Position attribute
layout (location = 0) in vec3 aPos;
// Color attribute
layout (location = 1) in vec3 aColor;
// Texture Coordinate attribute
layout (location = 2) in vec2 aTexCoord;

// Output position to fragment shader
out vec3 position;
// Output color to fragment shader
out vec3 color;
// Output texture coordinate to fragment shader
out vec2 texCoord;

// Imports the camera matrix
uniform mat4 camera;

// Uniform matrix for model, view and projection
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    position = vec3(model * view * projection * vec4(aPos, 1.0));
    color = aColor;
    texCoord = aTexCoord;

    gl_Position = camera * vec4(position, 1.0);
}
