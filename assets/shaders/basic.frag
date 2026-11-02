#version 330 core
in vec3 vColor;
out vec4 FragColor;

uniform float u_Highlight;
uniform vec4 u_Tint;

void main()
{
    vec3 highlightColor = mix(vColor, vec3(1.0, 1.0, 0.0), u_Highlight);
    FragColor = vec4(highlightColor, 1.0) * u_Tint;
}
