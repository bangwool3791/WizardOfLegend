#pragma once
#include "Scene.h"
class CStage :
	public CScene
{
public:
	CStage();
	virtual ~CStage();
private :
	void	Insert_Text();
	void	InitializeSound(void);
public:
	virtual void Initialize(void) override;

	virtual void Update(void) override;

	virtual void Late_Update(void) override;

	virtual void Render(HDC _DC) override;

	virtual void Release(void) override;

};

