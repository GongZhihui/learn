#include <DirectXMath.h>

#include "transformcbuffer.h"
#include "constantbuffer.h"
#include "graphics.h"

class TransformCBuffer::Private
{
public:
    Private(Graphics& gfx, const IDrawable& draw)
        : graphics{ gfx }
        , drawable{ draw }
        , vcbuffer{ gfx }
    {
    }

public:
    Graphics& graphics;
    const IDrawable& drawable;
    VertexConstantBuffer<DirectX::XMMATRIX> vcbuffer;
};

TransformCBuffer::TransformCBuffer(Graphics& gfx, const IDrawable& drawable)
    : IBindable{ gfx, IBindable::Type::TransformCBuffer }
    , MAKE_PRIVATRE(TransformCBuffer, gfx, drawable)
{
}

TransformCBuffer::~TransformCBuffer(){}

void TransformCBuffer::bind()
{
    p_->vcbuffer.update(DirectX::XMMatrixTranspose(
        p_->drawable.transform() * p_->graphics.projection()));
    p_->vcbuffer.bind();
}
