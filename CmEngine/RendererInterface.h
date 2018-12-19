#pragma once
#include "TreatedRenderInformation.h"

__interface IRendererInterface
{
	void RenderScene(FTreatedRenderInformation * _renderInformation);
	void Release();
};