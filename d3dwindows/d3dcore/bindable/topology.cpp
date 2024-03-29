#include "topology.h"

class Topology::Private
{
public:
    Private(D3D_PRIMITIVE_TOPOLOGY type)
        : typ{ type }
    {
    }

public:
    D3D_PRIMITIVE_TOPOLOGY typ;
};

Topology::Topology(Graphics& gfx, D3D_PRIMITIVE_TOPOLOGY type)
    : IBindable{ gfx, IBindable::Type::Topology }
    , MAKE_PRIVATRE(Topology, type)
{
}

Topology::~Topology()
{
}

void Topology::bind()
{
    graphics().deviceContext()->IASetPrimitiveTopology(p_->typ);
}
