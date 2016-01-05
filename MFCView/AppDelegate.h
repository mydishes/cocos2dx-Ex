#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"

/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by Director.
*/
class  AppDelegate : private cocos2d::Application
{
public:
	AppDelegate();
	virtual ~AppDelegate();

	virtual void initGLContextAttrs();

	/**
	@brief    Implement Director and Scene init code here.
	@return true    Initialize success, app continue.
	@return false   Initialize failed, app terminate.
	*/
	virtual bool applicationDidFinishLaunching();

	/**
	@brief  The function be called when the application enter background
	@param  the pointer of the application
	*/
	virtual void applicationDidEnterBackground();

	/**
	@brief  The function be called when the application enter foreground
	@param  the pointer of the application
	*/
	virtual void applicationWillEnterForeground();
	
	//create view by MFC HWND
	virtual void CreateWnd(HWND hwnd);
	// run by MFC　time
	virtual void Run2();
	
	void Resize(int width, int height);
	
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	HWND m_hwnd;
};
#endif // _APP_DELEGATE_H_

