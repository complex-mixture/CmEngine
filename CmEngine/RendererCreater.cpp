#include "RendererCreater.h"
#include "BaseRenderer.h"

IRendererInterface * FRendererCreater::CreateRenderer(FRenderSetting _renderSetting)
{
	return new FBaseRenderer;
}
