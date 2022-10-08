#include <vector>

#include "ibindable.h"
#include "graphics.h"
#include "idrawable.h"

class IDrawable::Private
{
public:
    Private(Graphics& gfx) 
        : graphics{ gfx }
    {
    }

    
public:
    Graphics& graphics;
    IndexBuffer* indexBuffer;
    std::vector<IBindable::upointer> binds;
};

IDrawable::IDrawable(Graphics& gfx)
    : MAKE_PRIVATRE(IDrawable, gfx)
{
}

IDrawable::~IDrawable(){}

void IDrawable::draw() const
{
    for (auto& b : p_->binds) 
    {
        b->bind();
    }

    p_->graphics.drawIndexed(p_->indexBuffer->count());
}

void IDrawable::addBind(IBindable::upointer bind)
{
    assert(bind->type() != IBindable::Type::IndexBuffer);
    p_->binds.push_back(std::move(bind));
}

void IDrawable::addIndexBuffer(IndexBuffer::upointer indexBuffer)
{
    assert(!p_->indexBuffer);
    p_->indexBuffer = indexBuffer.get();
    p_->binds.push_back(std::move(indexBuffer));

}
