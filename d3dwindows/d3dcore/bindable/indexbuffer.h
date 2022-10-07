#pragma once

#include <vector>
#include "global.h"
#include "ibindable.h"

class IndexBuffer : public IBindable 
{
public:
    IndexBuffer(Graphics& gfx, const std::vector<std::uint16_t>& indices);
    ~IndexBuffer();

    void bind() override;
    std::size_t count() const;

private:
    DECLARE_PRIVATE(IndexBuffer)
};
