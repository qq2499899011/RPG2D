#version 460 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D sprite;
uniform vec3 spriteColor;
uniform vec4 subTextureCoords; // x0, y0, x1, y1

void main()
{
    // 将传入的纹理坐标映射到子纹理范围
    vec2 subTexCoords = vec2(
        mix(subTextureCoords.x, subTextureCoords.z, TexCoords.x),
        mix(subTextureCoords.y, subTextureCoords.w, TexCoords.y)
    );
    vec4 texColor = texture(sprite, subTexCoords);
    if(texColor.a < 0.1)
        discard;
    color = vec4(spriteColor, 1.0) * texColor;
}