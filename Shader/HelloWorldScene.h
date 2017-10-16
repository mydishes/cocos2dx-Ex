#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "RenderShader.h"
class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
private:
	RenderShader* m_pShader;
	Node* m_pNode;
};

#endif // __HELLOWORLD_SCENE_H__
