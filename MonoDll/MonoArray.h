/////////////////////////////////////////////////////////////////////////*
//Ink Studios Source File.
//Copyright (C), Ink Studios, 2011.
//////////////////////////////////////////////////////////////////////////
// Wrapper for the MonoArray for less intensively ugly code and
// better workflow.
//////////////////////////////////////////////////////////////////////////
// 17/12/2011 : Created by Filip 'i59' Lundgren
////////////////////////////////////////////////////////////////////////*/
#ifndef __MONO_ARRAY_H__
#define __MONO_ARRAY_H__

#include "MonoObject.h"
#include "MonoClass.h"
#include "MonoAssembly.h"

#include <IMonoArray.h>

class CScriptArray 
	: public CScriptObject
	, public IMonoArray
{
public:
	// Used on MonoArray's returned from C#.
	CScriptArray(mono::object monoArray);
	// Used to send arrays to C#.
	CScriptArray(int size, IMonoClass *pContainingType = nullptr);

	virtual ~CScriptArray();

	// IMonoArray
	virtual void Clear() override { for(int i = 0; i < GetSize(); i++) mono_array_set((MonoArray *)m_pObject, void *, i, nullptr);  }

	virtual void Resize(int size);
	virtual int GetSize() const override { return (int)mono_array_length((MonoArray *)m_pObject); }

	virtual IMonoClass *GetElementClass() override { return CScriptAssembly::TryGetClassFromRegistry(m_pElementClass); }
	virtual IMonoClass *GetDefaultElementClass() { return CScriptAssembly::TryGetClassFromRegistry(m_pDefaultElementClass); }

	virtual IMonoObject *GetItem(int index) override;
	virtual const char *GetItemString(int index) override { return ToCryString(mono_array_get((MonoArray *)m_pObject, mono::string , index)); }

	virtual void InsertNativePointer(void *ptr, int index = -1) override;
	virtual void InsertObject(IMonoObject *pObject, int index = -1) override;
	virtual void InsertAny(MonoAnyValue value, int index = -1) override;
	virtual void InsertMonoString(mono::string string, int index = -1);
	// ~IMonoArray

	// IMonoObject
	virtual void Release() override { delete this; };
	
	virtual EMonoAnyType GetType() override { return eMonoAnyType_Array; }
	virtual MonoAnyValue GetAnyValue() override { return MonoAnyValue(); }

	virtual mono::object GetManagedObject() override { return CScriptObject::GetManagedObject(); }

	virtual IMonoClass *GetClass() override { return CScriptObject::GetClass(); }

	virtual void *UnboxObject() override { return CScriptObject::UnboxObject(); }
	// ~IMonoObject

	virtual void InsertMonoArray(mono::object arr, int index = -1);
	virtual void InsertMonoObject(mono::object object, int index = -1);

	static MonoClass *m_pDefaultElementClass;
private:

	int m_curIndex;

	MonoClass *m_pElementClass;
};

#endif //__MONO_ARRAY_H__