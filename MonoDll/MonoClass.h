/////////////////////////////////////////////////////////////////////////*
//Ink Studios Source File.
//Copyright (C), Ink Studios, 2011.
//////////////////////////////////////////////////////////////////////////
// Wrapper for the MonoClass for less intensively ugly code and
// better workflow.
//////////////////////////////////////////////////////////////////////////
// 17/12/2011 : Created by Filip 'i59' Lundgren
////////////////////////////////////////////////////////////////////////*/
#ifndef __MONO_CLASS_H__
#define __MONO_CLASS_H__

#include "MonoObject.h"
#include "MonoAssembly.h"

#include <MonoCommon.h>
#include <IMonoClass.h>

struct IMonoArray;


class CScriptClass 
	: public CScriptObject
	, public IMonoClass
{
public:
	CScriptClass(MonoClass *pClass, CScriptAssembly *pDeclaringAssembly);
	virtual ~CScriptClass();

	// IMonoClass
	virtual const char *GetName() override { return m_name.c_str(); }
	virtual const char *GetNamespace() override { return m_namespace.c_str(); }

	virtual IMonoAssembly *GetAssembly() { return m_pDeclaringAssembly; }

	virtual IMonoObject *CreateInstance(IMonoArray *pConstructorParams = nullptr) override;

	IMonoObject *BoxObject(void *object) override;

	virtual void AddRef() override { ++m_refs; }
	// ~IMonoClass

	// IMonoObject
	virtual void Release() override { if(0 >= --m_refs) delete this; }

	virtual IMonoObject *InvokeArray(const char *methodName, IMonoArray *params = nullptr, bool bStatic = false) override { return CScriptObject::InvokeArray(methodName, params, true); }
	virtual IMonoObject *Invoke(const char *methodName, void **params = nullptr, int numParams = 0,  bool bStatic = false) override { return CScriptObject::Invoke(methodName, params, numParams, bStatic); }

	virtual IMonoObject *GetProperty(const char *propertyName, bool bStatic = false) override { return CScriptObject::GetProperty(propertyName, true); }
	virtual void SetProperty(const char *propertyName, IMonoObject *pNewValue, bool bStatic = false) override { CScriptObject::SetProperty(propertyName, pNewValue, true); }
	virtual IMonoObject *GetField(const char *fieldName, bool bStatic = false) override { return CScriptObject::GetField(fieldName, true); }
	virtual void SetField(const char *fieldName, IMonoObject *pNewValue, bool bStatic = false) override { CScriptObject::SetField(fieldName, pNewValue, true); }

	virtual EMonoAnyType GetType() override { return eMonoAnyType_Class; }

	virtual mono::object GetManagedObject() override { return CScriptObject::GetManagedObject(); }

	virtual IMonoClass *GetClass() override { return this; }

	virtual void *UnboxObject() override { return CScriptObject::UnboxObject(); }
	// ~IMonoObject

	// CScriptObject
	virtual void OnPostScriptReload(bool initialLoad) override;
	// ~CScriptObject

	MonoMethod *GetMonoMethod(const char *name, IMonoArray *pArgs);
	MonoMethod *GetMonoMethod(const char *name, int numParams);

	MonoProperty *GetMonoProperty(const char *name);
	MonoClassField *GetMonoField(const char *name);

private:
	string m_name;
	string m_namespace;

	CScriptAssembly *m_pDeclaringAssembly;

	int m_refs;
};

#endif //__MONO_CLASS_H__