#ifndef _STD2D
#define _STD2D

// 4096 byte 제한
// 상수버퍼 대응 구조체와 항상 대응하게 해주어야 한다.
cbuffer TRANSFORM : register(b0)
{
    row_major matrix g_matWorld;        // 월드 행렬
    row_major matrix g_matWorldInv;     // 월드 역행렬
    
    row_major matrix g_matView;         // -> 뷰(카메라) 행렬
    row_major matrix g_matViewInv;      // -> 뷰(카메라) 역행렬
    
    row_major matrix g_matProj;         // -> 투영 행렬
    row_major matrix g_matProjInv;      // -> 역투영 행렬
    
    row_major matrix g_matWV;           // 월드 뷰 행렬
    row_major matrix g_matWVP;          // 월드 뷰 투영 행렬
}


Texture2D g_tex_0 : register(t0);

SamplerState g_sam_0 : register(s0);



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
    
    // 로컬(모델) 좌표를 -> 월드 -> 뷰 -> 투영 좌표계로 순차적으로 변환
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
    // Sample() - 특정 좌표(UV 기준)에 해당하는 색상 값을 가져옴
    float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    
    // 알파가 낮은 값(투명에 가까운 값)을 빨강으로 강제 치환
    //if (vColor.a <= 0.1f)
    //    vColor.rgba = float4(1.f, 0.f, 0.f, 1.f);
    
    
    // 흑백 값 표현 - rgb를 더해서 나온 평균 값으로 해당 픽셀을 칠하면 된다.
    //float Aver = (vColor.r + vColor.g + vColor.b) / 3.f;
    //vColor.rgb = float3(Aver, Aver, Aver);
    //vColor.a = 1.f;
    
    return vColor;
}


#endif