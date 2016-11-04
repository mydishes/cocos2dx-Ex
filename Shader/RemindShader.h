//******************************************************************
// FILENAME:	RenderShader.h
// AUTHOR  :	Steaphan
// VERSION :	1.0.0
// DESCRIBE:	残影特效
// GitHub  :    https://github.com/Steaphans/cocos2dx-Ex
//******************************************************************
#ifndef _RemindShader_H__
#define _RemindShader_H__ 
#include "RenderShader.h"
USING_NS_CC;
#define REMIND_RENDER_COUNT 5  // 残影个数
class RemindShader :public RenderShader
{
public:
	static RemindShader* create();

	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
private:
	virtual void onBegin() override;

	//virtual void onClear();

	virtual void Render() override;

	virtual bool init() override;

	virtual void CreateFrameBuffer() override;

	RemindShader(void);

	virtual ~RemindShader(void);

	GLuint m_pTextureArr[REMIND_RENDER_COUNT]; 
	  
	int   m_nRenderFrq;			// 渲染间隔帧率

	int	  m_nRenderCount;		// 渲染间隔计数

	int   m_nCurTexutreIndex;	// 当前渲染的纹理ID

	Vec2  m_ptRenderPos[REMIND_RENDER_COUNT];

	Vec2  m_ptCurRenderPos;
};

#endif
