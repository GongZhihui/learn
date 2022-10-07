#pragma once
#include <d3d11.h>
#include <vector>

#include "global.h"
#include "ibindable.h"

class InputLayout : public IBindable 
{
public:
    InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
        ID3DBlob *shaderBytecode);
    ~InputLayout();

    void bind() override;

private:
    DECLARE_PRIVATE(InputLayout)
};

