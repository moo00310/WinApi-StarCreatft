#pragma once

#include "CObj.h"

class CTile :   public CObj
{
public:
	CTile();
	virtual ~CTile();

public:
	int			Get_Option() { return m_iOption; }
	void		Set_Option(int _iOption) { m_iOption = _iOption; }

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

private:
	int		m_iOption;
	void Move_Frame() override;
};

