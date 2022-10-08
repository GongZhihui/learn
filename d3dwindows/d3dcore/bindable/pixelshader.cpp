#include <d3d11.h>
#include <d3dcompiler.h>

#include "d3ddef.h"
#include "pixelshader.h"
#include "errtrace.h"

class PixelShader::Private 
{
public:
    Private() = default;
    ~Private() = default;

public:
    ComPtr<ID3D11PixelShader> pixelShader;
};

PixelShader::PixelShader(Graphics& gfx, const std::wstring& fileName)
    : IShader{ gfx, IBindable::Type::PixelShader, fileName }
    , MAKE_PRIVATRE(PixelShader)
{
    HR(graphics().device()->CreatePixelShader(bytecode()->GetBufferPointer(), bytecode()->GetBufferSize(),
        nullptr, p_->pixelShader.ReleaseAndGetAddressOf()));
}

PixelShader::~PixelShader()
{
}

void PixelShader::bind()
{
    // bind ¶¥µã×ÅÉ«Æ÷
    graphics().deviceContext()->PSSetShader(p_->pixelShader.Get(), nullptr, 0);
}
