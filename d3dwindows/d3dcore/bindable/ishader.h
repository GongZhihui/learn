#pragma once

#include <string>
#include "global.h"
#include "ibindable.h"

class IShader : public IBindable 
{
public:
    IShader(Graphics& gfx, IBindable::Type type, const std::wstring& fileName);
    ~IShader();

    ID3DBlob* bytecode();

private:
    DECLARE_PRIVATE(IShader)
};

