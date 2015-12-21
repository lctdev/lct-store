#ifndef CURSOR_OVERLAY_H
#define CURSOR_OVERLAY_H

#include "assetViewerOverlay.h"

#include <foun/foun_matrix.h>
#include <foun/foun_vector.h>

class CursorOverlay : public AssetViewerOverlay
{
public:
	CursorOverlay();
	virtual ~CursorOverlay();

	virtual void Init();

	virtual void AcquireGraphics();
	virtual void ReleaseGraphics();

	virtual void ReadInput();
	virtual void Update();
	virtual void Draw();

private:
	struct Marker
	{
		lct::foun::Vector2 position;
		f32 alpha;
	};

	void BeginMarker(Marker& marker);
	void UpdateMarker(Marker& marker);
	void DrawMarker(Marker& marker, const lct::foun::FloatColor3& color);

	lct::foun::Matrix44 m_projectionTransform;	

	Marker m_pressMarker;
	Marker m_releaseMarker;
	static const u32 DOWN_MARKER_COUNT = 60;
	Marker m_downMarkers[DOWN_MARKER_COUNT];
	u32 m_currDownMarkerIndex;
};

#endif//CURSOR_OVERLAY_H
