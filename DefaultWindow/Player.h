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
    void Update() override;
    void Render(HDC hDC) override;
    void Release() override;
private:
    void Key_Input();
    Obj* Create_Bullet(Bullet::WASD _wasd);

private:
    list<Obj*>* m_pBullet;
    DWORD m_dwTime;
};

