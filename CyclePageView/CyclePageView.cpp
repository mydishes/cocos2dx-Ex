#include "CyclePageView.h" 

NS_CC_BEGIN

namespace ui {


CCyclePageView* CCyclePageView::create()
{
	CCyclePageView* widget = new (std::nothrow) CCyclePageView();
	if (widget && widget->init())
	{
		widget->autorelease();
		return widget;
	}
	CC_SAFE_DELETE(widget);
	return nullptr;
}

CCyclePageView::CCyclePageView() :m_pLeftLayout(nullptr), m_pRightLayout(nullptr),
 m_pCurLayout(nullptr), m_callback(nullptr), m_nPageSize(0)
{
	_touchEnabled = false;
	_isAutoScrolling = false;
	_curPageIdx = 0;
}
CCyclePageView::~CCyclePageView()
{
}


void CCyclePageView::SetPageSize(ssize_t size)
{
	if (!m_pCurLayout)
	{
		m_pCurLayout = Layout::create();
		m_pCurLayout->setContentSize(getContentSize());
		addChild(m_pCurLayout);
	}
	if (!m_pRightLayout)
	{
		m_pRightLayout = Layout::create();
		m_pRightLayout->setContentSize(getContentSize());
		addChild(m_pRightLayout);
	}
	if (!m_pLeftLayout)
	{
		m_pLeftLayout = Layout::create();
		m_pLeftLayout->setContentSize(getContentSize());
		addChild(m_pLeftLayout);
	}
	m_nPageSize = size;
}

void CCyclePageView::scrollToPage(ssize_t idx)
{
	if (_direction == Direction::HORIZONTAL)
	{
		_autoScrollDistance = (_curPageIdx - idx)*getContentSize().width;
		if (m_pCurLayout)
		{
			_autoScrollDistance -= m_pCurLayout->getPositionX();
		}
		if (_autoScrollDistance > 0)
		{
			_autoScrollDirection = AutoScrollDirection::RIGHT;
			_touchMoveDirection = TouchDirection::RIGHT;
			UpdateShowLayout(_curPageIdx - 1, m_pRightLayout);
		}
		else
		{
			_autoScrollDirection = AutoScrollDirection::LEFT;
			_touchMoveDirection = TouchDirection::LEFT;
			UpdateShowLayout(_curPageIdx + 1, m_pLeftLayout);
		}
	}
	else if (_direction == Direction::VERTICAL)
	{
		_autoScrollDistance = (_curPageIdx - idx)*getContentSize().height;
		if (m_pCurLayout)
		{
			_autoScrollDistance -= m_pCurLayout->getPositionY();
		}
		if (_autoScrollDistance > 0)
		{
			_autoScrollDirection = AutoScrollDirection::DOWN;
			_touchMoveDirection = TouchDirection::DOWN;
			UpdateShowLayout(_curPageIdx - 1, m_pRightLayout);
		}
		else
		{
			_autoScrollDirection = AutoScrollDirection::UP;
			_touchMoveDirection = TouchDirection::UP;
			UpdateShowLayout(_curPageIdx + 1, m_pLeftLayout);
		}
	}
	_autoScrollSpeed = fabs(_autoScrollDistance) / 0.2f;
	if (_autoScrollDistance>0.000001f || _autoScrollDistance<-0.000001f)
	{
		_isAutoScrolling = true;
	}
	UpdateShowLayout(_curPageIdx, m_pCurLayout,true);
}
 
bool CCyclePageView::scrollPages(Vec2 touchOffset)
{
	if (m_nPageSize <= 0)
	{
		return false;
	}
	movePages(touchOffset);
	return true;
}

void CCyclePageView::movePages(Vec2 offset)
{
	if (!m_pCurLayout || !m_pRightLayout || !m_pLeftLayout)
	{
		return;
	}
	m_pCurLayout->setPosition(m_pCurLayout->getPosition() + offset);
	m_pRightLayout->setPosition(m_pRightLayout->getPosition() + offset);
	m_pLeftLayout->setPosition(m_pLeftLayout->getPosition() + offset);
	Size size = getContentSize();
	switch (_touchMoveDirection)
	{
	case TouchDirection::LEFT: // left 
		if (m_pCurLayout->getPositionX()<-size.width*0.75)
		{

			Layout* newRight = m_pLeftLayout;
			newRight->setPositionX(m_pRightLayout->getPositionX() + size.width);
			m_pLeftLayout = m_pCurLayout;
			m_pCurLayout = m_pRightLayout;
			m_pRightLayout = newRight;

			++_curPageIdx;
			UpdateShowLayout(_curPageIdx, m_pCurLayout,true);
		}

		break;
	case TouchDirection::RIGHT: // right 
		if (m_pCurLayout->getPositionX()>size.width*0.75)
		{
			Layout* newLeft = m_pRightLayout;
			newLeft->setPositionX(m_pLeftLayout->getPositionX() - size.width);
			m_pRightLayout = m_pCurLayout;
			m_pCurLayout = m_pLeftLayout;
			m_pLeftLayout = newLeft;
			
			--_curPageIdx;
			UpdateShowLayout(_curPageIdx, m_pCurLayout, true);
		}
		break;
	case TouchDirection::UP:
		if (m_pCurLayout->getPositionY()>size.width*0.75)
		{
			Layout* newLeft = m_pRightLayout;
			newLeft->setPositionX(m_pLeftLayout->getPositionX() - size.height);
			m_pRightLayout = m_pCurLayout;
			m_pCurLayout = m_pLeftLayout;
			m_pLeftLayout = newLeft;
			--_curPageIdx;
			UpdateShowLayout(_curPageIdx, m_pCurLayout, true);
		}
		break;
	case TouchDirection::DOWN:
		if (m_pCurLayout->getPositionY()<-size.height*0.75)
		{
			Layout* newRight = m_pLeftLayout;
			newRight->setPositionX(m_pRightLayout->getPositionX() + size.height);
			m_pLeftLayout = m_pCurLayout;
			m_pCurLayout = m_pRightLayout;
			m_pRightLayout = newRight;
			++_curPageIdx;
			UpdateShowLayout(_curPageIdx, m_pCurLayout, true);
		}
		break;
	default:
		break;
	}
	if (_curPageIdx<0)
	{
		_curPageIdx = m_nPageSize-1;
	}
	if (_curPageIdx >= m_nPageSize)
	{
		_curPageIdx = 0;
	}
}

void CCyclePageView::doLayout()
{
	if (!_doLayoutDirty)
	{
		return;
	}

	updateAllPagesPosition();
	updateAllPagesSize();
	//updateBoundaryPages();
	 
	_doLayoutDirty = false;
}
void CCyclePageView::updateAllPagesSize()
{
	Size selfSize = getContentSize();
	if (!m_pCurLayout || !m_pRightLayout || !m_pLeftLayout)
	{
		return;
	}
	m_pCurLayout->setContentSize(selfSize);
	m_pRightLayout->setContentSize(selfSize);
	m_pLeftLayout->setContentSize(selfSize);
}

void CCyclePageView::updateAllPagesPosition()
{  
	// If the layout is dirty, don't trigger auto scroll
	_isAutoScrolling = false;

	if (!m_pCurLayout || !m_pRightLayout || !m_pLeftLayout)
	{
		return;
	}
	Size size = getContentSize();
	m_pCurLayout->setPosition(Vec2::ZERO);
	if (_direction == Direction::HORIZONTAL)
	{
		m_pRightLayout->setPositionX(size.width);
		m_pLeftLayout->setPositionX(-size.width);
	}
	else if (_direction == Direction::VERTICAL)
	{
		m_pRightLayout->setPositionY(size.height);
		m_pLeftLayout->setPositionY(-size.height);
	} 
}

void CCyclePageView::CopyProp(PageView* pPageView)
{
	if (pPageView)
	{
		this->copyProperties(pPageView);
		this->copyClonedWidgetChildren(pPageView);
		this->copySpecialProperties(pPageView);
	}
}
bool CCyclePageView::onTouchBegan(Touch *touch, Event *unusedEvent)
{
	return  PageView::onTouchBegan(touch, unusedEvent);
}

void CCyclePageView::handleMoveLogic(Touch *touch)
{
	PageView::handleMoveLogic(touch);
	UpdateShowLayout(_curPageIdx, m_pCurLayout);
	switch (_touchMoveDirection)
	{
	case TouchDirection::LEFT: // left 
		UpdateShowLayout(_curPageIdx+1, m_pRightLayout);
		break;
	case TouchDirection::RIGHT: // right 
		UpdateShowLayout(_curPageIdx- 1, m_pLeftLayout);
		break;
	case TouchDirection::UP:
		UpdateShowLayout(_curPageIdx - 1, m_pLeftLayout);
		break;
	case TouchDirection::DOWN:
		UpdateShowLayout(_curPageIdx + 1, m_pRightLayout);
		break;
	default:
		break;
	}
}
void CCyclePageView::UpdateShowLayout(ssize_t nCurIdx, Layout* layout,bool isForceCallBack)
{
	if (nCurIdx<0)
	{
		nCurIdx = m_nPageSize - 1;
	}
	if (nCurIdx >= m_nPageSize)
	{
		nCurIdx = 0;
	}
	if (layout && (isForceCallBack || layout->getTag() != nCurIdx))
	{ 
		layout->setTag(nCurIdx);
		if (m_callback)
		{
			m_callback(layout,nCurIdx);
		}
	}
}

void CCyclePageView::handleReleaseLogic(Touch *touch)
{
	if (m_nPageSize <= 0)
    {
        return;
    }
	if (m_pCurLayout)
    {

	Vec2 curPagePos = m_pCurLayout->getPosition();
	auto contentSize = getContentSize();

	float moveBoundray = 0.0f;
	float scrollDistance;
	if (_direction == Direction::HORIZONTAL)
	{
		curPagePos.y = 0;
		moveBoundray = curPagePos.x;
		scrollDistance = contentSize.width / 2.0;
	}
	else if (_direction == Direction::VERTICAL)
	{
		curPagePos.x = 0;
		moveBoundray = curPagePos.y;
		scrollDistance = contentSize.height / 2.0;
	}

	if (!_usingCustomScrollThreshold)
	{
		_customScrollThreshold = scrollDistance;
	}
	float boundary = _customScrollThreshold;

	if (_direction == Direction::HORIZONTAL)
	{
		if (moveBoundray <= -boundary)
		{
			scrollToPage(_curPageIdx + 1);
		}
		else if (moveBoundray >= boundary)
		{
			scrollToPage(_curPageIdx - 1);
		}
		else
		{
			scrollToPage(_curPageIdx);
		}
	}
	else if (_direction == Direction::VERTICAL)
	{
		if (moveBoundray >= boundary)
		{
			scrollToPage(_curPageIdx + 1);
		}
		else if (moveBoundray <= -boundary)
		{
			scrollToPage(_curPageIdx - 1);
		}
		else
		{
			scrollToPage(_curPageIdx);
		}
	}
    }
}
}
NS_CC_END
