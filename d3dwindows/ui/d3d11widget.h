#pragma once

#include <QtWidgets>

#include "global.h"

class D3d11Widget : public QWidget
{
public:
    D3d11Widget(QWidget *parent = nullptr);
    ~D3d11Widget();

protected:
    QPaintEngine* paintEngine() const override;

private:
    DECLARE_PRIVATE(D3d11Widget)
};