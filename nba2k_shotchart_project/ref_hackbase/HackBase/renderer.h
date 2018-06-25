#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "hackbase.h"

// pure virtual functions, need to be instantiated. Drawline and text wasnt, done by myself.
class Renderer {
public:
	virtual void BeginScene() = 0;
	virtual void EndScene() = 0;
	virtual void DrawPic(int x, int y) = 0;
	virtual void DrawRect(int x, int y, int w, int h, Color color) = 0;
	virtual void DrawBorder(int x, int y, int w, int h, int d, Color color) = 0;
	virtual void DrawLine(float x1, float y1, float x2, float y2, int size, bool antialias, Color color) = 0;
	virtual void DrawText(int x, int y, Color FontColor, char *Text, ...) = 0;
	virtual void DrawCircle(int x, int y, float radius, int width, UINT samples, Color color) = 0;
	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
	virtual void loglnDebugInfo() = 0;
};

#endif