#pragma once
#include <d3d11.h>

#include "global.h"
#include "ibindable.h"
#include "d3ddef.h"

template<class T, IBindable::Type typ>
class ConstantBuffer : public IBindable 
{
public:
    ConstantBuffer(Graphics& gfx);
    ConstantBuffer(Graphics& gfx, const T& constants);

    void update(const T& constants);

protected:
    ComPtr<ID3D11Buffer> constantBuffer;
};

template<class T>
class PixelConstantBuffer : public ConstantBuffer<T, IBindable::Type::PixelContant>
{
public:
    using ConstantBuffer<T, IBindable::Type::PixelContant>::ConstantBuffer;
    void bind() override;
};

template<class T>
class VertexConstantBuffer : public ConstantBuffer<T, IBindable::Type::VertexContant>
{
public:
    using ConstantBuffer<T, IBindable::Type::VertexContant>::ConstantBuffer;
    void bind() override;
};

template<class T, IBindable::Type typ>
inline ConstantBuffer<T, typ>::ConstantBuffer(Graphics& gfx)
    : IBindable{ gfx, typ }
{
    D3D11_BUFFER_DESC cbd = {};
    cbd.ByteWidth = sizeof(T);
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbd.MiscFlags = 0;
    cbd.StructureByteStride = 0;

    HR(device()->CreateBuffer(&cbd, nullptr, constantBuffer.ReleaseAndGetAddressOf()));
}

template<class T, IBindable::Type typ>
inline ConstantBuffer<T, typ>::ConstantBuffer(Graphics& gfx, const T& constants)
    : IBindable{ gfx, typ }
{
    D3D11_BUFFER_DESC cbd = {};
    cbd.ByteWidth = sizeof(constants);
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbd.MiscFlags = 0;
    cbd.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA csd = {};
    csd.pSysMem = &constants;
    HR(device()->CreateBuffer(&cbd, &csd, constantBuffer.ReleaseAndGetAddressOf()));
}

template<class T, IBindable::Type typ>
inline void ConstantBuffer<T, typ>::update(const T& constants)
{
    D3D11_MAPPED_SUBRESOURCE ms;
    HR(deviceContext()->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms));
    memcpy(ms.pData, &constants, sizeof(constants));
    deviceContext()->Unmap(constantBuffer.Get(), 0);
}

template<class T>
inline void PixelConstantBuffer<T>::bind()
{
    deviceContext()->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
}

template<class T>
inline void VertexConstantBuffer<T>::bind()
{
    deviceContext()->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
}
