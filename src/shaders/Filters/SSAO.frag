#version 430

out vec4 fragColor;
in vec2 texCoord;

uniform sampler2D tex;
uniform mat4 invProj;
uniform mat4 projection;

uniform float kernelArray[450];


void main() {
    vec4 c = texelFetch(tex, ivec2(gl_FragCoord), 0).xyzw;

    // calc view ray
    vec2 ndc = texCoord;// * 2.0 - 1.0;
    float aspect = 16.0/9.0;

    float thfov = tan(45 / 2.0); // can do this on the CPU
    vec3 viewRay = vec3(ndc.x * thfov * aspect, ndc.y * thfov, 1.0);
    normalize(viewRay);
    vec4 fragmentWorld = vec4(viewRay * c.w, 1);

    float occlusion = 0.0;
    float uRadius = 1;
    vec3 kernel;
    float rangeCheck;
    for (int i = 0; i < 150; ++i)
    {
        kernel = vec3(kernelArray[i*3],kernelArray[i*3+1],kernelArray[i*3+2]);
        // create samples from world position
        vec3 kernelSample = fragmentWorld.xyz + kernel * uRadius;

        // project samples to screen
        vec4 offset = vec4(kernelSample, 1.0);
        offset = projection * offset;
        offset.xy /= -offset.w;
        //offset.xy = offset.xy * 0.5 + 0.5;
        offset.xy = vec2(offset.x * 1280,
                         offset.y * 720);

        // check if sample passes depth test in screen
        float sampleDepth = texelFetch(tex, ivec2(offset.xy), 0).w;
        rangeCheck = abs(fragmentWorld.z - sampleDepth) < uRadius ? 1.0 : 0.0;
        occlusion += (sampleDepth <= kernelSample.z ? 1.0 : 0.0) * rangeCheck;
        // determine occlusion by fraction of occluded samples
    }
    occlusion -= 75;
    occlusion = 1.0 - (occlusion / (150));


    // Test if projection is correct
//    fragmentWorld = projection * fragmentWorld;
//    fragmentWorld.xy /= -fragmentWorld.w;
//    fragmentWorld.xy = vec2(fragmentWorld.x * 1280,
//                            fragmentWorld.y * 720);
//    fragColor = texelFetch(tex, ivec2(fragmentWorld.xy), 0);


    // apply occlusion to fragColor
    //fragColor = vec4(c.w,c.w,c.w,1);
    fragColor = c * occlusion;
    //fragColor = vec4(occlusion,occlusion,occlusion,1);
}
