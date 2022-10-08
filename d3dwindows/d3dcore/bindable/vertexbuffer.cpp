#include "vertexbuffer.h"

void VertexBuffer::bind()
{
    const UINT offset = 0;
    graphics().deviceContext()->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
}
