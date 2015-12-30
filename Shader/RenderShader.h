//******************************************************************
// FILENAME:	RenderShader.h
// AUTHOR  :	Steaphan
// VERSION :	1.0.0
// DESCRIBE:	特效基类
//******************************************************************
#ifndef _RenderShader_H__
#define _RenderShader_H__ 
#include "cocos2d.h"
USING_NS_CC;

// GL纹理坐标组
static const  GLfloat ccRenderTextcord[8] ={
	0.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
};
class RenderShader : public Node
{
public:
	static RenderShader* create();

	virtual bool LoadByteArray(const GLchar* vShaderByteArray, const GLchar* fShaderByteArray);	//加载 shader文件

	void	begin();				// 用于设置绘制开始要调用的onBegin

	void	end();					// 用于设置绘制结束要调用的onEnd

	void	push2Draw(Node* node);  // 添加待绘制节点

	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;  // cocos2dx绘制回调

protected:
	virtual void onBegin();

	virtual void onEnd();

	//virtual void onClear();

	virtual void Render();				// 绘制实现


	virtual bool init();

	virtual void CreateFrameBuffer();	//创建FBO及texture

	RenderShader(void);

	virtual ~RenderShader(void);
 

	GLuint m_nFrameBuffer;			// 用于当前绘制的fbo

	GLuint m_nFrameBufferTexture;	// 用于绑定在fbo中的texture

	GLProgram* m_glprogram;			// GL绘制管理

	GLint m_nOldFBO;				// 原来的fbo

	
	CustomCommand _beginCommand;
	CustomCommand _endCommand;
	GroupCommand _groupCommand;

	std::set<Node*> sRenderChild;	// 需要绘制的节点

	Size	m_sRenderSize;
};

#endif