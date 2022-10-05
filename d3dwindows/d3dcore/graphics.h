#pragma once
#include "global.h"

class Graphics 
{
public:
    using  Pointer = std::unique_ptr<Graphics>;

    Graphics(HWND hwnd);
    ~Graphics();

    void render();

private:
    DECLARE_PRIVATE(Graphics)
    DISABLE_COPY(Graphics)
};

DEFINE_MAKE_UNIQUE_PTR(Graphics)