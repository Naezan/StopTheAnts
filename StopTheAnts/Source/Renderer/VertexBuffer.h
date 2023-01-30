#pragma once

struct VERTEX { DirectX::XMFLOAT3 position; DirectX::XMFLOAT4 color; };

class VertexBuffer
{
public:
	VertexBuffer(ID3D11Device* dev, VERTEX* vertices, int verticeCount);
	~VertexBuffer();

	inline ID3D11Buffer* GetBuffer() { return m_pVertexBuffer; }
	inline ID3D11Buffer* const* GetBufferPtr() { return &m_pVertexBuffer; }

private:
	ID3D11Buffer* m_pVertexBuffer; // 정점 버퍼에 대한 포인터 
};
