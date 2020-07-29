#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform vec3    camPos;

void main()
{
    gl_Position = vec4((aPos.xy - camPos.xy) * camPos.z, aPos.z, 1.0);
    TexCoord = aTexCoord;
}
##MMMMMMMMMMAAAAAAAAAAAAARRRRRRRRRRRRGGGGGGGGGGGIIIIIIIIIIIINNNNNNNNNNNNN