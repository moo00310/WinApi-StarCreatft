#pragma once
#include "Obj.h"
#include "Bullet.h"
class Player :
    public Obj
{
public:
    Player();
    virtual ~Player();

public:
    void SetBulletList(list<Obj*>* _pBullet)
    {
        m_pBullet = _pBullet;
    }

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
private:
    void Key_Input();
    Obj* Create_Bullet(DIRECTION eDir);

private:
    list<Obj*>* m_pBullet;
    ULONGLONG m_dwTime;

};

