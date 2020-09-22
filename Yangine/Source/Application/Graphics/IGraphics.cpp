#include "IGraphics.h"
#include <Application/Graphics/SDLRenderer.h>

using yang::IGraphics;

IGraphics::IGraphics()
	:m_cameraRotation(0)
	,m_cameraScaleFactors(1.f,1.f)
{
	
}

IGraphics::~IGraphics()
{
	
}

std::unique_ptr<IGraphics> yang::IGraphics::Create()
{
	return std::make_unique<SDLRenderer>();
}

bool yang::IGraphics::DrawCircle(const FVec2& center, float radius, const IColor& color, uint8_t segments)
{
	// Those functions should handle logging errors
	if (!SetDrawColor(color))
	{
		return false;
	}
	return DrawCircle(center, radius, segments);
}

bool yang::IGraphics::FillCircle(const FVec2& center, float radius, const IColor& color, uint8_t segments)
{
	// Those functions should handle logging errors
	if (!SetDrawColor(color))
	{
		return false;
	}
	return FillCircle(center, radius, segments);
}

bool yang::IGraphics::DrawPolygon(const std::vector<FVec2>& points, const IColor& color)
{
	// Those functions should handle logging errors
	if (!SetDrawColor(color))
	{
		return false;
	}
	return DrawPolygon(points);
}

bool yang::IGraphics::FillTriangle(FVec2 points[3], const IColor& color)
{
	// Those functions should handle logging errors
	if (!SetDrawColor(color))
	{
		return false;
	}
	return FillTriangle(points);
}

bool yang::IGraphics::DrawRect(const IRect& rect, const IColor& color)
{
	// Those functions should handle logging errors
	if (!SetDrawColor(color))
	{
		return false;
	}
	return DrawRect(rect);
}

bool yang::IGraphics::FillRect(const IRect& rect, const IColor& color)
{
	// Those functions should handle logging errors
	if (!SetDrawColor(color))
	{
		return false;
	}
	return FillRect(rect);
}

bool yang::IGraphics::DrawLine(const IVec2& start, const IVec2& end, const IColor& color)
{
	// Those functions should handle logging errors
	if (!SetDrawColor(color))
	{
		return false;
	}
	return DrawLine(start, end);
}

bool yang::IGraphics::DrawLines(const std::vector<IVec2>& points, const IColor& color)
{
	// Those functions should handle logging errors
	if (!SetDrawColor(color))
	{
		return false;
	}
	return DrawLines(points);
}

bool yang::IGraphics::DrawRect(const FRect& rect, const IColor& color)
{
	// Those functions should handle logging errors
	if (!SetDrawColor(color))
	{
		return false;
	}
	return DrawRect(rect);
}

bool yang::IGraphics::FillRect(const FRect& rect, const IColor& color)
{
	// Those functions should handle logging errors
	if (!SetDrawColor(color))
	{
		return false;
	}
	return FillRect(rect);
}

bool yang::IGraphics::DrawLine(const FVec2& start, const FVec2& end, const IColor& color)
{
	// Those functions should handle logging errors
	if (!SetDrawColor(color))
	{
		return false;
	}
	return DrawLine(start, end);
}

bool yang::IGraphics::DrawLines(const std::vector<FVec2>& points, const IColor& color)
{
	// Those functions should handle logging errors
	if (!SetDrawColor(color))
	{
		return false;
	}
	return DrawLines(points);
}