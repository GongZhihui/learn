#include <d3d11.h>
#include <d3dcompiler.h>

#include "d3ddef.h"
#include "vertexshader.h"
#include "errtrace.h"

class VertexShader::Private 
{
public:
    Private() = default;
    ~Private() = default;

public:
    ComPtr<ID3D11VertexShader> vertexShader;
};

VertexShader::VertexShader(Graphics& gfx, const std::wstring& fileName)
    : IShader{ gfx, IBindable::Type::VertexShader, fileName }
    , MAKE_PRIVATRE(VertexShader)
{
    HR(graphics().device()->CreateVertexShader(bytecode()->GetBufferPointer(), bytecode()->GetBufferSize(),
        nullptr, p_->vertexShader.ReleaseAndGetAddressOf()));
}

VertexShader::~VertexShader(){}

void VertexShader::bind()
{
    graphics().deviceContext()->VSSetShader(p_->vertexShader.Get(), nullptr, 0);
}
