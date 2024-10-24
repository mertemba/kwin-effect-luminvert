#version 140

#include "colormanagement.glsl"
#include "saturation.glsl"

uniform sampler2D sampler;
uniform vec4 modulation;

in vec2 texcoord0;

out vec4 fragColor;

// RGB to YCbCr, ranges [0, 1]
vec3 rgb_to_ycbcr(vec3 rgb) {
    float y = 0.299 * rgb.r + 0.587 * rgb.g + 0.114 * rgb.b;
    float cb = (rgb.b - y) * 0.565;
    float cr = (rgb.r - y) * 0.713;

    return vec3(y, cb, cr);
}

// YCbCr to RGB
vec3 ycbcr_to_rgb(vec3 yuv) {
    return vec3(
        yuv.x + 1.403 * yuv.z,
        yuv.x - 0.344 * yuv.y - 0.714 * yuv.z,
        yuv.x + 1.770 * yuv.y
    );
}

void main()
{
    vec4 tex = texture(sampler, texcoord0);
    tex = sourceEncodingToNitsInDestinationColorspace(tex);
    tex = adjustSaturation(tex);

    // to preserve perceptual contrast, apply the inversion in gamma 2.2 space
    tex = nitsToEncoding(tex, gamma22_EOTF, 0.0, destinationReferenceLuminance);
    tex.rgb /= max(0.001, tex.a);

    // invert luminance in YCbCr
    vec3 ycbcr = rgb_to_ycbcr(vec3(tex));
    ycbcr.x = 1.0 - ycbcr.x;
    tex = vec4(ycbcr_to_rgb(ycbcr), tex.a);

    tex *= modulation;
    tex.rgb *= tex.a;
    tex = encodingToNits(tex, gamma22_EOTF, 0.0, destinationReferenceLuminance);

    fragColor = nitsToDestinationEncoding(tex);
}
