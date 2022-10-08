#pragma once

#include "global.h"
#include "ishader.h"

class VertexShader : public IShader 
{
public:
    VertexShader(Graphics& gfx, const std::wstring& fileName);
    ~VertexShader();

    void bind() override;

private:
    DECLARE_PRIVATE(VertexShader)
};

DEFINE_MAKE_UNIQUE_PTR(VertexShader)

