#include <d3d11.h>

#include "d3ddef.h"
#include "indexbuffer.h"
#include "graphics.h"

class IndexBuffer::Private 
{
public:
    Private(std::size_t cnt)
        : count{ cnt }
    {
    }

public:
    std::size_t count;
    ComPtr<ID3D11Buffer> indexBuffer;
};

IndexBuffer::IndexBuffer(Graphics& gfx, const std::vector<std::uint16_t>& indices)
    : IBindable{ gfx, IBindable::Type::IndexBuffer }
    , MAKE_PRIVATRE(IndexBuffer, indices.size())
{
    D3D11_BUFFER_DESC ibd = {};
    ibd.ByteWidth = sizeof(indices);
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0u;
    ibd.MiscFlags = 0u;
    ibd.StructureByteStride = sizeof(std::uint16_t);

    D3D11_SUBRESOURCE_DATA isd = {};
    isd.pSysMem = indices.data();
    HR(device()->CreateBuffer(&ibd, &isd, p_->indexBuffer.ReleaseAndGetAddressOf()));
}

IndexBuffer::~IndexBuffer()
{
}

void IndexBuffer::bind()
{
    deviceContext()->IASetIndexBuffer(p_->indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
}

std::size_t IndexBuffer::count() const
{
    return p_->count;
}
