#include "inputlayout.h"
#include "d3ddef.h"

class InputLayout::Private 
{
public:
    Private() = default;

public:
    ComPtr<ID3D11InputLayout> inputLayout;
};

InputLayout::InputLayout(Graphics& gfx, 
    const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, 
    ID3DBlob* shaderBytecode)
    : IBindable{ gfx, IBindable::Type::InputLayout }
    , MAKE_PRIVATRE(InputLayout)
{
    HR(graphics().device()->CreateInputLayout(layout.data(), std::size(layout), shaderBytecode->GetBufferPointer(),
        shaderBytecode->GetBufferSize(), p_->inputLayout.ReleaseAndGetAddressOf()));
}

InputLayout::~InputLayout(){}

void InputLayout::bind()
{
    graphics().deviceContext()->IASetInputLayout(p_->inputLayout.Get());
}
