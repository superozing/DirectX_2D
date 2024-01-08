#pragma once

// Vertex(����)
struct Vtx
{
	Vec3 vPos;		 // ������ ��ǥ	
	Vec4 vColor;	 // ������ ���� ����
	Vec2 vUV;		 // UV ��ǥ�� || Texture Coodinate
};


// ��� ���� ���� ����ü
struct tTransform 
{
	Matrix matWorld; // ���� ������ ��� �ϳ��� ���� ����
	Matrix	matView;
	Matrix	matProj;
};

// extern.cpp�� ����
extern tTransform g_Transform;