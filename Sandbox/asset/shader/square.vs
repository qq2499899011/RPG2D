#version 460 core
layout (location = 0) in vec2 pos;

uniform mat4 model;
uniform mat4 view_projection;

void main()
{
    gl_Position = view_projection * model * vec4(pos, 0.0, 1.0);
}