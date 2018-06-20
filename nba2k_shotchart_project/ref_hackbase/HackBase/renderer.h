#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "hackbase.h"

// pure virtual functions, need to be instantiated. Drawline and text wasnt, done by myself.
class Renderer {
public:
	virtual void BeginScene() = 0;
	virtual void EndScene() = 0;
	virtual void DrawRect(int x, int y, int w, int h, Color color) = 0;
	virtual void DrawBorder(int x, int y, int w, int h, int d, Color color) = 0;
	virtual void DrawLine(int x1, int y1, int x2, int y2, float size, bool antialias, Color color) = 0;
	virtual void DrawText(int x, int y, char *Text, ...) = 0;
	virtual void DrawCircle(int x, int y, float radius, int width, UINT samples, Color color) = 0;
	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
	virtual void loglnDebugInfo() = 0;
};

#endif