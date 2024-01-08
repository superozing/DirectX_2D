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
	Matrix	matWorld;
	Matrix	matWorldInv;

	Matrix	matView;
	Matrix	matViewInv;

	Matrix	matProj;
	Matrix	matProjInv;

	Matrix	matWV;		// ���� ��� * �� ���
	Matrix	matWVP;		// ���� �� ��� * ���� ���
};

// extern.cpp�� ����
extern tTransform g_Transform;