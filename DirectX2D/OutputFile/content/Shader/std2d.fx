#ifndef _STD2D
#define _STD2D

// 4096 byte ����
// ������� ���� ����ü�� �׻� �����ϰ� ���־�� �Ѵ�.
cbuffer TRANSFORM : register(b0)
{
    row_major matrix g_matWorld;  // ���� ���
    row_major matrix g_matView; // -> ��(ī�޶�) ���
    row_major matrix g_matProj; // -> ���� ���
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
    
    // ����(��) ��ǥ�� -> ���� -> �� -> ���� ��ǥ��� ���������� ��ȯ
    float4 vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    float4 vViewPos = mul(vWorldPos, g_matView);
    float4 vProjPos = mul(vWorldPos, g_matProj);
            
    output.vPosition = vProjPos;
    
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