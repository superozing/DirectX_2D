#include "pch.h"
#include "CConstBuffer.h"

#include "CDevice.h"

CConstBuffer::CConstBuffer()
    : m_Desc{} // Create���� �ʱ�ȭ
	, m_ElementCount(0)
	, m_ElementSize(0)
{
}

CConstBuffer::~CConstBuffer()
{
}

int CConstBuffer::Create(UINT _ElementSize, UINT _ElementCount)
{
	m_ElementSize = _ElementSize;
	m_ElementCount = _ElementCount;

	// ��� ����(Constant Buffer) ����
	m_Desc.ByteWidth = m_ElementSize * m_ElementCount;
	m_Desc.StructureByteStride = m_ElementSize;
	m_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// CPU�� ������ ���⸦ ��� (��� ���� �ʾ�� ����)
	m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_Desc.Usage = D3D11_USAGE_DYNAMIC;

	// ��� ���� ����
	IF_FAILED(DEVICE->CreateBuffer(&m_Desc, nullptr, m_CB.GetAddressOf()), L"��� ���� ����");

	return S_OK;
}

void CConstBuffer::SetData(void* _Src, UINT _ElementCount)
{
	// �⺻ ���ڰ� ���� ��� Create �� ������ ���� ������ ���
	if (0 == _ElementCount)
		_ElementCount = m_ElementCount;

	// �ý��� �޸𸮿��� GPU �޸𸮷� Map UnMap �ϱ� = ������ ����
	D3D11_MAPPED_SUBRESOURCE tSub{};

	// ��� ���۷κ��� �޸� ��������
	CONTEXT->Map(m_CB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);

	// �ý��� �޸�(Src) -> GPU �޸�(tSub)�� Memcpy
	memcpy(tSub.pData, _Src, _ElementCount);

	CONTEXT->Unmap(m_CB.Get(), 0);
}

void CConstBuffer::UpdateData(UINT _RegisterNum)
{
	// ��� ���� ����

	CONTEXT->VSSetConstantBuffers(_RegisterNum, 1, m_CB.GetAddressOf());
	CONTEXT->HSSetConstantBuffers(_RegisterNum, 1, m_CB.GetAddressOf());
	CONTEXT->DSSetConstantBuffers(_RegisterNum, 1, m_CB.GetAddressOf());
	CONTEXT->GSSetConstantBuffers(_RegisterNum, 1, m_CB.GetAddressOf());
	CONTEXT->PSSetConstantBuffers(_RegisterNum, 1, m_CB.GetAddressOf());
}

