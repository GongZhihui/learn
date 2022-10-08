#pragma once
#include <Windows.h>
#include <DirectXMath.h>
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
    void clearBuffer(float red, float green, float blue);

    // Õ∂”∞æÿ’Û
    void setProjection(DirectX::FXMMATRIX projection);
    DirectX::XMMATRIX projection();

    ID3D11Device* device();
    ID3D11DeviceContext* deviceContext();

private:
    void drawIndexed(std::size_t count);

private:
    friend class IDrawable;
    DECLARE_PRIVATE(Graphics)
    DISABLE_COPY(Graphics)
};

DEFINE_MAKE_UNIQUE_PTR(Graphics)