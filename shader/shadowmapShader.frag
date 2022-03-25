#version 330 core

layout(location = 0) out vec4 oMoment;

// https://github.com/TheRealMJP/Shadows/blob/master/Shadows/MSM.hlsl
vec4 getOptimizedMoments(in float depth) {
    float square = depth * depth;
    vec4 moments = vec4(depth, square, square * depth, square * square);
    vec4 optimized =
        mat4(-2.0722464, 13.794885723, 0.10587770, 9.792406211, 32.2370377,
             -59.468397570, -1.907746631, -33.765211055, -68.57107459,
             82.035975033, 9.349655510, 47.945609660, 39.370327413,
             -35.36490325, -6.654349074, -23.972804816) *
        moments;
    optimized[0] += 0.03595588480;
    return optimized;
}

void main() {
    float depth = gl_FragCoord.z;
    oMoment = getOptimizedMoments(depth);
}