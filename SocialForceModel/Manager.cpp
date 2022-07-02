#include "Manager.h"
#include "Vector.h"
#include "DxLib.h"
#include "Map.h"
#include "Agent.h"
#include "Object.h"
#include "Parameter.h"

Manager::Manager() {
	map = new Map(MAP_SIZE_W, MAP_SIZE_H);//��Ԃ̐���

	//��Ԃ̊O�ǂɏ�Q���𐶐�
	CreateObj(0.0, 0.0, 0.5, MAP_SIZE_H);//��ԍ��̕ǂ̐���
	CreateObj(MAP_SIZE_W- 0.5, 0.0, MAP_SIZE_W, MAP_SIZE_H);//��ԉE�̕ǂ̐���
	CreateObj(0.0, 0.0, MAP_SIZE_W, 0.5);//��ԏ�̕ǂ̐���
	CreateObj(0.0, MAP_SIZE_H - 0.5, MAP_SIZE_W, MAP_SIZE_H);//��ԉ��̕ǂ̐���
}

Manager::~Manager() {
	delete map;
	lAgent.clear();
	lObject.clear();
}

void Manager::Process() {
	double drawMagnification;
	int wSizeX, wSizeY;
	int originX, originY;//�`�ʎ��̌��_�̍��W


	GetDrawScreenSize(&wSizeX, &wSizeY);//��ʃT�C�Y�̊m�F
	drawMagnification = (wSizeY / map->getH() > wSizeX / map->getW()) ? (wSizeY - 200.0) / map->getH() : (wSizeX - 200.0) / map->getW();//�`�ʎ��̔{����ݒ肷��
										
	//�`�ʎ��̌��_�̍��W���v�Z(��ʂ̒��S�Ƌ�Ԃ̒��S����v����悤�ɂ���)
	originX = (wSizeX / 2) - static_cast<int>(map->getW() * drawMagnification / 2);
	originY = (wSizeY / 2) - static_cast<int>(map->getH() * drawMagnification / 2);

	//�G���^�[�L�[���������܂ŃV�~�����[�V���������s����
	while (CheckHitKey(KEY_INPUT_RETURN) == 0) {
		ProcessMessage();

		if (lAgent.size()<MAX_NUM_OF_AGENT) {
			CreateAgt();//�G�[�W�F���g�̐���
		}

		//�G�[�W�F���g�̈ړ�
		for (auto itr = lAgent.begin(); itr != lAgent.end();) {
			itr->CalDrivingForce();//���ȋ쓮�͂̌v�Z
			itr->CalRepulsiveForce(lAgent, lObject);//�˗͂̌v�Z
			itr->CalVelocity();//���x�̌v�Z
			itr->Move(lObject);//�ړ�

			//�ڕW�n�_�ɓ��������G�[�W�F���g�͍폜����
			if (itr->CheckArrived()) {
				itr = lAgent.erase(itr); 
			}
			else {
				itr++;
			}

		}

		ClearDrawScreen();//��ʂ̃N���A
		map->Draw(originX, originY, drawMagnification);//��Ԃ̕`��
		////��Q���̕`��
		for (auto itr = lObject.begin(); itr != lObject.end(); itr++) {
			itr->Draw(originX, originY, drawMagnification);
		}
		//�G�[�W�F���g�̕`��
		for (auto itr = lAgent.begin(); itr != lAgent.end(); itr++) {
			itr->Draw(originX, originY, drawMagnification);
		}
		ScreenFlip();//��ʂ̕\��

	}
}

//�G�[�W�F���g�̐���
void Manager::CreateAgt() {
	Agent* agent;
	Vector pos;
	int foo = 1000000;//�K���ɑ傫�Ȑ�����p�ӂ��AGetRand(�傫������)/�傫�������̌v�Z�ɂ���āA�^���I��0.0�`1.0�̎����̗����𐶐�������

	//�G�[�W�F���g��1�b�ӂ�̗����l�����Ғl�ɉ������m���ŁA�G�[�W�F���g�𐶐�������
	if (100.0 * GetRand(foo) / foo <= 100.0 * EXPECTED_INFLOW * DELTA_T) {
		agent = new Agent(lAgent);
		lAgent.push_back(*agent);
	}
}

//����̍��W(x1,y1)�A�E���̍��W(x2,y2)�̏�Q���̐���
void Manager::CreateObj(double x1, double y1, double x2, double y2) {
	Object* object;
	double x, y;//��Q���̒��S���W
	Vector posUpL, posBoR;//����ƉE���̈ʒu�x�N�g��

	//�e�������ʒu�x�N�g���̐����ɑ��
	posUpL.x = x1; posUpL.y = y1;
	posBoR.x = x2; posBoR.y = y2;

	//���S���W�̎Z�o
	x = (x2 - x1) / 2;
	y = (y2 - y1) / 2;

	object = new Object(x, y, &posUpL, &posBoR);
	lObject.push_back(*object);
}