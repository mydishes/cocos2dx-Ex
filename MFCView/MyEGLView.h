//******************************************************************
// FILENAME:	MyEGLView.h
// AUTHOR  :	Steaphan
// VERSION :	1.0.0
// DESCRIBE:	cocos2dx view for MFC 
//		create view by MFC HWND
// GitHub  :    https://github.com/Steaphans/cocos2dx-Ex
//******************************************************************
#ifndef __CMYEGLVIEW_H__
#define __CMYEGLVIEW_H__

#include "base/CCRef.h"
#include "platform/CCCommon.h"
#include "platform/CCGLView.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#ifndef GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#endif
#ifndef GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WGL
#endif
#endif /* (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) */
#include "cocos2d.h"
NS_CC_BEGIN

class /*CC_DLL*/ CMyEGLView : public cocos2d::GLView
{
public:
	static CMyEGLView* createWithRect(const std::string& viewName, Rect size, HWND  handle, float frameZoomFactor = 1.0f);

	bool initWithRect(const std::string& viewName, Rect rect, HWND  handle, float frameZoomFactor);

	void onGLFWError(int errorID, const char* errorDesc);

	/* override functions */
	virtual bool isOpenGLReady() override;
	virtual void end() override;
	virtual void swapBuffers() override;
	virtual void setIMEKeyboardState(bool bOpen) override;

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	HWND getWin32Window() { return m_hWnd; }
#endif /* (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) */

protected:

	CMyEGLView();
	virtual ~CMyEGLView();

	bool initGlew();

	float _frameZoomFactor;
	HDC  m_hDC;
	HGLRC m_hRC;
	HWND m_hWnd;

	bool _captured;
};

NS_CC_END   // end of namespace   cocos2d

#endif  // end of __CMYEGLVIEW_H__
