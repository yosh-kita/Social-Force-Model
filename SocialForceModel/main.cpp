#include "Dxlib.h"
#include "Manager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (DxLib_Init() == -1) { return -1; }//�c�w���C�u��������������

	SetDrawScreen(DX_SCREEN_BACK);//����ʂɕ`�ʂ���悤�ݒ�

	Manager manager;

	manager.Process();

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}