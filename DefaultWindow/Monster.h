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
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

};

