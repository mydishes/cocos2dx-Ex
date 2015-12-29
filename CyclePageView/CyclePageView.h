//******************************************************************
// 文件名:	CyclePageView.h
// 版  本:	1.0
// 描  述:	循环PageView
//******************************************************************
#ifndef __CyclePageView_H__
#define __CyclePageView_H__
#include "ui/UIPageView.h"
NS_CC_BEGIN

namespace ui {

//
class CCyclePageView : public PageView
{
public:
	static CCyclePageView* create(); 

	void SetPageSize(ssize_t size);
	 
	void scrollToPage(ssize_t idx) ; 

	void CopyProp(PageView* pPageView);
	 
	Layout* GetCurPage(){ return m_pCurLayout;}

	void addPageChangedListener(const std::function<void(Layout*, size_t)> callback){ m_callback = callback; }

protected:
	virtual bool onTouchBegan(Touch *touch, Event *unusedEvent) override;

	virtual bool scrollPages(Vec2 touchOffset) override;

	virtual void handleMoveLogic(Touch *touch) override;

	virtual void handleReleaseLogic(Touch *touch) override;
	
	void movePages(Vec2 offset);
	  
	void updateAllPagesSize();

	void updateAllPagesPosition();
	
	virtual void doLayout() override;
	
	void UpdateShowLayout(ssize_t nCurIdx, Layout* layout, bool isForceCallBack=false);

	CCyclePageView();
	~CCyclePageView();


	Layout* m_pLeftLayout;
	Layout* m_pRightLayout;
	Layout* m_pCurLayout;

	ssize_t m_nPageSize;

	std::function<void(Layout*,size_t)> m_callback;
};


}
NS_CC_END
#endif // __CyclePageView_H__
