#include "pch.h"
#include "CConstBuffer.h"

#include "CDevice.h"

CConstBuffer::CConstBuffer()
    : m_Desc{} // Create에서 초기화
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

	// 상수 버퍼(Constant Buffer) 생성
	m_Desc.ByteWidth = m_ElementSize * m_ElementCount;
	m_Desc.StructureByteStride = m_ElementSize;
	m_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// CPU가 데이터 쓰기를 허용 (상수 버퍼 맵언맵 위해)
	m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_Desc.Usage = D3D11_USAGE_DYNAMIC;

	// 상수 버퍼 생성
	IF_FAILED(DEVICE->CreateBuffer(&m_Desc, nullptr, m_CB.GetAddressOf()), L"상수 버퍼 생성");

	return S_OK;
}

void CConstBuffer::SetData(void* _Src, UINT _ElementCount)
{
	// 기본 인자가 들어올 경우 Create 때 적용한 인자 개수를 사용
	if (0 == _ElementCount)
		_ElementCount = m_ElementCount;

	// 시스템 메모리에서 GPU 메모리로 Map UnMap 하기 = 데이터 복사
	D3D11_MAPPED_SUBRESOURCE tSub{};

	// 상수 버퍼로부터 메모리 가져오기
	CONTEXT->Map(m_CB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);

	// 시스템 메모리(Src) -> GPU 메모리(tSub)로 Memcpy
	memcpy(tSub.pData, _Src, _ElementCount);

	CONTEXT->Unmap(m_CB.Get(), 0);
}

void CConstBuffer::UpdateData(UINT _RegisterNum)
{
	// 상수 버퍼 세팅

	CONTEXT->VSSetConstantBuffers(_RegisterNum, 1, m_CB.GetAddressOf());
	CONTEXT->HSSetConstantBuffers(_RegisterNum, 1, m_CB.GetAddressOf());
	CONTEXT->DSSetConstantBuffers(_RegisterNum, 1, m_CB.GetAddressOf());
	CONTEXT->GSSetConstantBuffers(_RegisterNum, 1, m_CB.GetAddressOf());
	CONTEXT->PSSetConstantBuffers(_RegisterNum, 1, m_CB.GetAddressOf());
}

