#ifndef _STD2D
#define _STD2D

// 4096 byte ����
// ������� ���� ����ü�� �׻� �����ϰ� ���־�� �Ѵ�.
cbuffer TRANSFORM : register(b0)
{
    row_major matrix g_matWorld;        // ���� ���
    row_major matrix g_matWorldInv;     // ���� �����
    
    row_major matrix g_matView;         // -> ��(ī�޶�) ���
    row_major matrix g_matViewInv;      // -> ��(ī�޶�) �����
    
    row_major matrix g_matProj;         // -> ���� ���
    row_major matrix g_matProjInv;      // -> ������ ���
    
    row_major matrix g_matWV;           // ���� �� ���
    row_major matrix g_matWVP;          // ���� �� ���� ���
}


Texture2D g_tex_0 : register(t0);

SamplerState g_sam_0 : register(s0);



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
    float4 vProjPos = mul(vViewPos, g_matProj);
            
    output.vPosition = vProjPos;
    
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
    return output;
}

// Pixel Shader 
float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    // Sample() - Ư�� ��ǥ(UV ����)�� �ش��ϴ� ���� ���� ������
    float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    
    // ���İ� ���� ��(���� ����� ��)�� �������� ���� ġȯ
    //if (vColor.a <= 0.1f)
    //    vColor.rgba = float4(1.f, 0.f, 0.f, 1.f);
    
    
    // ��� �� ǥ�� - rgb�� ���ؼ� ���� ��� ������ �ش� �ȼ��� ĥ�ϸ� �ȴ�.
    //float Aver = (vColor.r + vColor.g + vColor.b) / 3.f;
    //vColor.rgb = float3(Aver, Aver, Aver);
    //vColor.a = 1.f;
    
    return vColor;
}


#endif