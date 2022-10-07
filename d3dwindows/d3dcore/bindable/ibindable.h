#pragma once
#include "global.h"

class Graphics;
struct ID3D11Device;
struct ID3D11DeviceContext;

class IBindable 
{
public:
    IBindable(Graphics& gfx);
    virtual ~IBindable();

    virtual void bind() = 0;

    ID3D11Device* device();
    ID3D11DeviceContext* deviceContext();

private:
    DECLARE_PRIVATE(IBindable)
};

