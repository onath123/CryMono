#include "StdAfx.h"
#include "MonoInput.h"

#include "MonoScriptSystem.h"

#include <IGameFramework.h>

#include <IMonoAssembly.h>
#include <IMonoClass.h>
#include <IMonoArray.h> 

TActionHandler<CInput>	CInput::s_actionHandler;

CInput::CInput()
{
	REGISTER_METHOD(RegisterAction);

	gEnv->pGameFramework->GetIActionMapManager()->AddExtraActionListener(this);
	gEnv->pHardwareMouse->AddListener(this);
	gEnv->pInput->AddEventListener(this);
}

CInput::~CInput()
{
	// The code below currently crashes the Launcher at shutdown
	/*if(gEnv->pGameFramework)
	{
		if(IActionMapManager *pActionmapManager = gEnv->pGameFramework->GetIActionMapManager())
			pActionmapManager->RemoveExtraActionListener(this);
	}*/

	if(gEnv->pHardwareMouse)
		gEnv->pHardwareMouse->RemoveListener(this);

	if(gEnv->pInput)
		gEnv->pInput->RemoveEventListener(this);
}

IMonoClass *CInput::GetClass()
{
	return g_pScriptSystem->GetCryBraryAssembly()->GetClass("Input");
}

void CInput::OnHardwareMouseEvent(int iX,int iY,EHARDWAREMOUSEEVENT eHardwareMouseEvent, int wheelDelta)
{
	IMonoArray *pParams = CreateMonoArray(4);
	pParams->Insert(iX);
	pParams->Insert(iY);
	pParams->Insert(eHardwareMouseEvent);
	pParams->Insert(wheelDelta);

	GetClass()->InvokeArray(NULL, "OnMouseEvent", pParams);
	SAFE_RELEASE(pParams);
}

bool CInput::OnInputEvent(const SInputEvent &event)
{
	IMonoArray *pParams = CreateMonoArray(2);
	pParams->Insert(event.keyName.c_str());
	pParams->Insert(event.value);

	GetClass()->InvokeArray(NULL, "OnKeyEvent", pParams);
	SAFE_RELEASE(pParams);

	return false;
}

void CInput::OnAction(const ActionId& actionId, int activationMode, float value)
{
	s_actionHandler.Dispatch(this, 0, actionId, activationMode, value);
}

bool CInput::OnActionTriggered(EntityId entityId, const ActionId& actionId, int activationMode, float value)
{
	IMonoArray *pParams = CreateMonoArray(3);
	pParams->Insert(actionId.c_str());
	pParams->Insert(activationMode);
	pParams->Insert(value);

	GetClass()->InvokeArray(NULL, "OnActionTriggered", pParams);
	SAFE_RELEASE(pParams);

	return false;
}

// Scriptbinds
void CInput::RegisterAction(mono::string actionName)
{
	if(!s_actionHandler.GetHandler(ToCryString(actionName)))
		s_actionHandler.AddHandler(ToCryString(actionName), &CInput::OnActionTriggered);
}