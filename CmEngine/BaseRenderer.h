#pragma once
#include "RendererInterface.h"

class FBaseRenderer : public IRendererInterface
{
	void RenderScene(FTreatedRenderInformation * _renderInformation);
	void Release();
};