#pragma once

// Vertex(정점)
struct Vtx
{
	Vec3 vPos;		 // 정점의 좌표	
	Vec4 vColor;	 // 정점의 색상 정보
	Vec2 vUV;		 // UV 좌표계 || Texture Coodinate
};


// 상수 버퍼 대응 구조체
struct tTransform 
{
	Matrix	matWorld;
	Matrix	matWorldInv;

	Matrix	matView;
	Matrix	matViewInv;

	Matrix	matProj;
	Matrix	matProjInv;

	Matrix	matWV;		// 월드 행렬 * 뷰 행렬
	Matrix	matWVP;		// 월드 뷰 행렬 * 투영 행렬
};

// extern.cpp에 정의
extern tTransform g_Transform;