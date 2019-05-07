#pragma once
#include "RendererInterface.h"
#include "RenderSetting.h"
#include "Template.h"

class FRendererCreater final : public Tpl::TAsSingle<FRendererCreater>
{
public:
	IRendererInterface * CreateRenderer(FRenderSetting _renderSetting);
};
