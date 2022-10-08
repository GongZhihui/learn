#pragma once
#include <d3d11.h>

#include "global.h"
#include "ibindable.h"
#include "drawable/idrawable.h"

class TransformCBuffer : public IBindable 
{
public:
    TransformCBuffer(Graphics& gfx, const IDrawable& drawable);
    ~TransformCBuffer();

    void bind() override;

private:
    DECLARE_PRIVATE(TransformCBuffer)
};
