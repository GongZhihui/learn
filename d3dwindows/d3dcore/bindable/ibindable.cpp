#include "ibindable.h"
#include "graphics.h"

class IBindable::Private
{
public:
    Private(Graphics& gfx, IBindable::Type typ)
        : graphics{ gfx }
        , type{ typ }
    {
    }

    
public:
    Graphics& graphics;
    IBindable::Type type;
};

IBindable::IBindable(Graphics& gfx, Type type)
    : MAKE_PRIVATRE(IBindable, gfx, type)
{
}

IBindable::~IBindable() {}

IBindable::Type IBindable::type() const
{
    return p_->type;
}

ID3D11Device* IBindable::device()
{
    return p_->graphics.device();
}

ID3D11DeviceContext* IBindable::deviceContext()
{
    return p_->graphics.deviceContext();
}

