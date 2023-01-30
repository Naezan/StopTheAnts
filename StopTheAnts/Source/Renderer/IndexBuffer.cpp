#include "../pch.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(ID3D11Device* dev, ID3D11DeviceContext* devcon, unsigned int* indices, int indexCount)
{
	// Create indices.
	//unsigned int indices[] = { 0, 1, 2 };
	D3D11_BUFFER_DESC m_bufferDesc; // 버퍼를 설정하는 구조체
	m_bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_bufferDesc.ByteWidth = sizeof(unsigned int) * indexCount;
	m_bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_bufferDesc.CPUAccessFlags = 0;
	m_bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA m_InitData;
	m_InitData.pSysMem = indices;
	m_InitData.SysMemPitch = 0;
	m_InitData.SysMemSlicePitch = 0;

	auto hr = dev->CreateBuffer(&m_bufferDesc, &m_InitData, &m_pIndexBuffer);
	
	assert(SUCCEEDED(hr));

	devcon->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
}

IndexBuffer::~IndexBuffer()
{
	// 인덱스 버퍼를 해제합니다.
	if (m_pIndexBuffer)
	{
		m_pIndexBuffer->Release();
		m_pIndexBuffer = 0;
	}
}
