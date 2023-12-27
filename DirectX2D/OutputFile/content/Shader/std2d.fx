#ifndef _STD2D
#define _STD2D

// 4096 byte ����
cbuffer TRANSFORM : register(b0)
{
    // ������� ���� ����ü�� �׻� �����ϰ� ���־�� �Ѵ�.
    row_major float4x4 g_matWorld;
}

struct VS_IN // ���ؽ� ���̴��� �˷��� ����
{
    float4 vColor   : COLOR;
    float3 vPos     : POSITION;
    float2 vUV      : TEXCOORD;
};

struct VS_OUT // ���ؽ� ���̴��� �ȼ� ���̴����� �˷��� ����
{
    float4 vPosition    : SV_Position;
    float4 vColor       : COLOR;
    float2 vUV          : TEXCOORD;
};


// Vertex Shader
VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWorld); // ��� ��
    
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
    return output;
}

// Pixel Shader 
float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    // ������ ���� ������ �״�� ��ȯ.
    return _in.vColor;
}


#endif