#include <d3d11.h>
#include <cmath>

#include "triangle.h"
#include "bindable/indexbuffer.h"
#include "bindable/constantbuffer.h"
#include "bindable/vertexbuffer.h"
#include "bindable/topology.h"
#include "bindable/inputlayout.h"
#include "bindable/vertexshader.h"
#include "bindable/pixelshader.h"
#include "bindable/transformcbuffer.h"

#include "common.h"

class Triangle::Private
{
public:
    Private() = default;
};

namespace Internal 
{
struct Vertex
{
    float x;
    float y;
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
    std::uint8_t a;
};

struct ConstantBuffer
{
    float element[4][4];
};

}

Triangle::Triangle(Graphics& gfx)
    : IDrawable{ gfx }
{
    // 创建顶点缓冲
    std::vector<Internal::Vertex> vertexs = {
        { 0.0f, 0.5f, 255, 0, 0, 0 },
        { 0.5f, -0.5f, 0, 255, 0, 0 },
        { -0.5f, -0.5f, 0, 0, 255, 0 },

        { -0.3f, 0.3f, 0, 255, 0, 0 },
        { 0.3f, 0.3f, 0, 0, 255, 0 },
        { 0.0f, -0.8f, 255, 0, 0, 0 },
    };

    // create index buffer
    std::vector<std::uint16_t> indices = {
        0, 1, 2,
        0, 2, 3,
        0, 4, 1,
        2, 1, 5,
    };

    // create constant buffer
    float angle = 0.0f;
    const Internal::ConstantBuffer cb = {
        {
            (9.0f / 16.0f) * std::cos(angle), std::sin(angle), 0.0f, 0.0f,
            (9.0f / 16.0f) * -std::sin(angle), std::cos(angle), 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0, 0.0f, 1.0f,
        }
    };

    // 创建顶点着色器
    std::wstring vfileName = GetApplicationDirW() + L"\\vertexshader.cso";
    // 创建像素着色器
    std::wstring pfileName = GetApplicationDirW() + L"\\pixshader.cso";

    // input layout
    std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
        { "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    // bind
    addBind(MakeVertexBuffer(gfx, vertexs));
    addIndexBuffer(MakeIndexBuffer(gfx, indices));
    addBind(MakeVertexConstantBuffer<Internal::ConstantBuffer>(gfx, cb));
    auto pvs = MakeVertexShader(gfx, vfileName);
    auto pvsbc = pvs->bytecode();
    addBind(std::move(pvs));
    addBind(MakeInputLayout(gfx, ied, pvsbc));
    addBind(MakePixelShader(gfx, pfileName));
    addBind(MakeTopology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
}

DirectX::XMMATRIX Triangle::transform() const
{
    return DirectX::XMMATRIX();
}

void Triangle::update(float dt)
{
}
