#pragma once

// Vertex(����)
struct Vtx
{
	Vec3 vPos;		 // ������ ��ǥ	
	Vec4 vColor;	 // ������ ���� ����
	Vec2 vUV;		 // UV ��ǥ�� || Texture Coodinate
};


// ��� ���� ���� ����ü (�����°� ����� ����)
struct tTransform 
{
	Vec4	vWorldPos;
	Vec4    vWorldScale;
};