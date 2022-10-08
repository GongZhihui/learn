#pragma once

#include "drawable/idrawable.h"

class Triangle : public IDrawable 
{
public:
    DEFINE_POINTER(Triangle)

    Triangle(Graphics& gfx);
    ~Triangle() = default;

    DirectX::XMMATRIX transform() const override;
    void update(float dt) override;

private:
    DECLARE_PRIVATE(Triangle)
};

DEFINE_MAKE_UNIQUE_PTR(Triangle);
