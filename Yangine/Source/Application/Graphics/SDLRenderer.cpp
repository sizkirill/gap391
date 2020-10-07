#include "SDLRenderer.h"
#include <Utils/Logger.h>
#include <Application/Window/SDLWindow.h>
#include <Application/Graphics/Textures/SDLTexture.h>
#include <SDL/SDL_image.h>
#include <cassert>

using yang::SDLRenderer;

SDLRenderer::SDLRenderer()
	:m_pRenderer(nullptr, &SDL_DestroyRenderer)
{
	
}

SDLRenderer::~SDLRenderer()
{
	
}

bool yang::SDLRenderer::Initialize(IWindow* pWindow)
{
	SDL_Window* pSDLWindow = reinterpret_cast<SDL_Window*>(pWindow->GetNativeWindow());

	m_pRenderer.reset(SDL_CreateRenderer(pSDLWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));

	if (!m_pRenderer)
	{
		LOG(Error, "SDL_Renderer failed to initialize, %s", SDL_GetError());
		return false;
	}

	return true;
}

bool yang::SDLRenderer::StartDrawing(uint8_t red, uint8_t green, uint8_t blue, uint8_t /*alpha*/)
{
	SDL_SetRenderDrawColor(m_pRenderer.get(), red, green, blue, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(m_pRenderer.get());

	// TODO: Check for SDL errors
	return true;
}

void yang::SDLRenderer::EndDrawing()
{
	SDL_RenderPresent(m_pRenderer.get());
}

std::shared_ptr<yang::ITexture> yang::SDLRenderer::LoadTexture(IResource* pResource)
{
    if (!m_pRenderer)
    {
        LOG(Warning, "SDL_Renderer is nullptr");
        return nullptr;
    }

    SDL_RWops* pOps = SDL_RWFromConstMem(pResource->GetData().data(), static_cast<int>(pResource->GetData().size()));
    SDL_Surface* pSurface = IMG_Load_RW(pOps, 0);
    
    if (!pSurface)
    {
        LOG(Error, "Unable to load image. Error: %s", SDL_GetError());
        return nullptr;
    }

    std::shared_ptr<ITexture> pTexture = std::make_shared<SDLTexture>(pResource);
    
    if (!static_cast<SDLTexture*>(pTexture.get())->Init(m_pRenderer.get(), pSurface))
    {
        LOG(Error, "Unable to init SDLTexture");
        return nullptr;
    }
    return pTexture;
}

bool yang::SDLRenderer::DrawTexture(ITexture* pTexture, IVec2 position, const TextureDrawParams& drawParams)
{
	if (!pTexture)
	{
		LOG(Error, "Texture was nullptr");
		return false;
	}

	SDL_Texture* pSDLTexture = reinterpret_cast<SDL_Texture*>(pTexture->GetNativeTexture());

	SDL_FPoint pointToRotate;
	if (drawParams.m_pointToRotate.has_value())
	{
		pointToRotate.x = (float)drawParams.m_pointToRotate.value().x;
		pointToRotate.y = (float)drawParams.m_pointToRotate.value().y;
	}

	//FVec2 transformedPosition = m_cameraTransform.TransformPoint(FVec2(position));
	IVec2 dimensions = pTexture->GetDimensions();
	FRect dstRect{(float)position.x, (float)position.y, (float)dimensions.x, (float)dimensions.y};

	SDL_FRect destination = ToSDLFRect(m_cameraTransform.TransformAARect(dstRect));

	if (SDL_RenderCopyExF(m_pRenderer.get(), pSDLTexture, nullptr, &destination, drawParams.m_angle + Math::ToDegrees(m_cameraRotation),
		(drawParams.m_pointToRotate ? &pointToRotate : nullptr), ToRendererFlip(drawParams.m_flip)))
    {
        LOG(Error, "Unable to draw SDL_Texture. Error: %s", SDL_GetError());
        return false;
    }
    return true;
}

bool yang::SDLRenderer::DrawTexture(ITexture* pTexture, const IRect& dest, const TextureDrawParams& drawParams)
{
    if (!pTexture)
    {
        LOG(Error, "Texture was nullptr");
        return false;
    }

	FVec2 destOrigin = m_cameraTransform.TransformPoint(FVec2((float)dest.x, (float)dest.y));
	//SDL_FRect destination = ToSDLFRect(m_cameraTransform.TransformAARect(dest));
	SDL_FRect destination {destOrigin.x, destOrigin.y, (float)dest.width * m_cameraScaleFactors.x, (float)dest.height * m_cameraScaleFactors.y};

    SDL_Texture* pSDLTexture = reinterpret_cast<SDL_Texture*>(pTexture->GetNativeTexture());

	SDL_FPoint pointToRotate;
	if (drawParams.m_pointToRotate.has_value())
	{
		pointToRotate.x = (float)drawParams.m_pointToRotate.value().x;
		pointToRotate.y = (float)drawParams.m_pointToRotate.value().y;
	}

    if (SDL_RenderCopyExF(m_pRenderer.get(), pSDLTexture, nullptr, &destination, drawParams.m_angle + Math::ToDegrees(m_cameraRotation), 
		(drawParams.m_pointToRotate ? &pointToRotate : nullptr), ToRendererFlip(drawParams.m_flip)))
    {
        LOG(Error, "Unable to draw SDL_Texture. Error: %s", SDL_GetError());
        return false;
    }
    return true;
}

bool yang::SDLRenderer::DrawTexture(ITexture* pTexture, const IRect& src, const IRect& dest, const TextureDrawParams& drawParams)
{
    if (!pTexture)
    {
        LOG(Error, "Texture was nullptr");
        return false;
    }

    SDL_Rect source = ToSDLRect(src);
	FVec2 destOrigin = m_cameraTransform.TransformPoint(FVec2((float)dest.x, (float)dest.y));
    //SDL_FRect destination = ToSDLFRect(m_cameraTransform.TransformAARect(dest));
	SDL_FRect destination {destOrigin.x, destOrigin.y, (float)(dest.width+1) * m_cameraScaleFactors.x, (float)(dest.height+1) * m_cameraScaleFactors.y};
    SDL_Texture* pSDLTexture = reinterpret_cast<SDL_Texture*>(pTexture->GetNativeTexture());

	SDL_FPoint pointToRotate;
	if (drawParams.m_pointToRotate.has_value())
	{
		pointToRotate.x = (float)drawParams.m_pointToRotate.value().x;
		pointToRotate.y = (float)drawParams.m_pointToRotate.value().y;
	}

    if (SDL_RenderCopyExF(m_pRenderer.get(), pSDLTexture, &source, &destination, drawParams.m_angle + Math::ToDegrees(m_cameraRotation),
		(drawParams.m_pointToRotate ? &pointToRotate : nullptr), ToRendererFlip(drawParams.m_flip)))
    {
        LOG(Error, "Unable to draw SDL_Texture. Error: %s", SDL_GetError());
        return false;
    }
    return true;
}

bool yang::SDLRenderer::DrawLines(const std::vector<IVec2>& points)
{
	std::vector<FVec2> fpoints(points.size());
	std::transform(points.cbegin(), points.cend(), fpoints.begin(), [this](const IVec2& point){ return m_cameraTransform.TransformPoint(FVec2(point)); });
	// On one hand, I probably shouldn't do this. On the other - what can go wrong? Both are just structs with 'int x' and 'int y' members. Making a new vector and copying data is too expensive
	assert(sizeof(FVec2) == sizeof(SDL_FPoint));
	if (SDL_RenderDrawLinesF(m_pRenderer.get(), reinterpret_cast<const SDL_FPoint*>(fpoints.data()), static_cast<int>(fpoints.size())))
	{
		LOG(Error, "Unable to draw lines. Error: %s", SDL_GetError());
		return false;
	};
	
	return true;
}

bool yang::SDLRenderer::DrawCircle(const FVec2& center, float radius, uint8_t segments)
{
	std::vector<FVec2> points;
	points.reserve(segments);

	for (size_t i = 0; i < segments; ++i)
	{
		points.emplace_back(center.x + radius * std::cosf(2 * Math::kPi * i / segments), center.y +  radius * std::sinf(2 * Math::kPi * i / segments));
	}
	
	return DrawPolygon(points);
}

bool yang::SDLRenderer::DrawPolygon(const std::vector<FVec2>& points)
{
	assert(sizeof(FVec2) == sizeof(SDL_FPoint));
	return DrawLines(points) && DrawLine(points[points.size() - 1], points[0]);
}

bool yang::SDLRenderer::FillTriangle(FVec2 points[3])
{
	// I want a local copy, we might not want to modify the incoming array
	FVec2 vertices[3];
	memcpy(vertices, points, sizeof(FVec2) * 3);

	// Start to draw at top point
	std::sort(vertices, vertices + 3, [](const FVec2& first, const FVec2& second) {return first.y < second.y; });

	const FVec2* pRightVertex;
	const FVec2* pLeftVertex;

	// We need to know which one is left and which one is right
	if (vertices[1].x > vertices[0].x)
	{
		pRightVertex = &vertices[1];
		pLeftVertex = &vertices[2];
	}
	else
	{
		pRightVertex = &vertices[2];
		pLeftVertex = &vertices[1];
	}

	// Initial delta's. 
	Vector2 leftDelta = vertices[0] - *pLeftVertex;
	Vector2 rightDelta = vertices[0] - *pRightVertex;

	int y = static_cast<int>(vertices[0].y);
	float deltaLeftX = 0;
	float deltaRightX = 0;

	// Consider storing all lines and draw them all in a one bunch.
	// Starting drawing from the top vertex until we reach next vertex
	for (; y < static_cast<int>(vertices[1].y); ++y)
	{
		FVec2 p1(vertices[0].x - deltaLeftX, static_cast<float>(y));
		FVec2 p2(vertices[0].x + deltaRightX, static_cast<float>(y));

		if (!DrawLine(p1, p2))
		{
			return false;
		}

		deltaLeftX -= leftDelta.x / leftDelta.y;
		deltaRightX += rightDelta.x / rightDelta.y;
	}

	// We reached next vertex. Now we should determine if the vertex was left or right and do stuff according to that information
	if (pRightVertex->y < pLeftVertex->y)
	{
		rightDelta = vertices[1] - vertices[2];
		deltaRightX = 0;
		deltaLeftX += vertices[1].x - vertices[0].x;
	}
	else
	{
		leftDelta = vertices[2] - vertices[1];
		deltaLeftX = 0;
		deltaRightX += vertices[0].x - vertices[1].x;
	}

	// TODO: Get rid of those static casts ?
	// Doing the same as in prev loop but from second vertex to third.
	for (; y < static_cast<int>(vertices[2].y); ++y)
	{
		FVec2 p1(vertices[1].x - deltaLeftX, static_cast<float>(y));
		FVec2 p2(vertices[1].x + deltaRightX, static_cast<float>(y));

		if (!DrawLine(p1, p2))
		{
			return false;
		}

		deltaLeftX -= leftDelta.x / leftDelta.y;
		deltaRightX += rightDelta.x / rightDelta.y;

	}

	return true;
}

bool yang::SDLRenderer::FillCircle(const FVec2& center, float radius, uint8_t segments)
{
	// Starting at top point, so initial angle is pi/2
	FVec2 currentLeft = center - FVec2(0, radius);
	FVec2 currentRight = currentLeft;
	float currentAngle = Math::kPi / 2;

	// Starting at top point. Also rounding number of segments to be an even number
	for (int i = 0; i <= segments / 2; ++i)
	{
		FVec2 delta(std::cosf(currentAngle), std::sinf(currentAngle));

		FVec2 nextLeft = center - radius * delta;
		FVec2 nextRight = center + radius * delta;

		float cumulativeDeltaX = currentLeft.x - nextLeft.x;
		float stepDeltaX = cumulativeDeltaX / (nextLeft.y - currentLeft.y);

		// TODO: Get rid of those static_casts ?
		for (size_t y = static_cast<size_t>(currentLeft.y); y < static_cast<size_t>(nextLeft.y); ++y)
		{
			// TODO: think if it's better to draw all lines at once (but there will be twice as more lines then)

			FVec2 p1(nextLeft.x + cumulativeDeltaX, static_cast<float>(y));
			FVec2 p2(nextRight.x - cumulativeDeltaX, static_cast<float>(y));
			cumulativeDeltaX -= stepDeltaX;
			if (!DrawLine(p1, p2))
			{
				return false;
			}
		}

		currentLeft = nextLeft;
		currentRight = nextRight;
		currentAngle += 2 * Math::kPi / segments;
	}

	return true;
}

SDL_RendererFlip yang::SDLRenderer::ToRendererFlip(FlipDirection flip)
{
	switch (flip)
	{
	case FlipDirection::kVertical:
		return SDL_RendererFlip::SDL_FLIP_VERTICAL;
	case FlipDirection::kHorizontal:
		return SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
	case FlipDirection::kNone:
		return SDL_RendererFlip::SDL_FLIP_NONE;
	default:
		return SDL_RendererFlip::SDL_FLIP_NONE;
	}
}

bool yang::SDLRenderer::SetRenderTarget(ITexture* pTarget)
{

	SDL_Texture* pTargetNativeTexture = pTarget ? reinterpret_cast<SDL_Texture*>(pTarget->GetNativeTexture()) : nullptr;

	if (SDL_SetRenderTarget(m_pRenderer.get(), pTargetNativeTexture))
	{
		LOG(Error, "Unable to set render target. Error: %s", SDL_GetError());
		return false;
	}
	return true;
}

bool yang::SDLRenderer::SetDrawColor(const IColor& color)
{
	if (SDL_SetRenderDrawColor(m_pRenderer.get(), color.Red(), color.Green(), color.Blue(), color.Alpha()))
	{
		LOG(Error, "Unable to set RenderColor. Error: %s", SDL_GetError());
		return false;
	}
	return true;
}

std::shared_ptr<yang::ITexture> yang::SDLRenderer::CreateTexture(IVec2 dimensions)
{
	SDL_Texture* pTexture = SDL_CreateTexture(m_pRenderer.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dimensions.x, dimensions.y);
	SDL_SetTextureBlendMode(pTexture, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(m_pRenderer.get(), pTexture);
	SDL_SetRenderDrawColor(m_pRenderer.get(), 0, 0, 0, 0);
	SDL_RenderClear(m_pRenderer.get());
	SDL_SetRenderTarget(m_pRenderer.get(), nullptr);

	// some evil actions here. We want to access private constructor of SDL_Texture, and std::make_shared cannot do that.
	SDLTexture* pRetVal = new SDLTexture(pTexture);
	pRetVal->SetDimensions(dimensions);
	return std::shared_ptr<SDLTexture>(pRetVal);
}

bool yang::SDLRenderer::DrawRect(const FRect& rect)
{
	SDL_FRect toDraw = ToSDLFRect(m_cameraTransform.TransformAARect(rect));

    if (SDL_RenderDrawRectF(m_pRenderer.get(), &toDraw))
    {
        LOG(Error, "Unable to draw SDL_Rect. Error: %s", SDL_GetError());
        return false;
    }
    return true;
}

bool yang::SDLRenderer::FillRect(const FRect& rect)
{
    SDL_FRect toDraw = ToSDLFRect(m_cameraTransform.TransformAARect(rect));

    if (SDL_RenderFillRectF(m_pRenderer.get(), &toDraw))
    {
        LOG(Error, "Unable to fill SDL_Rect. Error: %s", SDL_GetError());
        return false;
    }
    return true;
}

bool yang::SDLRenderer::DrawLine(const FVec2& start, const FVec2& end)
{
	FVec2 _start = m_cameraTransform.TransformPoint(start);
	FVec2 _end = m_cameraTransform.TransformPoint(end);

	if (SDL_RenderDrawLineF(m_pRenderer.get(), _start.x, _start.y, _end.x, _end.y))
	{
		LOG(Error, "Unable to draw line. Error: %s", SDL_GetError());
		return false;
	}
	return true;
}

bool yang::SDLRenderer::DrawLines(const std::vector<FVec2>& points)
{
	std::vector<FVec2> transformedPoints(points.size());
	std::transform(points.cbegin(), points.cend(), transformedPoints.begin(), [this](const FVec2& point){return m_cameraTransform.TransformPoint(point); });
	// On one hand, I probably shouldn't do this. On the other - what can go wrong? Both are just structs with 'float x' and 'float y' members.
	assert(sizeof(FVec2) == sizeof(SDL_FPoint));
	if (SDL_RenderDrawLinesF(m_pRenderer.get(), reinterpret_cast<const SDL_FPoint*>(transformedPoints.data()), static_cast<int>(transformedPoints.size())))
	{
		LOG(Error, "Unable to draw lines. Error: %s", SDL_GetError());
		return false;
	};

	return true;
}

bool yang::SDLRenderer::DrawRect(const IRect& rect)
{
	return yang::SDLRenderer::DrawRect(FRect(rect));
}

bool yang::SDLRenderer::FillRect(const IRect& rect)
{
	return yang::SDLRenderer::FillRect(FRect(rect));
}

bool yang::SDLRenderer::DrawLine(const IVec2& start, const IVec2& end)
{
	return yang::SDLRenderer::DrawLine(yang::FVec2(start), yang::FVec2(end));
}
