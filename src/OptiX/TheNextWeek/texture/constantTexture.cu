
#include "texture.cuh"

rtDeclareVariable(float3, color, , );

RT_CALLABLE_PROGRAM float3 constantTexture(float u, float v, float3 p) {
    return color;
}
