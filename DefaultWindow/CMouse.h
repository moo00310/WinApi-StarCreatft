#pragma once

#include "CObj.h"
#include "CTile.h"


class CMouse : public CObj
{
public:
	CMouse(): m_iScrollX(0), m_iScrollY(0), m_dwTime(0) {}
	virtual ~CMouse() {}

public:
	virtual void Initialize() PURE;
	virtual int Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC hDC) PURE;
	virtual void Release() PURE;

	void SetScroll();
	void LockMouse();

protected:
	int m_iScrollX;
	int m_iScrollY;
	ULONGLONG m_dwTime;
};

/*---------------
	EditMouse
--------------------*/
class EditMouse : public CMouse
{
public:
	EditMouse();
	~EditMouse();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

	void GetEditInfo(EditType _type , int _id)
	{ 
		m_eEditType = _type;
		m_iDrawID = _id; 
	}

private:
	EditType m_eEditType;
	int m_iDrawID;
	ULONGLONG m_dwTime;
};


/*---------------
	GameMouse
--------------------*/
class GameMouse : public CMouse
{

};