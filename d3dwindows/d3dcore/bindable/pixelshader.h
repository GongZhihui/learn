#pragma once

#include "global.h"
#include "ishader.h"

class PixelShader : public IShader
{
public:
    PixelShader(Graphics& gfx, const std::wstring& fileName);
    ~PixelShader();

    void bind() override;

private:
    DECLARE_PRIVATE(PixelShader)
};

DEFINE_MAKE_UNIQUE_PTR(PixelShader)