#pragma once
#include "RendererInterface.h"
#include "RenderSetting.h"

class FRendererCreater
{
	FRendererCreater() {}
public:
	static FRendererCreater & Get()
	{
		static FRendererCreater single;
		return single;
	}

	IRendererInterface * CreateRenderer(FRenderSetting _renderSetting);
};