#include "cursorOverlay.h"
#include "assetViewerMessages.h"

/*
* Internal Constants
*/
static const lct::foun::FloatColor3 PRESS_COLOR = { 0.0f, 1.0f, 0.0f };
static const lct::foun::FloatColor3 RELEASE_COLOR = { 1.0f, 0.0f, 0.0f };
static const lct::foun::FloatColor3 DOWN_COLOR = { 0.0f, 0.0f, 1.0f };

static const f32 MARKER_FRAMES = 60.0f;
static const f32 MARKER_ALPHA_STEP = 1.0f / MARKER_FRAMES;
static const f32 MARKER_SIZE = 64.0f;

/*
* Public Instance
*/
CursorOverlay::CursorOverlay()
: AssetViewerOverlay()

, m_projectionTransform()

, m_pressMarker()
, m_releaseMarker()
, m_downMarkers()
, m_currDownMarkerIndex(0)
{
}

CursorOverlay::~CursorOverlay()
{
}

void CursorOverlay::Init()
{
	AssetViewerOverlay::Init();
}

void CursorOverlay::AcquireGraphics()
{
}

void CursorOverlay::ReleaseGraphics()
{
}

void CursorOverlay::ReadInput()
{
	if (m_subShared.pCursor->IsPress())
	{
		BeginMarker(m_pressMarker);
	}
	if (m_subShared.pCursor->IsRelease())
	{
		BeginMarker(m_releaseMarker);
	}
	if (m_subShared.pCursor->IsDown())
	{
		BeginMarker(m_downMarkers[m_currDownMarkerIndex]);
		++m_currDownMarkerIndex;
		if (m_currDownMarkerIndex >= DOWN_MARKER_COUNT)
		{
			m_currDownMarkerIndex = 0;
		}
	}
}

void CursorOverlay::Update()
{
	const lct::foun::RectEdges& screenEdges = m_shared.pScreen->GetRectEdges();

	lct::foun::Matrix44OrthographicProjection(m_projectionTransform, screenEdges.left, screenEdges.right, screenEdges.bottom, screenEdges.top, -1.0f, 1.0f);

	UpdateMarker(m_pressMarker);
	UpdateMarker(m_releaseMarker);
	for (u32 downMarkerIndex = 0; downMarkerIndex < DOWN_MARKER_COUNT; ++downMarkerIndex)
	{
		UpdateMarker(m_downMarkers[downMarkerIndex]);
	}
}

void CursorOverlay::Draw()
{
	m_subShared.pFillDrawContext->ActivateRenderState();
	m_subShared.pFillDrawContext->ActivateShader();
	m_subShared.pFillDrawContext->ActivateQuad();
	m_subShared.pFillDrawContext->ActivateProjectionTransform(m_projectionTransform);

	lct::foun::Matrix32 cursorWorldTransform;
	lct::foun::Matrix32Identity(cursorWorldTransform);
	m_subShared.pFillDrawContext->ActivateWorldTransform(cursorWorldTransform);
	
	for (u32 downMarkerIndex = 0; downMarkerIndex < DOWN_MARKER_COUNT; ++downMarkerIndex)
	{
		DrawMarker(m_downMarkers[downMarkerIndex], DOWN_COLOR);
	}
	DrawMarker(m_pressMarker, PRESS_COLOR);
	DrawMarker(m_releaseMarker, RELEASE_COLOR);
}

/*
* Private Instance
*/
void CursorOverlay::BeginMarker(Marker& marker)
{
	const lct::foun::RectEdges& screenEdges = m_shared.pScreen->GetRectEdges();

	marker.position.x = screenEdges.left + static_cast<f32>(m_subShared.pCursor->GetX());
	marker.position.y = screenEdges.top - static_cast<f32>(m_subShared.pCursor->GetY());

	marker.alpha = 1.0f;
}

void CursorOverlay::UpdateMarker(Marker& marker)
{
	if (marker.alpha > 0.0f)
	{
		marker.alpha -= MARKER_ALPHA_STEP;

		if (marker.alpha < 0.0f)
		{
			marker.alpha = 0.0f;
		}
	}
}

void CursorOverlay::DrawMarker(Marker& marker, const lct::foun::FloatColor3& color)
{
	if (marker.alpha <= 0.0f)
	{
		return;
	}

	lct::foun::RectCentered cursorRect;
	cursorRect.width = MARKER_SIZE;
	cursorRect.height = MARKER_SIZE;

	lct::foun::FloatColor4 cursorColor;

	cursorRect.x = marker.position.x;
	cursorRect.y = marker.position.y;

	cursorColor.r = color.r;
	cursorColor.g = color.g;
	cursorColor.b = color.b;
	cursorColor.a = marker.alpha;

	m_subShared.pFillDrawContext->DrawRect(cursorRect, cursorColor);
}
