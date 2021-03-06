#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "Flash.h"
#include "baseRes.h"
#include "FileCommon.h"
#define ShowFrame false

#include "ChoseHero.h"
#include "LevelMenu.h"
#include "GameControler.h"
#include "GetItemLayer.h"
#include "ShopMenu.h"
#include "GameoverBtnMenu.h"
#include "MainMenu.h"

#define LevelOpenNum	18

CFlashControl* g_pHero = new CFlashControl;

int AppDelegate::s_LevelOpen[36]= {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int AppDelegate::s_LevelStar[36]= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int AppDelegate::s_LevelHero2[36]= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int AppDelegate::s_LevelHero3[36]= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int AppDelegate::s_LevelKey[36]= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int AppDelegate::s_BulletNum[18]= {0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//子弹数
int AppDelegate::s_WeaponLevel[18]	= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int AppDelegate::s_WeaponOwn[18]	= {0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int AppDelegate::s_WeaponUse[3]		=  {2,3,0};//正在使用的子弹，购买时子弹

int AppDelegate::s_Money		= 2000;
int AppDelegate::s_Medal = 0; //勋章
int AppDelegate::s_GrenadeNum	= 0;
int AppDelegate::s_VoiceOpen	= 1;
int AppDelegate::s_Healbox		= 3;
int AppDelegate::s_FirstLogin	= 0;
int AppDelegate::s_SkillEnergy[2]= {0, 0};
int AppDelegate::s_ShopOnSell[4] = {0,0,0,0};
int AppDelegate::s_KillMonster = 0;
long AppDelegate::s_imei	= 0;

int AppDelegate::s_OnSaleNum = 0;
int AppDelegate::s_OnSaleTimes = 0;
int AppDelegate::s_OnSaleDay = 0;
int AppDelegate::s_OnSaleHour = 0;
int AppDelegate::s_HeroType = 0;//英雄选择

int AppDelegate::s_Hero2 = 3;
int AppDelegate::s_Hero3 = 6;
int AppDelegate::s_Hero3Array[6] = {0,0,0,0,0,0,};

int AppDelegate::m_voice = 50;
int AppDelegate::m_canVoiceResume = 0;
int AppDelegate::m_Sell1 = 0;
int AppDelegate::m_Sell2 = 0;

using namespace CocosDenshion;
USING_NS_CC;

AppDelegate::AppDelegate()
{
	SimpleAudioEngine::sharedEngine()->preloadEffect("SoundEffect/zmanhua1.mp3");
	s_WeaponUse[0] = 2;
	s_WeaponUse[1] = 3;
	s_WeaponUse[2] = -1;
	s_WeaponOwn[2] = 1;
	s_WeaponOwn[3] = 1;
	s_BulletNum[2] = 100;
	s_BulletNum[3] = 100;
#ifdef All_Level_Open//全关开启
	for ( int i = 0; i < LevelLimit*6; i++ )
#else
	for ( int i = 0; i < 1; i++ )
#endif
	{
		s_LevelOpen[i] = 1;//每一关关卡是否开启
		s_LevelStar[i] = 0;//每一关获得星星数
	}
#ifdef Plat_Telecom_Demo
	s_Money = 10000;
	s_Medal = 3000;
#endif
}

AppDelegate::~AppDelegate(){
	SimpleAudioEngine::end();
}

void AppDelegate::SaveOnSale(){
	common::SaveData("_a", s_OnSaleNum);//s_Money
	common::SaveData("_b", s_OnSaleTimes);//s_GrenadeNum
	common::SaveData("_c", s_OnSaleDay);//s_VoiceOpen
	common::SaveData("_d", s_OnSaleHour);//s_Healbox
	SaveArray("y", s_ShopOnSell, 4);//s_LevelStar
}

void AppDelegate::LoadOnSale()
{
	common::LoadData("_a", s_OnSaleNum);//s_Money
	common::LoadData("_b", s_OnSaleTimes);//s_GrenadeNum
	common::LoadData("_c", s_OnSaleDay);//s_VoiceOpen
	common::LoadData("_d", s_OnSaleHour);//s_Healbox
	LoadArray("y", s_ShopOnSell, 4);//s_LevelStar
}

void AppDelegate::SaveMoney(){
	common::SaveData("a", s_Money);//s_Money
}

void AppDelegate::SaveMedal(){
	common::SaveData("b", s_Medal);//s_Medal
}

void AppDelegate::SaveStatus(){
	common::SaveData("a", s_Money);//s_Money
	common::SaveData("b", s_Medal);//s_Medal
	common::SaveData("c", s_GrenadeNum);//s_GrenadeNum
	common::SaveData("d", s_VoiceOpen);//s_VoiceOpen
	common::SaveData("e", s_Healbox);//s_Healbox
	SaveArray("o", s_SkillEnergy, 1);//s_SkillEnergy
	SaveArray("l", s_WeaponUse, 2);//s_WeaponUse
	common::SaveData("j", s_WeaponLevel, s_WeaponUse[0]);//s_WeaponLevel
	common::SaveData("k", s_WeaponOwn, s_WeaponUse[0]);//s_WeaponOwn
	common::SaveData("j", s_WeaponLevel, s_WeaponUse[1]);//s_WeaponLevel
	common::SaveData("k", s_WeaponOwn, s_WeaponUse[1]);//s_WeaponOwn
	SaveArray("i", s_BulletNum, 18);//s_BulletNum
	common::SaveData("z", s_KillMonster);//s_Healbox
	common::SaveData("ba", s_Hero2);//s_Healbox
	common::SaveData("bb", s_Hero3);//s_Healbox
	SaveArray("bc", s_Hero3Array, 6);//s_LevelStar
	common::SaveData("_s1", m_Sell1);//s_Money
	common::SaveData("_s2", m_Sell2);//s_Money
}

void AppDelegate::SaveBody(){
	common::SaveData("bb", s_Hero3);//s_Healbox
	SaveArray("bc", s_Hero3Array, 6);//s_LevelStar
}

void AppDelegate::SaveLevel( int _level ){
	common::SaveData("g", s_LevelOpen, _level);//s_LevelOpen
	common::SaveData("g", s_LevelOpen, _level+1);//s_LevelOpen
	common::SaveData("h", s_LevelStar, _level);//s_LevelStar
	common::SaveData("x", s_LevelKey, _level);//s_LevelStar
	common::SaveData("bd", s_LevelHero2, _level);//s_LevelOpen
	common::SaveData("be", s_LevelHero3, _level);//s_LevelStar
}

void AppDelegate::SaveWeapon()
{
	SaveArray("i", s_BulletNum, 18);//s_BulletNum
	SaveArray("j", s_WeaponLevel, 18);//s_WeaponLevel 
	SaveArray("k", s_WeaponOwn, 18);//s_WeaponOwn
}

void AppDelegate::SaveWeapon( int _index ){
	common::SaveData("j", s_WeaponLevel, _index);//s_WeaponLevel
	common::SaveData("k", s_WeaponOwn, _index);//s_WeaponOwn
	common::SaveData("a", s_Money);//s_Money
}

void AppDelegate::SaveBullet( int _index ){
	common::SaveData("i", s_BulletNum, _index);//s_LevelOpen
	common::SaveData("a", s_Money);//s_Money
}

void AppDelegate::SaveInit(){
	SaveArray("l", s_WeaponUse, 2);//s_WeaponUse
	AppDelegate::SaveStatus();
#ifdef All_Level_Open
	for ( int i = 0; i < 24; i++ )
#else
	for ( int i = 0; i < 1; i++ )
#endif
	{
		AppDelegate::SaveLevel(i);
	}
}

void AppDelegate::SaveAll(){
	common::SaveData("a", s_Money);//s_Money
	common::SaveData("b", s_Medal);//s_Medal
	common::SaveData("c", s_GrenadeNum);//s_GrenadeNum
	common::SaveData("d", s_VoiceOpen);//s_VoiceOpen
	common::SaveData("e", s_Healbox);//s_Healbox
	SaveArray("g", s_LevelOpen, 36);//s_LevelOpen
	SaveArray("h", s_LevelStar, 36);//s_LevelStar
	SaveArray("i", s_BulletNum, 18);//s_BulletNum
	SaveArray("j", s_WeaponLevel, 18);//s_WeaponLevel
	SaveArray("k", s_WeaponOwn, 18);//s_WeaponOwn
	SaveArray("l", s_WeaponUse, 3);//s_WeaponUse
	SaveArray("o", s_SkillEnergy, 2);//s_SkillEnergy

	SaveArray("x", s_LevelKey, LevelOpenNum);//s_LevelStar
	common::SaveData("z", s_KillMonster);//s_Healbox
	common::SaveData("aa", s_imei);//s_Healbox
	common::SaveData("_g", s_HeroType);//s_Healbox
	common::SaveData("ba", s_Hero2);//s_Healbox
	common::SaveData("bb", s_Hero3);//s_Healbox
	SaveArray("bc", s_Hero3Array, 6);//s_LevelStar
	SaveArray("bd", s_LevelHero2, LevelOpenNum);//s_LevelOpen
	SaveArray("be", s_LevelHero3, LevelOpenNum);//s_LevelStar
	common::SaveData("_s1", m_Sell1);//s_Money
	common::SaveData("_s2", m_Sell2);//s_Money
}

void AppDelegate::SaveGuide(){
	common::SaveData("f", s_FirstLogin);//s_FirstLogin
}

void AppDelegate::LoadAll(){
	common::LoadData("a", s_Money);
	common::LoadData("b", s_Medal);
	common::LoadData("c", s_GrenadeNum);
	common::LoadData("d", s_VoiceOpen);
	common::LoadData("e", s_Healbox);
	LoadArray("g", s_LevelOpen, 36);
	LoadArray("h", s_LevelStar, 36);
	LoadArray("i", s_BulletNum, 18);
	LoadArray("j", s_WeaponLevel, 18);
	LoadArray("k", s_WeaponOwn, 18);
	LoadArray("l", s_WeaponUse, 3);
	LoadArray("o", s_SkillEnergy, 1);
	LoadArray("x", s_LevelKey, LevelOpenNum);//s_LevelStar
	common::LoadData("z", s_KillMonster);//s_Healbox
	common::SaveData("aa", s_imei);//s_Healbox
	common::LoadData("_g", s_HeroType);//s_Healbox
	common::LoadData("ba", s_Hero2);//s_Healbox太阳之子
	common::LoadData("bb", s_Hero3);//s_Healbox
	LoadArray("bc", s_Hero3Array, 6);//s_LevelStar
	LoadArray("bd", s_LevelHero2, LevelOpenNum);//s_LevelOpen
	LoadArray("be", s_LevelHero3, LevelOpenNum);//s_LevelStar
	common::LoadData("_s1", m_Sell1);//s_Money
	common::LoadData("_s2", m_Sell2);//s_Money
}

bool AppDelegate::applicationDidFinishLaunching()
{
	CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    CCDirector *pDirector = CCDirector::sharedDirector();
	pDirector->setOpenGLView(eglView);

    // turn on display FPS
    pDirector->setDisplayStats(ShowFrame);
    pDirector->setAnimationInterval(1.0 / 60);

	CCSize frameSize = eglView->getFrameSize();
	eglView->setDesignResolutionSize(frameSize.width, frameSize.height, kResolutionNoBorder);

    // create a scene. it's an autorelease object
	common::LoadData("f", s_FirstLogin);
	CCScene* pScene = CMainMenu::StartMenu();
	pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
	//CCTextureCache::sharedTextureCache()->removeAllTextures();

	/*{
	CCDirector::sharedDirector()->pause();
	SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->pauseAllEffects();
	m_voice = SimpleAudioEngine::sharedEngine()->getEffectsVolume();
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(0);
	PayService::getInstance()->setCheckTrue();
	}*/

	//m_canVoiceResume = 0;
	//PayService::OnPause();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
	//CCTextureCache::sharedTextureCache()->reloadAllTextures();
	//CCDirector::sharedDirector()->resume();
	//if ( CGameControler::GetGameControler() )
	//	CGameControler::GetGameControler()->PauseGame();
	//m_canVoiceResume++;
	//PayService::OnResume();
	//SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	//SimpleAudioEngine::sharedEngine()->resumeAllEffects();
	//SimpleAudioEngine::sharedEngine()->setEffectsVolume(m_voice);
}

void AppDelegate::AudioBackResume()
{
	m_canVoiceResume++;
	if ( m_canVoiceResume >= 2 )
	{
		CCTextureCache::sharedTextureCache()->reloadAllTextures();
		m_canVoiceResume = 0;
		CCDirector::sharedDirector()->startAnimation();
		if ( CGameControler::GetGameControler() )
			CGameControler::GetGameControler()->PauseGame(1);
		else
		{
			SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
		}
		SimpleAudioEngine::sharedEngine()->resumeAllEffects();
		SimpleAudioEngine::sharedEngine()->setEffectsVolume(m_voice);
	}
}

void AppDelegate::AudioBackPause(){
	CCTextureCache::sharedTextureCache()->removeAllTextures();
	SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->pauseAllEffects();
	m_voice = SimpleAudioEngine::sharedEngine()->getEffectsVolume();
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(0);
	m_canVoiceResume = 0;
	CCDirector::sharedDirector()->stopAnimation();
}

void AppDelegate::ChangeScene(cocos2d::CCScene* _pScene, eTransition _transition, float _time)
{
	CCDirector::sharedDirector()->getRunningScene()->stopAllActions();
	CCDirector::sharedDirector()->getRunningScene()->pauseSchedulerAndActions();
	CCScene* pScene = NULL;
	switch ( _transition ){
	case etHorizontal:
		pScene = CCTransitionProgressHorizontal::create(_time, _pScene);
		break;
	case etVertical:
		pScene = CCTransitionProgressVertical::create(_time, _pScene);
		break;
	case etOutIn:
		pScene = CCTransitionProgressInOut::create(_time, _pScene);
		break;
	case etFade:
		pScene = CCTransitionFade::create(_time, _pScene);
		break;
	default:
		break;
	}
	if ( pScene ){
		CCDirector::sharedDirector()->replaceScene(_pScene);
	}
}

void AppDelegate::ChangeScene(cocos2d::CCScene* _pScene){
	if ( _pScene ){
		CCDirector::sharedDirector()->getRunningScene()->stopAllActions();
		CCDirector::sharedDirector()->getRunningScene()->pauseSchedulerAndActions();
		CCDirector::sharedDirector()->replaceScene(_pScene);
	}
}

void AppDelegate::AudioInit1(){
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("MS/Music/mainmenu/mainmenu.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("MS/Music/level1/1.mp3");
#ifndef GameTypeC
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("MS/Music/level1/2.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("MS/Music/level2/1.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("MS/Music/level2/2.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("MS/Music/mainmenu/ShopAchieve.mp3");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("MS/Music/mainmenu/LevelChose.mp3");	
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("MS2/moreIndex.mp3");
#endif
}

void AppDelegate::AudioInit2(){
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("MS/Music/level2/1.mp3");
}

void AppDelegate::AudioInit3(){}

void AppDelegate::AudioPlayBgm(const char* _path, bool _Repeat){
	if ( s_VoiceOpen ){
		SimpleAudioEngine::sharedEngine()->setEffectsVolume(100);
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(_path, _Repeat);
	}
}

void AppDelegate::AudioStopBgm(){
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->stopAllEffects();
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(0);
}

void AppDelegate::AudioStopB(){
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

void AppDelegate::AudioPause(){
	SimpleAudioEngine::sharedEngine()->pauseAllEffects();
	SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void AppDelegate::AudioResume(){
	if ( AppDelegate::s_VoiceOpen ){
		SimpleAudioEngine::sharedEngine()->resumeAllEffects();
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	}
}

int AppDelegate::AudioPlayEffect(const char* _path){
	int id = -1;
	if ( s_VoiceOpen ){
		id = SimpleAudioEngine::sharedEngine()->playEffect(_path);
	}
	return id;
}

void AppDelegate::AudioStopEffect( int _id ){}

int AppDelegate::WeaponIndex( int _num )
{
	switch ( _num )
	{
	case 3:
		return 0;
	case 4:
		return 1;
	case 5:
		return 2;
	case 7:
		return 3;
	case 8:
		return 4;
	case 9:
		return 5;
	case 10:
		return 6;
	case 11:
		return 7;
	case 12:
		return 8;
	case 16:
		return 9;
	case 17:
		return 10;
	case 2:
		return 20;
	case 6:
		return 21;
	case 13:
		return 22;
	case 14:
#ifdef Old_Version
		return 20;
#else
		return 23;
#endif
	case 15:
		return 30;
	}
	return -1;
}

int AppDelegate::WeaponSpeed( int _num )
{
	return g_iWeaponCooltime[_num] - g_iWeaponUpgradeS[_num]*AppDelegate::s_WeaponLevel[_num];
}

int AppDelegate::WeaponDamage( int _num )
{
	return g_iWeaponDamage[_num] + g_iWeaponUpgradeD[_num]*AppDelegate::s_WeaponLevel[_num];
}

void AppDelegate::DataInit()
{
	//PayService::getInstance();
	if ( AppDelegate::s_FirstLogin == 0 )
	{
		AppDelegate::SaveInit();
		char buffer[255];
		sprintf(buffer, "NP%.4d", abs(s_imei%10000));
		cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey("name", buffer);
		cocos2d::CCUserDefault::sharedUserDefault()->flush();
	}
}

void AppDelegate::DataInit2()
{
	//PayService::getInstance();
	if ( AppDelegate::s_FirstLogin == 0 ){
		AppDelegate::SaveInit();
		char buffer[255];
		sprintf(buffer, "NP%.4d", abs(s_imei%10000));
		cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey("name", buffer);
		cocos2d::CCUserDefault::sharedUserDefault()->flush();
	}

	else
		AppDelegate::LoadAll();
}

void AppDelegate::AudioSwitch()
{
	AppDelegate::s_VoiceOpen = 1 - AppDelegate::s_VoiceOpen;
	AudioSetVoice();
}

void AppDelegate::AudioSetVoice()
{
	if ( AppDelegate::s_VoiceOpen == 0 )
	{
		m_voice = SimpleAudioEngine::sharedEngine()->getEffectsVolume();
		SimpleAudioEngine::sharedEngine()->setEffectsVolume(0);
	}
	else
	{
		SimpleAudioEngine::sharedEngine()->setEffectsVolume(m_voice);
	}
}

void AppDelegate::GetPayContent( int _type )
{//计费内容
	if ( _type < 0 )
		return;
	if ( _type > 17 )
		return;
	switch (_type)
	{
	case 1:
		{
			AppDelegate::m_Sell1		= 1;
			AppDelegate::s_WeaponOwn[6] = 1;
			AppDelegate::s_WeaponOwn[9] = 1;
			if ( AppDelegate::s_HeroType == 0 )
			{
				AppDelegate::s_WeaponUse[0] = 6;
				AppDelegate::s_WeaponUse[1] = 9;
			}
			AppDelegate::s_BulletNum[9] += 100;
			if ( AppDelegate::s_BulletNum[9] > 999 )
				AppDelegate::s_BulletNum[9] = 999;
			AppDelegate::s_Healbox = 3;
			AppDelegate::s_GrenadeNum = 10;
			AppDelegate::s_Money += 1000;
			AppDelegate::SaveStatus();
			/*if ( CGameControler::GetGameControler() )
				CGameControler::GetGameControler()->GuideBuy();*/
		}
		break;
	case 2:
		{
			AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] += 10*g_iWeaponBulletOnce[AppDelegate::s_WeaponUse[1]];
			if ( AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] > 999 )
				AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] = 999;
			AppDelegate::m_Sell1 = 1;
			int iSkillCanUse = 5;
			if ( AppDelegate::s_SkillEnergy[0] < 100 )
			{
				iSkillCanUse = 4;
				AppDelegate::s_SkillEnergy[0] = 100;
			}
			AppDelegate::m_Sell2 = iSkillCanUse;
			AppDelegate::s_GrenadeNum = 10;
			AppDelegate::s_Healbox = 3;
			AppDelegate::s_Money += 2000;
			AppDelegate::SaveStatus();
			/*if ( CGameControler::GetGameControler() )
				CGameControler::GetGameControler()->Sell2Resume();*/
		}
		break;
	case 3:
		{
			AppDelegate::m_Sell1 = 1;
			AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] += 10*g_iWeaponBulletOnce[AppDelegate::s_WeaponUse[1]];
			if ( AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] > 999 )
				AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] = 999;
			AppDelegate::s_Healbox = 3;
			AppDelegate::s_GrenadeNum = 10;
			AppDelegate::s_Money += 1000;
			AppDelegate::SaveStatus();
			if ( CGameControler::GetGameControler() )
				CGameControler::GetGameControler()->Sell3Resume();
		}
		break;
	case 4:
		{
			AppDelegate::m_Sell1		= 1;
			AppDelegate::s_WeaponOwn[6] = 1;
			AppDelegate::s_WeaponOwn[9] = 1;
			if ( AppDelegate::s_HeroType == 0 )
			{
				AppDelegate::s_WeaponUse[0] = 6;
				AppDelegate::s_WeaponUse[1] = 9;
			}
			AppDelegate::s_BulletNum[9] += 100;
			if ( AppDelegate::s_BulletNum[9] > 999 )
				AppDelegate::s_BulletNum[9] = 999;
			AppDelegate::s_Healbox = 3;
			AppDelegate::s_GrenadeNum = 10;
			AppDelegate::s_Money += 1000;
			AppDelegate::SaveStatus();
			/*if ( ccbLevelMenu::s_pccbLevelMenu )
				ccbLevelMenu::s_pccbLevelMenu->Sell1Resume();*/
			if ( ccbGameoverBtnMenu::s_pccbGameoverBtnMenu )
				ccbGameoverBtnMenu::s_pccbGameoverBtnMenu->Sell1Resume();
		}
		break;
	case 5:{}break;
	case 6:
		{
			AppDelegate::s_Hero3 = 0;
			AppDelegate::s_WeaponOwn[15] = 1;
			if ( AppDelegate::s_WeaponOwn[8] == 0 )
			{
				AppDelegate::s_WeaponOwn[8] = 1;
				AppDelegate::s_BulletNum[8] = 100;
			}
			AppDelegate::s_Healbox = 3;
			AppDelegate::s_GrenadeNum = 10;
			AppDelegate::s_Money += 1000;
			AppDelegate::SaveStatus();
			AppDelegate::SaveWeapon(15);
			AppDelegate::SaveWeapon(8);
			AppDelegate::SaveBullet(8);
		}
		break;
	case 7:
		{
			AppDelegate::s_Hero2 = 0;
			AppDelegate::s_WeaponOwn[14] = 1;
			AppDelegate::s_WeaponOwn[17] = 1;
			AppDelegate::s_BulletNum[17] = 135;
			AppDelegate::s_LevelOpen[30] = 1;
			AppDelegate::SaveStatus();
			AppDelegate::SaveWeapon(14);
			AppDelegate::SaveWeapon(17);
			AppDelegate::SaveLevel(30);
		}
		break;
	case 8:
		{
			AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] += 3*g_iWeaponBulletOnce[AppDelegate::s_WeaponUse[1]];
			if ( AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] > 999 )
				AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] = 999;
			AppDelegate::s_Healbox = 3;
			AppDelegate::s_GrenadeNum += 5;
			if ( AppDelegate::s_GrenadeNum > 10 )
				AppDelegate::s_GrenadeNum = 10;
			AppDelegate::SaveStatus();
			if ( CGameControler::GetGameControler() )
				CGameControler::GetGameControler()->Sell4Resume();
		}
		break;
	case 9:
		{
			int iSkillCanUse = 5;
			if ( AppDelegate::s_SkillEnergy[0] < 100 )
			{
				iSkillCanUse = 4;
				AppDelegate::s_SkillEnergy[0] = 100;
			}
			AppDelegate::m_Sell2 = iSkillCanUse;
			AppDelegate::SaveStatus();
			/*if ( CGameControler::GetGameControler() )
				CGameControler::GetGameControler()->Sell5Resume();*/
		}
		break;
	case 10:
		{
			AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] += 10*g_iWeaponBulletOnce[AppDelegate::s_WeaponUse[1]];
			if ( AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] > 999 )
				AppDelegate::s_BulletNum[AppDelegate::s_WeaponUse[1]] = 999;
			AppDelegate::s_Healbox = 3;
			AppDelegate::s_GrenadeNum = 10;
			AppDelegate::SaveStatus();
			if ( CGameControler::GetGameControler() )
				CGameControler::GetGameControler()->Sell3Resume1();
		}
		break;
	case 11:{}		break;
	case 12:
		{
			AppDelegate::s_Medal += 100;
			AppDelegate::SaveMedal();
		}
		break;
	case 13:
		{
			AppDelegate::s_Medal += 200;
			AppDelegate::SaveMedal();
		}
		break;
	case 14:
		{
			AppDelegate::s_Medal += 2000;
			AppDelegate::SaveMedal();
		}
		break;
	case 15:
		{
			AppDelegate::s_Money += 1000;
			AppDelegate::SaveMoney();
		}
		break;
	case 16:
		{
			AppDelegate::s_Money += 2000;
			AppDelegate::SaveMoney();
		}
		break;
	case 17:
		{
			AppDelegate::s_Money += 20000;
			AppDelegate::SaveMoney();
		}
		break;
	}
}
//
//void AppDelegate::CanclePay( int _type ){}
