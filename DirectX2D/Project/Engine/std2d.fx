#ifndef _STD2D
#define _STD2D

// 4096 byte 제한
cbuffer TRANSFORM : register(b0)
{
    // 상수버퍼 대응 구조체와 항상 대응하게 해주어야 한다.
    row_major float4x4 g_matWorld;
}

struct VS_IN // 버텍스 쉐이더에 알려줄 정보
{
    float4 vColor   : COLOR;
    float3 vPos     : POSITION;
    float2 vUV      : TEXCOORD;
};

struct VS_OUT // 버텍스 쉐이더가 픽셀 쉐이더에게 알려줄 정보
{
    float4 vPosition    : SV_Position;
    float4 vColor       : COLOR;
    float2 vUV          : TEXCOORD;
};


// Vertex Shader
VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWorld); // 행렬 곱
    
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
    return output;
}

// Pixel Shader 
float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    // 정점의 색상 정보를 그대로 반환.
    return _in.vColor;
}


#endif