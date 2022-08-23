#include "WidgetHandler.h"
#include "Scaleform/WidgetMenu.h"

void WidgetHandler::LoadWidget(std::string _path, int32_t _x, int32_t _y)
{
    AddWidgetMenuTask([_path, _x, _y](WidgetMenu& a_menu) {
		a_menu.LoadWidget(_path, _x, _y);
	});
}

void WidgetHandler::LoadText(std::string _text, std::string _font, int32_t _size, int32_t _x, int32_t _y)
{
    AddWidgetMenuTask([_text, _font, _size, _x, _y](WidgetMenu& a_menu) {
		a_menu.LoadText(_text, _font, _size, _x, _y);
	});
}

void WidgetHandler::UnloadWidget(int32_t _id)
{
    AddWidgetMenuTask([_id](WidgetMenu& a_menu) {
		a_menu.UnloadWidget(_id);
	});
}

void WidgetHandler::UnloadText(int32_t _id)
{
    AddWidgetMenuTask([_id](WidgetMenu& a_menu) {
		a_menu.UnloadText(_id);
	});
}

void WidgetHandler::OpenWidgetMenu()
{
	auto msgQ = RE::UIMessageQueue::GetSingleton();
	if (msgQ) {
		msgQ->AddMessage(WidgetMenu::MenuName(), RE::UI_MESSAGE_TYPE::kShow, nullptr);
	}
}

void WidgetHandler::CloseWidgetMenu()
{
	auto msgQ = RE::UIMessageQueue::GetSingleton();
	if (msgQ) {
		msgQ->AddMessage(WidgetMenu::MenuName(), RE::UI_MESSAGE_TYPE::kHide, nullptr);
	}
}

void WidgetHandler::AddWidgetMenuTask(WidgetTasklet a_task)
{
	OpenWidgetMenu();
	Locker locker(_lock);
	_WidgetMenuTaskQueue.push_back(std::move(a_task));
}

void WidgetHandler::ProcessWidgetMenu(WidgetMenu& a_menu)
{
	if (!_WidgetMenuTaskQueue.empty()) {
		for (auto& task : _WidgetMenuTaskQueue) {
			task(a_menu);
		}
		_WidgetMenuTaskQueue.clear();
	}

	if (_refreshWidgetMenu) {
		a_menu.RefreshUI();
	}

	_refreshWidgetMenu = false;
}

void WidgetHandler::AddID(int32_t _id, eIDType _type)
{
    Locker locker(_lock);
    mIDHolder.push_back(std::make_pair(_id, _type));
}

void WidgetHandler::RemoveID(int32_t _id)
{
    Locker locker(_lock);
    for (int i = 0; i < mIDHolder.size(); i++) {
        if (mIDHolder[i].first == _id) {
            mIDHolder.erase(mIDHolder.begin() + i);
            break;
		}
	}
}

std::vector<std::pair<int32_t, WidgetHandler::eIDType>> WidgetHandler::GetIDList()
{
    Locker locker(_lock);
    return mIDHolder;
}

void WidgetHandler::SetText(int32_t _id, std::string _text)
{
	Locker locker(_lock);
    mText.push_back(std::make_tuple(_id, _text));
    RefreshWidgetMenu();
}

void WidgetHandler::ProcessText()
{
	Locker locker(_lock);
    mText.erase(mText.begin());
}

void WidgetHandler::SetSize(int32_t _id, int32_t _width, int32_t _height)
{
	Locker locker(_lock);
    mSize.push_back(std::make_tuple(_id, _width, _height));
    RefreshWidgetMenu();
}

void WidgetHandler::ProcessSize()
{
	Locker locker(_lock);
    mSize.erase(mSize.begin());
}

void WidgetHandler::SetPos(int32_t _id, int32_t _x, int32_t _y)
{
    Locker locker(_lock);
    mPos.push_back(std::make_tuple(_id, _x, _y));
    RefreshWidgetMenu();
}

void WidgetHandler::ProcessPos()
{
    Locker locker(_lock);
    mPos.erase(mPos.begin());
}

void WidgetHandler::SetAlpha(int32_t _id, int32_t _alpha)
{
    Locker locker(_lock);
    mAlpha.push_back(std::make_tuple(_id, _alpha));
    RefreshWidgetMenu();
}

void WidgetHandler::ProcessAlpha()
{
    Locker locker(_lock);
    mAlpha.erase(mAlpha.begin());
}

void WidgetHandler::Animate(int32_t _id)
{
    Locker locker(_lock);
    mAnimate.push_back(_id);
    RefreshWidgetMenu();
}

void WidgetHandler::ProcessAnimate()
{
    Locker locker(_lock);
    mAnimate.erase(mAnimate.begin());
}

void WidgetHandler::DissolveIn()
{
    Locker locker(_lock);
    mDissolveIn = true;
    RefreshWidgetMenu();
}

void WidgetHandler::ProcessDissolveIn()
{
    Locker locker(_lock);
    mDissolveIn = false;
}

void WidgetHandler::DissolveOut()
{
    Locker locker(_lock);
    mDissolveOut = true;
    RefreshWidgetMenu();
}

void WidgetHandler::ProcessDissolveOut()
{
    Locker locker(_lock);
    mDissolveOut = false;
}