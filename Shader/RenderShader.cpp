#include "RenderShader.h"  
#define STRINGIFY(A)  #A
const char* ccPositionTextureColor_frag_test = STRINGIFY(
\n#ifdef GL_ES\n
  precision lowp float;
\n#endif\n
 
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

void main()
{
	vec4 color = v_fragmentColor * texture2D(CC_Texture0, v_texCoord);
	float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));
	gl_FragColor = vec4(vec3(gray), color.a);
}
);

RenderShader* RenderShader::create()
{
	auto node = new RenderShader();
	if (node  && node->init())
	{
		node->autorelease();
		return node;
	}
	delete node;
	return nullptr;
}

bool RenderShader::init()
{
	if (m_glprogram)
	{
		m_glprogram->release();
		m_glprogram = nullptr;
	}

	CreateFrameBuffer();
  
	m_glprogram = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, ccPositionTextureColor_frag_test);
	m_glprogram->retain();

	return true;
}

RenderShader::RenderShader(void)
	:m_nFrameBuffer(0), m_nFrameBufferTexture(0), m_glprogram(nullptr)
{
	GLView* glView = Director::getInstance()->getOpenGLView();
	if (!glView)
	{
		return;
	}
	//屏幕大小 
	m_sRenderSize = glView->getFrameSize();
}

RenderShader::~RenderShader(void)
{
	if (m_glprogram)
	{
		m_glprogram->release();
		m_glprogram = nullptr;
	}
	if (m_nFrameBuffer)
	{
		glDeleteFramebuffers(1, &m_nFrameBuffer);
		m_nFrameBuffer = 0;
	}
	if (m_nFrameBufferTexture)
	{
		glDeleteTextures(1, &m_nFrameBufferTexture);
		m_nFrameBufferTexture = 0;
	}
}

bool RenderShader::LoadByteArray(const GLchar* vShaderByteArray, const GLchar* fShaderByteArray)
{
	if (m_glprogram)
	{
		m_glprogram->release();
		m_glprogram = nullptr;
	}

	this->CreateFrameBuffer();

	m_glprogram = GLProgram::createWithByteArrays(vShaderByteArray, fShaderByteArray);
	m_glprogram->retain();

	return true;
}


void RenderShader::onBegin()
{
	if (!m_nFrameBuffer)
	{
		return;
	}
	Director* director = Director::getInstance();
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

	//save old fbo and bind own
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_nOldFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_nFrameBuffer);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void RenderShader::onEnd()
{
	if (!m_nFrameBuffer)
	{
		return;
	}

	// restore viewport for render to direct fbo
	Director *director = Director::getInstance();
	// restore viewport
	director->setViewport();
	this->Render();

	glBindFramebuffer(GL_FRAMEBUFFER, m_nOldFBO);
	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}


void RenderShader::Render()
{
	if (!m_glprogram || !m_nFrameBufferTexture)
	{
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_nOldFBO);
	m_glprogram->use();
	m_glprogram->setUniformsForBuiltins();

	//GLuint glTexture = m_glprogram->getUniformLocationForName("CC_Texture0");
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, m_nFrameBufferTexture);
	//glUniform1i(glTexture, 0);
	GL::bindTexture2D(m_nFrameBufferTexture);
	Size sSize = Director::getInstance()->getVisibleSize();
	float x = 0;
	float y = 0;
	float w = sSize.width;
	float h = sSize.height;
	GLfloat vertices[] = {
		x, y + h,
		x + w, y + h,
		x, y,
		x + w, y
	};
	
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, ccRenderTextcord);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


	/*GL::bindVAO(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);*/
}



void RenderShader::CreateFrameBuffer()
{
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_nOldFBO);
	if (m_nFrameBuffer == 0)
	{
		glGenFramebuffers(1, &m_nFrameBuffer);

		if (m_nFrameBuffer == 0)
		{
			CCLOG("m_FilterFrameBuffer == 0");
			return;
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, m_nFrameBuffer);

	if (m_nFrameBufferTexture == 0)
	{
		glGenTextures(1, &m_nFrameBufferTexture);  
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_nFrameBufferTexture);

		GLsizei nWidth = m_sRenderSize.width;
		GLsizei nHeight = m_sRenderSize.height;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nWidth, nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
		{
			CCLOG("cocos2d: Texture2D: Error uploading compressed texture glError: 0x%04X", err);
			return;
		}

	}
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_nFrameBufferTexture, 0);


	GLenum error;
	if ((error = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
	{
		CCLOG("Failed to make complete framebuffer object 0x%X", error);
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_nOldFBO);
}

void RenderShader::begin()
{ 
	Director* director = Director::getInstance();
	CCASSERT(nullptr != director, "Director is null when seting matrix stack");
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

	_groupCommand.init(_globalZOrder);

	Renderer *renderer = Director::getInstance()->getRenderer();
	renderer->addCommand(&_groupCommand);
	renderer->pushGroup(_groupCommand.getRenderQueueID());

	_beginCommand.init(_globalZOrder);
	_beginCommand.func = CC_CALLBACK_0(RenderShader::onBegin, this);

	renderer->addCommand(&_beginCommand);
}

void RenderShader::end()
{
	_endCommand.init(_globalZOrder);
	_endCommand.func = CC_CALLBACK_0(RenderShader::onEnd, this);

	Director* director = Director::getInstance();
	CCASSERT(nullptr != director, "Director is null when seting matrix stack");

	Renderer *renderer = director->getRenderer();
	renderer->addCommand(&_endCommand);
	renderer->popGroup();

	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

}

void RenderShader::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	//Begin will create a render group using new render target
	begin();

	////clear screen
	//_clearCommand.init(_globalZOrder);
	//_clearCommand.func = CC_CALLBACK_0(RenderShader::onClear, this);
	//renderer->addCommand(&_clearCommand);

	//! make sure all children are drawn
	{
		auto it = sRenderChild.begin();
		while (it != sRenderChild.end())
		{
			if (Node* node = *it)
			{
				node->visit(renderer, transform, flags);
				node->release();
			}
			++it;
		}
		sRenderChild.clear();
	}
	//End will pop the current render group
	end();
}


void	RenderShader::push2Draw(Node* node)
{
	if (node && sRenderChild.find(node) == sRenderChild.end())
	{
		node->retain();
		sRenderChild.insert(node);
	}
	else
	{
		CCLOG(" push same!!!");
	}
}