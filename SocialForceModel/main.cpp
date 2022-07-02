#include "Dxlib.h"
#include "Manager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (DxLib_Init() == -1) { return -1; }//ＤＸライブラリ初期化処理

	SetDrawScreen(DX_SCREEN_BACK);//裏画面に描写するよう設定

	Manager manager;

	manager.Process();

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}