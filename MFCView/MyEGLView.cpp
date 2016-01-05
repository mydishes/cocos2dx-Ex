#include "MyEGLView.h"

NS_CC_BEGIN
////////////////////////////////////////////////////

struct keyCodeItem
{
	int glfwKeyCode;
	EventKeyboard::KeyCode keyCode;
};

static std::unordered_map<int, EventKeyboard::KeyCode> g_keyCodeMap;

static keyCodeItem g_keyCodeStructArray[] = {
	/* The unknown key */
	{ GLFW_KEY_UNKNOWN, EventKeyboard::KeyCode::KEY_NONE },

	/* Printable keys */
	{ GLFW_KEY_SPACE, EventKeyboard::KeyCode::KEY_SPACE },
	{ GLFW_KEY_APOSTROPHE, EventKeyboard::KeyCode::KEY_APOSTROPHE },
	{ GLFW_KEY_COMMA, EventKeyboard::KeyCode::KEY_COMMA },
	{ GLFW_KEY_MINUS, EventKeyboard::KeyCode::KEY_MINUS },
	{ GLFW_KEY_PERIOD, EventKeyboard::KeyCode::KEY_PERIOD },
	{ GLFW_KEY_SLASH, EventKeyboard::KeyCode::KEY_SLASH },
	{ GLFW_KEY_0, EventKeyboard::KeyCode::KEY_0 },
	{ GLFW_KEY_1, EventKeyboard::KeyCode::KEY_1 },
	{ GLFW_KEY_2, EventKeyboard::KeyCode::KEY_2 },
	{ GLFW_KEY_3, EventKeyboard::KeyCode::KEY_3 },
	{ GLFW_KEY_4, EventKeyboard::KeyCode::KEY_4 },
	{ GLFW_KEY_5, EventKeyboard::KeyCode::KEY_5 },
	{ GLFW_KEY_6, EventKeyboard::KeyCode::KEY_6 },
	{ GLFW_KEY_7, EventKeyboard::KeyCode::KEY_7 },
	{ GLFW_KEY_8, EventKeyboard::KeyCode::KEY_8 },
	{ GLFW_KEY_9, EventKeyboard::KeyCode::KEY_9 },
	{ GLFW_KEY_SEMICOLON, EventKeyboard::KeyCode::KEY_SEMICOLON },
	{ GLFW_KEY_EQUAL, EventKeyboard::KeyCode::KEY_EQUAL },
	{ GLFW_KEY_A, EventKeyboard::KeyCode::KEY_A },
	{ GLFW_KEY_B, EventKeyboard::KeyCode::KEY_B },
	{ GLFW_KEY_C, EventKeyboard::KeyCode::KEY_C },
	{ GLFW_KEY_D, EventKeyboard::KeyCode::KEY_D },
	{ GLFW_KEY_E, EventKeyboard::KeyCode::KEY_E },
	{ GLFW_KEY_F, EventKeyboard::KeyCode::KEY_F },
	{ GLFW_KEY_G, EventKeyboard::KeyCode::KEY_G },
	{ GLFW_KEY_H, EventKeyboard::KeyCode::KEY_H },
	{ GLFW_KEY_I, EventKeyboard::KeyCode::KEY_I },
	{ GLFW_KEY_J, EventKeyboard::KeyCode::KEY_J },
	{ GLFW_KEY_K, EventKeyboard::KeyCode::KEY_K },
	{ GLFW_KEY_L, EventKeyboard::KeyCode::KEY_L },
	{ GLFW_KEY_M, EventKeyboard::KeyCode::KEY_M },
	{ GLFW_KEY_N, EventKeyboard::KeyCode::KEY_N },
	{ GLFW_KEY_O, EventKeyboard::KeyCode::KEY_O },
	{ GLFW_KEY_P, EventKeyboard::KeyCode::KEY_P },
	{ GLFW_KEY_Q, EventKeyboard::KeyCode::KEY_Q },
	{ GLFW_KEY_R, EventKeyboard::KeyCode::KEY_R },
	{ GLFW_KEY_S, EventKeyboard::KeyCode::KEY_S },
	{ GLFW_KEY_T, EventKeyboard::KeyCode::KEY_T },
	{ GLFW_KEY_U, EventKeyboard::KeyCode::KEY_U },
	{ GLFW_KEY_V, EventKeyboard::KeyCode::KEY_V },
	{ GLFW_KEY_W, EventKeyboard::KeyCode::KEY_W },
	{ GLFW_KEY_X, EventKeyboard::KeyCode::KEY_X },
	{ GLFW_KEY_Y, EventKeyboard::KeyCode::KEY_Y },
	{ GLFW_KEY_Z, EventKeyboard::KeyCode::KEY_Z },
	{ GLFW_KEY_LEFT_BRACKET, EventKeyboard::KeyCode::KEY_LEFT_BRACKET },
	{ GLFW_KEY_BACKSLASH, EventKeyboard::KeyCode::KEY_BACK_SLASH },
	{ GLFW_KEY_RIGHT_BRACKET, EventKeyboard::KeyCode::KEY_RIGHT_BRACKET },
	{ GLFW_KEY_GRAVE_ACCENT, EventKeyboard::KeyCode::KEY_GRAVE },
	{ GLFW_KEY_WORLD_1, EventKeyboard::KeyCode::KEY_GRAVE },
	{ GLFW_KEY_WORLD_2, EventKeyboard::KeyCode::KEY_NONE },

	/* Function keys */
	{ GLFW_KEY_ESCAPE, EventKeyboard::KeyCode::KEY_ESCAPE },
	{ GLFW_KEY_ENTER, EventKeyboard::KeyCode::KEY_ENTER },
	{ GLFW_KEY_TAB, EventKeyboard::KeyCode::KEY_TAB },
	{ GLFW_KEY_BACKSPACE, EventKeyboard::KeyCode::KEY_BACKSPACE },
	{ GLFW_KEY_INSERT, EventKeyboard::KeyCode::KEY_INSERT },
	{ GLFW_KEY_DELETE, EventKeyboard::KeyCode::KEY_DELETE },
	{ GLFW_KEY_RIGHT, EventKeyboard::KeyCode::KEY_RIGHT_ARROW },
	{ GLFW_KEY_LEFT, EventKeyboard::KeyCode::KEY_LEFT_ARROW },
	{ GLFW_KEY_DOWN, EventKeyboard::KeyCode::KEY_DOWN_ARROW },
	{ GLFW_KEY_UP, EventKeyboard::KeyCode::KEY_UP_ARROW },
	{ GLFW_KEY_PAGE_UP, EventKeyboard::KeyCode::KEY_PG_UP },
	{ GLFW_KEY_PAGE_DOWN, EventKeyboard::KeyCode::KEY_PG_DOWN },
	{ GLFW_KEY_HOME, EventKeyboard::KeyCode::KEY_HOME },
	{ GLFW_KEY_END, EventKeyboard::KeyCode::KEY_END },
	{ GLFW_KEY_CAPS_LOCK, EventKeyboard::KeyCode::KEY_CAPS_LOCK },
	{ GLFW_KEY_SCROLL_LOCK, EventKeyboard::KeyCode::KEY_SCROLL_LOCK },
	{ GLFW_KEY_NUM_LOCK, EventKeyboard::KeyCode::KEY_NUM_LOCK },
	{ GLFW_KEY_PRINT_SCREEN, EventKeyboard::KeyCode::KEY_PRINT },
	{ GLFW_KEY_PAUSE, EventKeyboard::KeyCode::KEY_PAUSE },
	{ GLFW_KEY_F1, EventKeyboard::KeyCode::KEY_F1 },
	{ GLFW_KEY_F2, EventKeyboard::KeyCode::KEY_F2 },
	{ GLFW_KEY_F3, EventKeyboard::KeyCode::KEY_F3 },
	{ GLFW_KEY_F4, EventKeyboard::KeyCode::KEY_F4 },
	{ GLFW_KEY_F5, EventKeyboard::KeyCode::KEY_F5 },
	{ GLFW_KEY_F6, EventKeyboard::KeyCode::KEY_F6 },
	{ GLFW_KEY_F7, EventKeyboard::KeyCode::KEY_F7 },
	{ GLFW_KEY_F8, EventKeyboard::KeyCode::KEY_F8 },
	{ GLFW_KEY_F9, EventKeyboard::KeyCode::KEY_F9 },
	{ GLFW_KEY_F10, EventKeyboard::KeyCode::KEY_F10 },
	{ GLFW_KEY_F11, EventKeyboard::KeyCode::KEY_F11 },
	{ GLFW_KEY_F12, EventKeyboard::KeyCode::KEY_F12 },
	{ GLFW_KEY_F13, EventKeyboard::KeyCode::KEY_NONE },
	{ GLFW_KEY_F14, EventKeyboard::KeyCode::KEY_NONE },
	{ GLFW_KEY_F15, EventKeyboard::KeyCode::KEY_NONE },
	{ GLFW_KEY_F16, EventKeyboard::KeyCode::KEY_NONE },
	{ GLFW_KEY_F17, EventKeyboard::KeyCode::KEY_NONE },
	{ GLFW_KEY_F18, EventKeyboard::KeyCode::KEY_NONE },
	{ GLFW_KEY_F19, EventKeyboard::KeyCode::KEY_NONE },
	{ GLFW_KEY_F20, EventKeyboard::KeyCode::KEY_NONE },
	{ GLFW_KEY_F21, EventKeyboard::KeyCode::KEY_NONE },
	{ GLFW_KEY_F22, EventKeyboard::KeyCode::KEY_NONE },
	{ GLFW_KEY_F23, EventKeyboard::KeyCode::KEY_NONE },
	{ GLFW_KEY_F24, EventKeyboard::KeyCode::KEY_NONE },
	{ GLFW_KEY_F25, EventKeyboard::KeyCode::KEY_NONE },
	{ GLFW_KEY_KP_0, EventKeyboard::KeyCode::KEY_0 },
	{ GLFW_KEY_KP_1, EventKeyboard::KeyCode::KEY_1 },
	{ GLFW_KEY_KP_2, EventKeyboard::KeyCode::KEY_2 },
	{ GLFW_KEY_KP_3, EventKeyboard::KeyCode::KEY_3 },
	{ GLFW_KEY_KP_4, EventKeyboard::KeyCode::KEY_4 },
	{ GLFW_KEY_KP_5, EventKeyboard::KeyCode::KEY_5 },
	{ GLFW_KEY_KP_6, EventKeyboard::KeyCode::KEY_6 },
	{ GLFW_KEY_KP_7, EventKeyboard::KeyCode::KEY_7 },
	{ GLFW_KEY_KP_8, EventKeyboard::KeyCode::KEY_8 },
	{ GLFW_KEY_KP_9, EventKeyboard::KeyCode::KEY_9 },
	{ GLFW_KEY_KP_DECIMAL, EventKeyboard::KeyCode::KEY_PERIOD },
	{ GLFW_KEY_KP_DIVIDE, EventKeyboard::KeyCode::KEY_KP_DIVIDE },
	{ GLFW_KEY_KP_MULTIPLY, EventKeyboard::KeyCode::KEY_KP_MULTIPLY },
	{ GLFW_KEY_KP_SUBTRACT, EventKeyboard::KeyCode::KEY_KP_MINUS },
	{ GLFW_KEY_KP_ADD, EventKeyboard::KeyCode::KEY_KP_PLUS },
	{ GLFW_KEY_KP_ENTER, EventKeyboard::KeyCode::KEY_KP_ENTER },
	{ GLFW_KEY_KP_EQUAL, EventKeyboard::KeyCode::KEY_EQUAL },
	{ GLFW_KEY_LEFT_SHIFT, EventKeyboard::KeyCode::KEY_LEFT_SHIFT },
	{ GLFW_KEY_LEFT_CONTROL, EventKeyboard::KeyCode::KEY_LEFT_CTRL },
	{ GLFW_KEY_LEFT_ALT, EventKeyboard::KeyCode::KEY_LEFT_ALT },
	{ GLFW_KEY_LEFT_SUPER, EventKeyboard::KeyCode::KEY_HYPER },
	{ GLFW_KEY_RIGHT_SHIFT, EventKeyboard::KeyCode::KEY_RIGHT_SHIFT },
	{ GLFW_KEY_RIGHT_CONTROL, EventKeyboard::KeyCode::KEY_RIGHT_CTRL },
	{ GLFW_KEY_RIGHT_ALT, EventKeyboard::KeyCode::KEY_RIGHT_ALT },
	{ GLFW_KEY_RIGHT_SUPER, EventKeyboard::KeyCode::KEY_HYPER },
	{ GLFW_KEY_MENU, EventKeyboard::KeyCode::KEY_MENU },
	{ GLFW_KEY_LAST, EventKeyboard::KeyCode::KEY_NONE }
};


static void SetupPixelFormat(HDC hDC)
{
	int pixelFormat;

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // size
		1,                          // version
		PFD_SUPPORT_OPENGL |        // OpenGL window
		PFD_DRAW_TO_WINDOW |        // render to window
		PFD_DOUBLEBUFFER,           // support double-buffering
		PFD_TYPE_RGBA,              // color type
		32,                         // preferred color depth
		0, 0, 0, 0, 0, 0,           // color bits (ignored)
		0,                          // no alpha buffer
		0,                          // alpha bits (ignored)
		0,                          // no accumulation buffer
		0, 0, 0, 0,                 // accum bits (ignored)
		24,                         // depth buffer
		8,                          // no stencil buffer
		0,                          // no auxiliary buffers
		PFD_MAIN_PLANE,             // main layer
		0,                          // reserved
		0, 0, 0,                    // no layer, visible, damage masks
	};

	pixelFormat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, pixelFormat, &pfd);
}
//////////////////////////////////////////////////////////////////////////
// implement CMyEGLView
//////////////////////////////////////////////////////////////////////////

CMyEGLView::CMyEGLView() :m_hDC(NULL), m_hRC(NULL), m_hWnd(NULL), _frameZoomFactor(1.0f), _captured(false)
{ 
	g_keyCodeMap.clear();
	for (auto& item : g_keyCodeStructArray)
	{
		g_keyCodeMap[item.glfwKeyCode] = item.keyCode;
	}
}
CMyEGLView::~CMyEGLView()
{
}

CMyEGLView* CMyEGLView::createWithRect(const std::string& viewName, Rect rect, HWND  handle, float frameZoomFactor)
{
	auto ret = new (std::nothrow) CMyEGLView;
	if (ret && ret->initWithRect(viewName, rect, handle, frameZoomFactor)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}


void CMyEGLView::onGLFWError(int errorID, const char* errorDesc)
{
	//_glfwError = StringUtils::format("GLFWError #%d Happen, %s", errorID, errorDesc);
	CCLOGERROR("%s", _glfwError.c_str());
}

bool CMyEGLView::initWithRect(const std::string& viewName, Rect rect, HWND handle, float frameZoomFactor)
{
	m_hWnd = handle;
	setViewName(viewName);

	//gl  ContextCurrent
	m_hDC = GetDC(m_hWnd);
	SetupPixelFormat(m_hDC);
	m_hRC = wglCreateContext(m_hDC);
	wglMakeCurrent(m_hDC, m_hRC);
	setFrameSize(rect.size.width, rect.size.height);

	// check OpenGL version at first
	const GLubyte* glVersion = glGetString(GL_VERSION);

	if (utils::atof((const char*)glVersion) < 1.5)
	{
		char strComplain[256] = { 0 };
		sprintf(strComplain,
			"OpenGL 1.5 or higher is required (your version is %s). Please upgrade the driver of your video card.",
			glVersion);
		MessageBox(strComplain, "OpenGL version too old");
		return false;
	}

	initGlew();

	// Enable point size by default.
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

	return true;
}

LRESULT CMyEGLView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	BOOL bProcessed = FALSE;

	float	_mouseX = (short)LOWORD(lParam);
	float	_mouseY = (short)HIWORD(lParam);
	switch (message)
	{
	case WM_LBUTTONDOWN:
		if (MK_LBUTTON == wParam)
		{

			if (this->getViewPortRect().equals(Rect::ZERO) || this->getViewPortRect().containsPoint(Vec2(_mouseX, _mouseY)))
			{
				_captured = true;
				intptr_t id = 0;
				this->handleTouchesBegin(1, &id, &_mouseX, &_mouseY);
			}
			EventMouse event(EventMouse::MouseEventType::MOUSE_DOWN);
			//Because OpenGL and cocos2d-x uses different Y axis, we need to convert the coordinate here
			event.setCursorPosition(_mouseX, this->getViewPortRect().size.height - _mouseY);
			event.setMouseButton(0);
			Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
		}
		break;

	case WM_MOUSEMOVE:
		if (MK_LBUTTON == wParam && _captured)
		{
			POINT point = { (short)LOWORD(lParam), (short)HIWORD(lParam) };
			CCPoint pt(point.x, point.y);
			int id = 0;
			pt.x /= _frameZoomFactor;
			pt.y /= _frameZoomFactor;
			handleTouchesMove(1, &id, &pt.x, &pt.y);
			EventMouse event(EventMouse::MouseEventType::MOUSE_MOVE);
			//Because OpenGL and cocos2d-x uses different Y axis, we need to convert the coordinate here
			event.setCursorPosition(_mouseX, this->getViewPortRect().size.height - _mouseY);
			Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);


		}
		break;

	case WM_LBUTTONUP:
		if (_captured)
		{
			POINT point = { (short)LOWORD(lParam), (short)HIWORD(lParam) };
			CCPoint pt(point.x, point.y);
			int id = 0;
			pt.x /= _frameZoomFactor;
			pt.y /= _frameZoomFactor;
			handleTouchesEnd(1, &id, &pt.x, &pt.y);

			ReleaseCapture();
			_captured = false;
			EventMouse event(EventMouse::MouseEventType::MOUSE_UP);
			//Because OpenGL and cocos2d-x uses different Y axis, we need to convert the coordinate here
			event.setCursorPosition(_mouseX, this->getViewPortRect().size.height - _mouseY);
			event.setMouseButton(0);
			Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
		}
		break;
	case WM_SIZE:
		switch (wParam)
		{
		case SIZE_RESTORED:
			CCApplication::sharedApplication()->applicationWillEnterForeground();
			break;
		case SIZE_MINIMIZED:
			CCApplication::sharedApplication()->applicationDidEnterBackground();
			break;
		}
		break;
	case WM_KEYDOWN:
	{
					   EventKeyboard event(g_keyCodeMap[wParam], TRUE);
					   auto dispatcher = Director::getInstance()->getEventDispatcher();
					   dispatcher->dispatchEvent(&event);
	}

		break;
	case WM_KEYUP:
	{
					 EventKeyboard event(g_keyCodeMap[wParam], FALSE);
					 auto dispatcher = Director::getInstance()->getEventDispatcher();
					 dispatcher->dispatchEvent(&event);
	}
		break;
	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(m_hWnd, &ps);
		EndPaint(m_hWnd, &ps);
		break;
	case WM_CLOSE:
		CCDirector::getInstance()->end();
		break;
	case WM_DESTROY:
		end();
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(m_hWnd, message, wParam, lParam);
	}
	return 0;
}


bool CMyEGLView::isOpenGLReady()
{
	return nullptr != m_hDC;
}

void CMyEGLView::end()
{
	if (m_hDC != NULL && m_hRC != NULL)
	{
		// deselect rendering context and delete it
		wglMakeCurrent(m_hDC, NULL);
		wglDeleteContext(m_hRC);
		m_hDC = NULL;
		m_hRC = NULL;
	}
	m_hWnd = nullptr;
	// Release self. Otherwise, GLViewImpl could not be freed.
	release();
}

void CMyEGLView::swapBuffers()
{
	if (m_hDC != NULL)
	{
		::SwapBuffers(m_hDC);
	}
}
void CMyEGLView::setIMEKeyboardState(bool bOpen)
{

}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
static bool glew_dynamic_binding()
{
	const char *gl_extensions = (const char*)glGetString(GL_EXTENSIONS);

	// If the current opengl driver doesn't have framebuffers methods, check if an extension exists
	if (glGenFramebuffers == nullptr)
	{
		log("OpenGL: glGenFramebuffers is nullptr, try to detect an extension");
		if (strstr(gl_extensions, "ARB_framebuffer_object"))
		{
			log("OpenGL: ARB_framebuffer_object is supported");

			glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC)wglGetProcAddress("glIsRenderbuffer");
			glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)wglGetProcAddress("glBindRenderbuffer");
			glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)wglGetProcAddress("glDeleteRenderbuffers");
			glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)wglGetProcAddress("glGenRenderbuffers");
			glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)wglGetProcAddress("glRenderbufferStorage");
			glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC)wglGetProcAddress("glGetRenderbufferParameteriv");
			glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC)wglGetProcAddress("glIsFramebuffer");
			glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer");
			glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)wglGetProcAddress("glDeleteFramebuffers");
			glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)wglGetProcAddress("glGenFramebuffers");
			glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)wglGetProcAddress("glCheckFramebufferStatus");
			glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC)wglGetProcAddress("glFramebufferTexture1D");
			glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)wglGetProcAddress("glFramebufferTexture2D");
			glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC)wglGetProcAddress("glFramebufferTexture3D");
			glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)wglGetProcAddress("glFramebufferRenderbuffer");
			glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)wglGetProcAddress("glGetFramebufferAttachmentParameteriv");
			glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");
		}
		else
		if (strstr(gl_extensions, "EXT_framebuffer_object"))
		{
			log("OpenGL: EXT_framebuffer_object is supported");
			glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC)wglGetProcAddress("glIsRenderbufferEXT");
			glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)wglGetProcAddress("glBindRenderbufferEXT");
			glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)wglGetProcAddress("glDeleteRenderbuffersEXT");
			glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)wglGetProcAddress("glGenRenderbuffersEXT");
			glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)wglGetProcAddress("glRenderbufferStorageEXT");
			glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC)wglGetProcAddress("glGetRenderbufferParameterivEXT");
			glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC)wglGetProcAddress("glIsFramebufferEXT");
			glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebufferEXT");
			glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)wglGetProcAddress("glDeleteFramebuffersEXT");
			glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)wglGetProcAddress("glGenFramebuffersEXT");
			glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)wglGetProcAddress("glCheckFramebufferStatusEXT");
			glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC)wglGetProcAddress("glFramebufferTexture1DEXT");
			glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)wglGetProcAddress("glFramebufferTexture2DEXT");
			glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC)wglGetProcAddress("glFramebufferTexture3DEXT");
			glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)wglGetProcAddress("glFramebufferRenderbufferEXT");
			glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)wglGetProcAddress("glGetFramebufferAttachmentParameterivEXT");
			glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmapEXT");
		}
		else
		{
			log("OpenGL: No framebuffers extension is supported");
			log("OpenGL: Any call to Fbo will crash!");
			return false;
		}
	}
	return true;
}
#endif
// helper
bool CMyEGLView::initGlew()
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
	GLenum GlewInitResult = glewInit();
	if (GLEW_OK != GlewInitResult)
	{
		MessageBox((char *)glewGetErrorString(GlewInitResult), "OpenGL error");
		return false;
	}

	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
	{
		log("Ready for GLSL");
	}
	else
	{
		log("Not totally ready :(");
	}

	if (glewIsSupported("GL_VERSION_2_0"))
	{
		log("Ready for OpenGL 2.0");
	}
	else
	{
		log("OpenGL 2.0 not supported");
	}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if (glew_dynamic_binding() == false)
	{
		MessageBox("No OpenGL framebuffer support. Please upgrade the driver of your video card.", "OpenGL error");
		return false;
	}
#endif

#endif // (CC_TARGET_PLATFORM != CC_PLATFORM_MAC)

	return true;
}
NS_CC_END