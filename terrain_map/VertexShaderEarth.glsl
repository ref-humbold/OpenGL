#version 330 core

uniform mat4 worldToCameraMat;

layout(location=0) in vec2 vertexPos;

out vec4 fragmentColor;

const float RADIUS = 6400000.0;

void main()
{
    float lng = radians(vertexPos.x);
    float lat = radians(vertexPos.y);

    float mapPosX = RADIUS*cos(lat)*sin(lng);
    float mapPosY = RADIUS*sin(lat);
    float mapPosZ = RADIUS*cos(lat)*cos(lng);

    vec4 vPos = vec4(mapPosX, mapPosY, mapPosZ, 1.0);
    vec4 vPosClip = worldToCameraMat*vPos;

    if(vertexPos.y == 90.0)
    {
        fragmentColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
    else if(vertexPos.y == -90.0)
    {
        fragmentColor = vec4(0.0, 0.0, 1.0, 1.0);
    }
    else
    {
        fragmentColor = vec4(1.0, 1.0, 1.0, 1.0);
    }

    gl_Position = vPosClip;
}