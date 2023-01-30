#pragma once

class IndexBuffer
{
public:
	IndexBuffer(ID3D11Device* dev, ID3D11DeviceContext* devcon, unsigned int* indices, int indexCount);
	~IndexBuffer();

	inline ID3D11Buffer* GetBuffer() { return m_pIndexBuffer; }

private:
	ID3D11Buffer* m_pIndexBuffer; // 정점 버퍼에 대한 포인터 
};
