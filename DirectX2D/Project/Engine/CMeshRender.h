#pragma once
#include "CRenderComponent.h"
class CMeshRender :
    public CRenderComponent
{
private:

public:
    virtual void UpdateData() override;
    virtual void render() override;

public:
    CMeshRender();
    ~CMeshRender();
};

