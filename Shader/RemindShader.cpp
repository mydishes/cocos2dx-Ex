#include "RemindShader.h"

#define STRINGIFY(A)  #A
const char* ccRemind_fsh = STRINGIFY(
	\n#ifdef GL_ES\n
	precision lowp float;
	\n#endif\n

	varying vec2 v_texCoord;
	uniform float u_alpha;

	void main()
	{
		vec4 pcolor = texture2D(CC_Texture0, v_texCoord) * u_alpha;
        gl_FragColor = pcolor;
	}
	);


RemindShader* RemindShader::create()
{
	auto node = new RemindShader();
	if (node  && node->init())
	{
		node->autorelease();
		return node;
	}
	delete node;
	return nullptr;
}

bool RemindShader::init()
{
	if (m_glprogram)
	{
		m_glprogram->release();
		m_glprogram = nullptr;
	}

	CreateFrameBuffer();
	m_glprogram = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, ccRemind_fsh);
	m_glprogram->retain();
	return true;
}

RemindShader::RemindShader(void)
: m_nCurTexutreIndex(0), m_nRenderFrq(10), m_nRenderCount(0)
{
	memset(m_pTextureArr, 0, REMIND_RENDER_COUNT);
}

RemindShader::~RemindShader(void)
{
	if (m_pTextureArr[0])
	{
		glDeleteTextures(REMIND_RENDER_COUNT, m_pTextureArr);
		memset(m_pTextureArr, 0, REMIND_RENDER_COUNT);
	}
}
 
void RemindShader::onBegin()
{
	if (!m_nFrameBuffer)
	{
		return;
	}	
	Director* director = Director::getInstance(); 
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
//    director->setProjection(director->getProjection());
//    GLView* glView = director->getOpenGLView();
//    if (!glView)
//    {
//    	return;
//    }
//    Size frameSize =glView->getFrameSize();
//    {
//    	// Calculate the adjustment ratios based on the old and new projections
//    	Size size = director->getWinSizeInPixels();
//    	float widthRatio = size.width / frameSize.width;
//    	float heightRatio = size.height / frameSize.height;
//    	//caculate the projections of size change
//    	Mat4 orthoMatrix;
//    	Mat4::createOrthographicOffCenter((float)-1.0 / widthRatio, (float)1.0 / widthRatio, (float)-1.0 / heightRatio, (float)1.0 / heightRatio, -1, 1, &orthoMatrix);
//    	director->multiplyMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION, orthoMatrix);
//    }
//    
//    //calculate viewport
//    {
//    	glViewport(0, 0, (GLsizei)(frameSize.width), (GLsizei)(frameSize.height));
//    }
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_nOldFBO);
	// 仅在计数为0才更新新的渲染
	if (m_nRenderCount == 0)
    {
		glBindFramebuffer(GL_FRAMEBUFFER, m_nFrameBuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pTextureArr[m_nCurTexutreIndex], 0);
		m_nCurTexutreIndex = m_nCurTexutreIndex + 1 < REMIND_RENDER_COUNT ? m_nCurTexutreIndex + 1 : 0;

 		glClearColor(0.0, 0.0, 0.0, 0.0);
 		glClear(GL_COLOR_BUFFER_BIT);
	}
}

void RemindShader::Render()
{
	if (!m_glprogram )
	{
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_nOldFBO);
	m_glprogram->use();
	m_glprogram->setUniformsForBuiltins();

	Size sSize = Director::getInstance()->getVisibleSize();
	float x = 0;
	float y = 0;
    float w = sSize.width;
	float h = sSize.height;

	// 除当前纹理外的纹理存储的都是拖影
	int nBeginIndex = m_nCurTexutreIndex;
	int nCurIndex = nBeginIndex;
	GLfloat falpha = 0.3f;
	float addf = 0.05f;

	// 渲染存在m_pTextureArr队列里的拖影纹理到主场景
	do
	{    
        Point delta = _position;// this->convertToWorldSpace(_position);
		GLfloat vertices[] = {
			x   - delta.x,y+ h - delta.y,
			x+w - delta.x,y+ h - delta.y,
			x - delta.x  , y - delta.y,
			x+w - delta.x, y - delta.y
		};
		//GLuint glTexture = m_glprogram->getUniformLocationForName("CC_Texture0");
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, m_pTextureArr[nCurIndex]);
		//glUniform1i(glTexture, 0);
		GL::bindTexture2D(m_pTextureArr[nCurIndex]);

		GLuint glAlpha = m_glprogram->getUniformLocationForName("u_alpha");
		glUniform1f(glAlpha, falpha);

		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, ccRenderTextcord);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		falpha += addf;
		addf += 0.025f;
		nCurIndex = ++nCurIndex<REMIND_RENDER_COUNT ? nCurIndex : 0; 
	} while (nCurIndex != nBeginIndex);
//
//	GL::bindVAO(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void RemindShader::CreateFrameBuffer()
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
	GLsizei nWidth = m_sRenderSize.width;
	GLsizei nHeight = m_sRenderSize.height;
	glBindFramebuffer(GL_FRAMEBUFFER, m_nFrameBuffer);
	if (m_pTextureArr[0] == 0)
	{
		glGenTextures(REMIND_RENDER_COUNT, m_pTextureArr);
		for (int i = 0; i < REMIND_RENDER_COUNT; i++)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_pTextureArr[i]);
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
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pTextureArr[i], 0);
			glClearColor(0.0, 0.0, 0.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT);
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, m_nOldFBO);
}
  

void RemindShader::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	if (sRenderChild.size() <= 0 )
	{
		if (m_nFrameBuffer)
		{
			// 清理所有缓存的纹理
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_nOldFBO);
			glBindFramebuffer(GL_FRAMEBUFFER, m_nFrameBuffer);
			for (int i = 0; i < REMIND_RENDER_COUNT; i++)
			{
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pTextureArr[i], 0);
				glClearColor(0.0, 0.0, 0.0, 0.0);
				glClear(GL_COLOR_BUFFER_BIT);
			}
			glBindFramebuffer(GL_FRAMEBUFFER, m_nOldFBO);
		}
		m_nRenderCount = m_nRenderFrq - 1;
		return;
	}
	//Begin will create a render group using new render target
	begin();

	if (++m_nRenderCount == m_nRenderFrq)
	{
		//! make sure all children are drawn 
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
		m_nRenderCount = 0;	// 0 提示需要渲染进去
	}
	sRenderChild.clear();
	//End will pop the current render group
	end();

}
