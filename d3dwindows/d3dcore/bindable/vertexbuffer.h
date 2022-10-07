#pragma once
#include <d3d11.h>
#include <vector>

#include "global.h"
#include "ibindable.h"
#include "d3ddef.h"

class VertexBuffer : public IBindable 
{
public:
    template<class T>
    VertexBuffer(Graphics& gfx, const std::vector<T>& vertices);
    ~VertexBuffer() = default;

    void bind() override;

private:
    std::uint32_t stride;
    ComPtr<ID3D11Buffer> vertexBuffer;
};

template<class T>
inline VertexBuffer::VertexBuffer(Graphics& gfx, const std::vector<T>& vertices)
    : IBindable{ gfx }
    , stride{ sizeof(T) }
{
    D3D11_BUFFER_DESC bd = {};
    bd.ByteWidth = sizeof(T) * vertices.size();
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    bd.StructureByteStride = sizeof(T);

    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = vertices.data();
    HR(device()->CreateBuffer(&bd, &sd, vertexBuffer.ReleaseAndGetAddressOf()));
}
