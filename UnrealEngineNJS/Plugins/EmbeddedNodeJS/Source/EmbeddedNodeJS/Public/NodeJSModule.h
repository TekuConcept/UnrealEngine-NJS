//
// Created by TekuConcept on October 17, 2021
//

#pragma once

#include <memory>
#include "Modules/ModuleManager.h"
#include "node.h"


class FNodeJSModule final :
	public IModuleInterface,
	public FTickableGameObject
{
public:
	FNodeJSModule();
	virtual void              StartupModule() override;
	virtual void              ShutdownModule() override;
	virtual void              Tick(float Duration) override;
	virtual bool              IsTickable() const override final { return M_IsReady; }
	virtual bool              IsTickableInEditor() const override { return true; }
	virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Conditional; }
	virtual TStatId           GetStatId() const override { return M_StatId; }

private:
	TStatId      M_StatId;
	void*        M_LibnodeHandle;
	bool         M_IsReady;

	std::shared_ptr<node::NodeContext> M_Context;
	bool                               M_IsEol;

	inline int  StartupNodejs();
	inline void ShutdownNodejs();
};
