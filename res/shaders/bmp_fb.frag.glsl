#version 450 core

layout(std430, binding = 0) readonly buffer ssbo0 {
    uint bitmap[];
};
flat in uvec2 vBitmapCoord;
in vec2 vTexCoord;

uniform uvec2 u_BitmapDim;
uniform uint u_ColorFormatFlag;

out vec4 fColor;

// Color Packing format
const uint MASK_RGBA = uint(0x1);
const uint MASK_SINGLE_BIT = uint(0x2);

// RGBA
const uint MASK_R = uint(0xFF000000);
const uint MASK_G = uint(0x00FF0000);
const uint MASK_B = uint(0x0000FF00);
const uint MASK_A = uint(0x000000FF);

void main()
{
    uint selRGBA = (u_ColorFormatFlag&MASK_RGBA) >> 0;
    uint selSINGLE_BIT = (u_ColorFormatFlag&MASK_SINGLE_BIT) >> 1;
    uint pixelInd = vBitmapCoord.y*u_BitmapDim.x + vBitmapCoord.x;

    uint colorUintInd = pixelInd*selRGBA;
    colorUintInd += uint(floor(pixelInd/32))*selSINGLE_BIT;
    uint colorBitInd = 0*selRGBA;
    colorBitInd += (pixelInd%32)*selSINGLE_BIT;

    // Unpack rgba
    float r = float((bitmap[colorUintInd]&MASK_R) >> 24) / (0xFF-1);
    float g = float((bitmap[colorUintInd]&MASK_G) >> 16) / (0xFF-1);
    float b = float((bitmap[colorUintInd]&MASK_B) >>  8) / (0xFF-1);
    float a = float((bitmap[colorUintInd]&MASK_A) >>  0) / (0xFF-1);
    // Unpack singleBit
    float singleBit = float((bitmap[colorUintInd] >> colorBitInd)&uint(1));

    vec4 color = vec4(r, g, b, a)*selRGBA;
    color += vec4(vec3(singleBit), 1.0)*selSINGLE_BIT;

    fColor = color;
}
