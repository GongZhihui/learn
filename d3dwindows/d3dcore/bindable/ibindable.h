#pragma once
#include "global.h"
#include "graphics.h"

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
        TransformCBuffer,
        PixelShader,
        VertexShader
    };

    IBindable(Graphics& gfx, IBindable::Type type);
    virtual ~IBindable();

    virtual void bind() = 0;

    IBindable::Type type() const;
    Graphics& graphics();

private:
    DECLARE_PRIVATE(IBindable)
};

