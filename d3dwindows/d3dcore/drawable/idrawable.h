#pragma once

#include <DirectXMath.h>
#include "global.h"
#include "bindable/indexbuffer.h"

class Graphics;
class IBindable;
class IndexBuffer;

class IDrawable 
{
public:
    IDrawable(Graphics& gfx);
    virtual ~IDrawable();

    void draw() const;

    virtual DirectX::XMMATRIX transform() const = 0;
    virtual void update(float dt) = 0;
    
    void addBind(IBindable::upointer bind);
    void addIndexBuffer(IndexBuffer::upointer indexBuffer);

private:
    DECLARE_PRIVATE(IDrawable)
};

