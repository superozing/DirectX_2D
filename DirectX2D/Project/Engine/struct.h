#pragma once

// Vertex(정점)
struct Vtx
{
	Vec3 vPos;		 // 정점의 좌표	
	Vec4 vColor;	 // 정점의 색상 정보
	Vec2 vUV;		 // UV 좌표계 || Texture Coodinate
};


// 상수 버퍼 대응 구조체 (오프셋과 비슷한 개념)
struct tTransform 
{
	Vec4	vWorldPos;
	Vec4    vWorldScale;
};