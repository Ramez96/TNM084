#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;

out vec3 exNormal;
out vec2 texCoord;
out float myHeight;

uniform mat4 modelviewMatrix;
uniform mat4 projectionMatrix;

void main(void)
{
	texCoord = inTexCoord;
	exNormal = inNormal;
    myHeight = inPosition.y;
	gl_Position = projectionMatrix * modelviewMatrix * vec4(inPosition, 1.0);
}
