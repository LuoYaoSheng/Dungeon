#include "SignUpLayer.h"
#include "base/CCEventKeyboard.h"
#include "TcpClient.h"
#include "../../PacketType.h"
#include "LoginScene.h"

const std::string SignUpLayer::layerName = std::string("sign_up_layer");

// on "init" you need to initialize your instance
bool SignUpLayer::init()
{
	//////////////////////////////
	// 1. super init first
	if (!LayerColor::initWithColor(ccc4(127, 127, 80, 255)))
	{
		return false;
	}


	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	wchar_t wbuf[] = L"�г��� : >Ŭ��<";
	char buf[sizeof(wbuf)] = "";
	WideCharToMultiByte(CP_UTF8, 0, wbuf, -1, buf, sizeof(buf), NULL, NULL);
	Size nameBoxSize = Size(250.f, 40.f);
	mNameEditBox = EditBox::create(nameBoxSize, Scale9Sprite::create("extensions/name_edit.png"));
	mNameEditBox->setPosition(Vec2(visibleSize.width / 2.f , visibleSize.height * 3.f / 4.f));
	mNameEditBox->setAnchorPoint(ccp(0.5f, 0.5f));
	mNameEditBox->setFontName("���� ���");
	mNameEditBox->setFontSize(30);
	mNameEditBox->setFontColor(ccRED);
	mNameEditBox->setPlaceHolder(buf);
	mNameEditBox->setDelegate(this);
	addChild(mNameEditBox);

	// Sign Up Button Init
	{
		char buf[1024];
		WideCharToMultiByte(CP_UTF8, 0, L"<ȸ������>", -1, buf, sizeof(buf), NULL, NULL);
		mSignUpButtonItem = MenuItemFont::create(
			buf,
			this,
			menu_selector(SignUpLayer::onMenuItemClicked));
		mSignUpButtonItem->setAnchorPoint(Vec2(0.5f, 0.5f));
		mSignUpButtonItem->setFontNameObj("���� ���");
		mSignUpButtonItem->setFontSizeObj(60);
		mSignUpButtonItem->setColor(ccBLACK);
	}

	// Exit Button Init
	{
		char buf[1024];
		WideCharToMultiByte(CP_UTF8, 0, L"(Ȥ�� ���� �߸��Ǿ�����?)", -1, buf, sizeof(buf), NULL, NULL);
		mExitButtonItem = MenuItemFont::create(
			buf,
			this,
			menu_selector(SignUpLayer::onMenuItemClicked));
		mExitButtonItem->setAnchorPoint(Vec2(0.5f, 0.5f));
		mExitButtonItem->setFontNameObj("���� ���");
		mExitButtonItem->setFontSizeObj(20);
		mExitButtonItem->setColor(ccORANGE);
	}

	// create menu, it's an autorelease object
	auto menu = Menu::create(mSignUpButtonItem, mExitButtonItem, NULL);
	menu->setPosition(ccp(visibleSize.width / 2.f, visibleSize.height * 2.f / 4.f));
	this->addChild(menu, 1);
	menu->alignItemsVertically();

	this->setKeyboardEnabled(true);
	return true;
}

void SignUpLayer::onMenuItemClicked(Ref* sender)
{
	if (sender == mSignUpButtonItem)
	{
		char id[1024] = "";
		char pw[1024] = "";
		wchar_t name[1024] = L"";
		MultiByteToWideChar(CP_UTF8, 0, mNameEditBox->getText(), -1, name, sizeof(name));

		auto loginScene = dynamic_cast<LoginScene*>(cocos2d::Director::getInstance()->getRunningScene()->getChildByName(LoginScene::layerName));
		if (loginScene == nullptr)
			return;

		strcpy(id, loginScene->getID());
		strcpy(pw, loginScene->getPW());

		WCHAR buf[1024] = L"";
		TcpClient::getInstance()->signUpRequest(id, pw, name);
	}
	else if (sender == mExitButtonItem)
	{
		setVisible(false);
		init();
	}
}

void SignUpLayer::onSignUpResult(SignUpResult outPacket)
{
	//TODO
	if (outPacket.mSucced)
	{
		auto scheduler = cocos2d::Director::getInstance()->getScheduler();
		auto layer = cocos2d::Director::getInstance()->getRunningScene()->getChildByName(LoginScene::layerName);
		if (layer != nullptr)
			scheduler->performFunctionInCocosThread(CC_CALLBACK_0(LoginScene::tryLogin, dynamic_cast<LoginScene*>(layer)));
		setVisible(false);
		init();
	}
	else
	{
		switch (outPacket.mErrorType)
		{	
		case SRT_DupplicatedID:
		{
			MessageBox("�߸��� �����Դϴ�. �α��κ��� �ٽ� ���ּ���.", "!?");
			this->init();
			this->setVisible(false);
		}break;
		case SRT_DupplicatedName:
		{
			MessageBox("�̹� �ִ� �г����Դϴ�.\n", "�ߺ� �г���");
		}break;
		case SRT_WRONG_VALUE:
		{
			MessageBox("�߸��� ���� ������ �� �����ϴ�.\n�ٽ� Ȯ�����ּ���.", "�߸��� �� �Է�");
		}break;
		default:
		{
		}break;
		}
	}
	CCLOG("HI");
}

void SignUpLayer::editBoxEditingDidEnd(EditBox* editBox)
{
	wchar_t name[1024] = L"";
	MultiByteToWideChar(CP_UTF8, 0, mNameEditBox->getText(), -1, name, sizeof(name));
	
	if (wcslen(name) > MAX_NAME_LEN)
	{
		name[MAX_NAME_LEN] = NULL;
		char buf[1024]="";
		sprintf(buf, "�̸��� �ʹ� ����. \n%d���� �̳��� �����ּ���.", MAX_NAME_LEN);
		MessageBox(buf, "�� �̸� ������");
		
		memset(buf, 0, sizeof(buf));
		WideCharToMultiByte(CP_UTF8, 0, name, -1, buf, sizeof(buf), NULL, NULL);
		mNameEditBox->setText(buf);
	}
	}
