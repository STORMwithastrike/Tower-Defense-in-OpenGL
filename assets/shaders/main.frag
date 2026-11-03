#version 330 core
in vec4 vColor;
in vec2 vTexCoord;

out vec4 FragColor;

uniform vec4 u_Tint;              // Global RGBA tint (default: 1, 1, 1, 1)
uniform float u_Highlight;        // 0.0 to 1.0 yellow highlight for hover / selection
uniform float u_DamageIntensity;  // 0.0 to 1.0 red damage tint for attacked units
uniform float u_Grayscale;        // 0.0 to 1.0 grayscale conversion for locked cards
uniform int u_UseTexture;         // 1 if texturing is enabled, 0 for vertex colors
uniform sampler2D u_Texture;

void main()
{
    vec4 baseColor = vColor;
    if (u_UseTexture == 1) {
        baseColor = texture(u_Texture, vTexCoord) * vColor;
    }

    // 1. Apply Grayscale (standard luminance weights) if requested
    if (u_Grayscale > 0.0) {
        float gray = dot(baseColor.rgb, vec3(0.299, 0.587, 0.114));
        baseColor.rgb = mix(baseColor.rgb, vec3(gray), u_Grayscale);
    }

    // 2. Apply Yellow Highlight (for lane hover / selection)
    if (u_Highlight > 0.0) {
        vec3 yellow = vec3(1.0, 0.92, 0.25);
        baseColor.rgb = mix(baseColor.rgb, yellow, u_Highlight * 0.45);
    }

    // 3. Apply Red Damage Tint (for wounded or attacked units)
    if (u_DamageIntensity > 0.0) {
        vec3 redDamage = vec3(0.95, 0.15, 0.15);
        baseColor.rgb = mix(baseColor.rgb, redDamage, clamp(u_DamageIntensity, 0.0, 0.85));
    }

    // 4. Combine with uniform tint (supports direct tint and red damage alpha blend)
    if (u_Tint.g == 0.0 && u_Tint.b == 0.0 && u_Tint.r == 1.0) {
        FragColor = mix(baseColor, vec4(1.0, 0.0, 0.0, 1.0), u_Tint.a);
    } else {
        FragColor = baseColor * u_Tint;
    }
}

