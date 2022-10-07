#include "ibindable.h"
#include "graphics.h"

class IBindable::Private
{
public:
    Private(Graphics& gfx) 
        : graphics{ gfx }
    {
    }

    
public:
    Graphics& graphics;
};

IBindable::IBindable(Graphics& gfx)
    : MAKE_PRIVATRE(IBindable, gfx)
{
}

IBindable::~IBindable() {}

ID3D11Device* IBindable::device()
{
    return p_->graphics.device();
}

ID3D11DeviceContext* IBindable::deviceContext()
{
    return p_->graphics.deviceContext();
}

