#include <d3d11.h>
#include <d3dcompiler.h>

#include "ishader.h"
#include "d3ddef.h"

class IShader::Private 
{
public:
    Private() = default;
    ~Private() = default;

public:
    ComPtr<ID3DBlob> blob;
};

IShader::IShader(Graphics& gfx, 
    IBindable::Type type, 
    const std::wstring& fileName)
    : IBindable{ gfx, type }
    , MAKE_PRIVATRE(IShader)
{
    HR(D3DReadFileToBlob(fileName.c_str(), p_->blob.ReleaseAndGetAddressOf()));
}

IShader::~IShader(){}

ID3DBlob* IShader::bytecode()
{
    return p_->blob.Get();
}
