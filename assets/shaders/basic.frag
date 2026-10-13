#version 430 core
in vec3 vColor;
out vec4 FragColor;

uniform float u_Highlight;
uniform vec4 u_Tint;

void main()
{
    // Mix the base color with yellow if highlighted (mouse hover)
    vec3 highlightColor = mix(vColor, vec3(1.0, 1.0, 0.0), u_Highlight);
    
    // Apply the tint (for damage effects or default rendering)
    FragColor = vec4(highlightColor, 1.0) * u_Tint;
}