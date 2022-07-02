#pragma once

const static double MAP_SIZE_W = 50.0;//��Ԃ̉��̒���
const static double MAP_SIZE_H = 50.0;//��Ԃ̏c�̒���
const static int MAX_NUM_OF_AGENT = 1000;//�G�[�W�F���g�̋�ԓ��ő�l��
const static double MAX_TARGET_SPEED = 1.8;//�G�[�W�F���g�̎��R���s���x�̍ő�l[m/s]
const static double MIN_TARGET_SPEED = 0.6;//�G�[�W�F���g�̎��R���s���x�̍ő�l[m/s]
const static double VIEW = 10.0; //�G�[�W�F���g�̎���
const static double MASS = 80.0;//�G�[�W�F���g�̎���[kg]
const static double RADIUS = 0.25; //�G�[�W�F���g�̔��a[m]
const static double DELTA_T = 0.1;//�V�~�����[�V��������1�X�e�b�v�ӂ�̎���
const static double EXPECTED_INFLOW = 2.0;//�G�[�W�F���g��1�b�ӂ�̗����l�����Ғl[�l/s]

//Social Force Model�̗͂̌v�Z�ɕK�v�ȃp�����[�^
const static double COEF_OF_PROPULSION = 0.5;//���i�͌W��[s]
const static double COEF_INTERACTION = 2000.0; //�C���^���N�V������p[N]
const static double COEF_REPLUSION = 0.08; //������p[m]
const static double COEF_REPLUSION_FORCE = 120000.0; //�����͌W��[kg*s^-2]
const static double COEF_FRICTION_FORCE = 240000.0;//���C�͌W��[kg*m^-1*s^-1]