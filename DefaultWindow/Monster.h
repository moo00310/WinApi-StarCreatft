#pragma once
#include "Obj.h"
class Monster :
    public Obj
{
public:
    Monster();
    virtual ~Monster();

public:
    void Initialize() override;
    void Update() override;
    void Render(HDC hDC) override;
    void Release() override;

private:
    bool m_bRight;
};

