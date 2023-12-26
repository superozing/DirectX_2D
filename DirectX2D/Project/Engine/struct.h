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
	Matrix matWorld; // 기존 정보를 행렬 하나로 관리 가능
};