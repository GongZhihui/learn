#pragma once
#include <d3d11.h>

#include "global.h"
#include "ibindable.h"

class Topology : public IBindable 
{
public:
    Topology(Graphics& gfx, D3D_PRIMITIVE_TOPOLOGY type);
    ~Topology();

    void bind() override;

private:
    DECLARE_PRIVATE(Topology)
};
