#include "../pch.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(ID3D11Device* dev, VERTEX* vertices, int verticeCount)
{
	D3D11_BUFFER_DESC m_bufferDesc; // 버퍼를 설정하는 구조체

	m_bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_bufferDesc.ByteWidth = sizeof(VERTEX) * verticeCount;
	m_bufferDesc.CPUAccessFlags = 0;
	m_bufferDesc.MiscFlags = 0;
	m_bufferDesc.StructureByteStride = 0;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA m_InitData;
	m_InitData.pSysMem = vertices;
	m_InitData.SysMemPitch = 0;
	m_InitData.SysMemSlicePitch = 0;

	auto hr = dev->CreateBuffer(&m_bufferDesc, &m_InitData, &m_pVertexBuffer);

	assert(SUCCEEDED(hr));
}

VertexBuffer::~VertexBuffer()
{
	// 버텍스 버퍼를 해제합니다.
	if (m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = 0;
	}
}
