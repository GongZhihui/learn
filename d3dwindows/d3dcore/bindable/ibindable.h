#pragma once
#include "global.h"

class Graphics;
struct ID3D11Device;
struct ID3D11DeviceContext;

class IBindable 
{
public:
    DEFINE_POINTER(IBindable)
    enum class Type 
    {
        PixelContant,
        VertexContant,
        IndexBuffer,
        InputLayout,
        Topology,
        VertexBuffer,
        TransformCBuffer
    };

    IBindable(Graphics& gfx, IBindable::Type type);
    virtual ~IBindable();

    virtual void bind() = 0;

    IBindable::Type type() const;
    ID3D11Device* device();
    ID3D11DeviceContext* deviceContext();

private:
    DECLARE_PRIVATE(IBindable)
};

