#pragma once
#include <Windows.h>
#include "global.h"

struct ID3D11Device;
struct ID3D11DeviceContext;

class Graphics 
{
public:
    using  Pointer = std::unique_ptr<Graphics>;

    Graphics(HWND hwnd);
    ~Graphics();

    void render();
    ID3D11Device* device();
    ID3D11DeviceContext* deviceContext();

private:
    DECLARE_PRIVATE(Graphics)
    DISABLE_COPY(Graphics)
};

DEFINE_MAKE_UNIQUE_PTR(Graphics)