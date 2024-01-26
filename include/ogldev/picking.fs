#version 330 core
uniform uint gObjectIndex;
uniform uint gDRawIndex;

out  uvec3 FragColor;

void main()
{
FragColor= uvec3(gObjectIndex, gDRawIndex, gl_PrimitiveID);
}