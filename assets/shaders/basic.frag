#version 330 core
in vec3 vColor;
out vec4 FragColor;

uniform vec4 u_Tint;
uniform float u_Highlight;
uniform vec4 u_HighlightColor;

void main()
{
    vec3 highlightTint = (u_HighlightColor.a > 0.0 || u_HighlightColor.rgb != vec3(0.0)) ? u_HighlightColor.rgb : vec3(1.0, 1.0, 0.0);
    vec3 highlightColor = mix(vColor, highlightTint, u_Highlight);
    FragColor = mix(vec4(highlightColor, 1.0), vec4(1.0, 0.0, 0.0, 1.0), u_Tint.a);
}

