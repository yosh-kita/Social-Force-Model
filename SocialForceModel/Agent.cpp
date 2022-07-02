#include "Agent.h"
#include "Dxlib.h"
#include "Vector.h"
#include "Parameter.h"
#include "Object.h"
#define _USE_MATH_DEFINES
#include <math.h>

int Agent::n = 0;

//�G�[�W�F���g�̐����n�_�E�ړ��ڕW�n�_�͋�Ԃ�4���t�߂ɐݒ肷�邽�߁A�e����񋓌^�Œ�`
enum class Direction {
	nW = 0,
	nE = 1,
	sW = 2,
	sE = 3
};

Agent::Agent(std::list<Agent>& lAgent) {
	int foo = 1000000;//�K���ɑ傫�Ȑ�����p�ӂ��AGetRand(�傫������)/�傫�������̌v�Z�ɂ���āA�^���I��0.0�`1.0�̎����̗����𐶐�������
	Direction start, goal;
	Vector vec;

	vel = new Vector;
	goalPos = new Vector;
	drivingForce = new Vector;
	repulsiveForce = new Vector;
	targetSpeed = MIN_TARGET_SPEED + ((MAX_TARGET_SPEED - MIN_TARGET_SPEED) * GetRand(foo) / foo);//���s���x���ő�l�`�ŏ��l�͈̔͂Ń����_���ɐݒ肷��
	id = n;//�G�[�W�F���g�̎��ʔԍ��͐������ɐݒ�
	n++;

	start = GetRandDir();//�G�[�W�F���g�̐����n�_�������_���Ɍ��肷��

	//�ړ��ڕW�n�_���A�����n�_�Ɣ��Ȃ��悤�Ƀ����_���Ɍ��肷��
	do {
		goal = GetRandDir();
	} while (goal == start);

	vec = GetPosDir(start);//�����n�_�����̋�Ԃ̋��̍��W���擾
	ShiftPos(start, vec, 1.0 * GetRand(foo) / foo);//�����n�_���}1.0 m�͈̔͂ł��炷
	
	while (CheckExistAgt(vec, 1.0, lAgent)) {
		ShiftPos(start, vec, 1.0);//�����n�_�t�߁i���a1.0m�͈̔́j�ɃG�[�W�F���g�����݂���ꍇ�A�����n�_�����炵�āA�G�[�W�F���g�̐������̏Փ˂������
	}
	

	SetPos(vec.x, vec.y);

	vec = GetPosDir(goal);
	ShiftPos(goal, vec, 1.0 * GetRand(foo) / foo);//�ړ��ڕW�n�_���}1.0 m�͈̔͂ł��炷
	SetGoalPos(vec.x, vec.y);

	graphHandle = LoadGraph("Graphics/Agt.png");
}

Agent::~Agent() {
	delete vel;
	delete goalPos;
	delete drivingForce;
	delete repulsiveForce;
}

//���ȋ쓮�͂̌v�Z
void Agent::CalDrivingForce() {
	Vector dirGoal;//�ړI�n�ւ̕����x�N�g��

	//���ݒn����ړI�n�ւ̒P�ʃx�N�g�����Z�o
	dirGoal = *goalPos - *pos;
	dirGoal.Normalize();

	drivingForce->ZeroVec();
	*drivingForce = (dirGoal * targetSpeed - *vel) / COEF_OF_PROPULSION;
}

//�˗͂̌v�Z
void Agent::CalRepulsiveForce(std::list<Agent>& lAgent, std::list<Object>& lObject) {
	repulsiveForce->ZeroVec();

	//���͂̃G�[�W�F���g����󂯂�˗͂̌v�Z
	for (auto itr = lAgent.begin(); itr != lAgent.end(); itr++) {
		if (id != itr->id) {
			*repulsiveForce += CalOneAgtReplusive(*itr, RADIUS, RADIUS);
		}
	}

	//���͂̏�Q������󂯂�˗͂̌v�Z
	for (auto itr = lObject.begin(); itr != lObject.end(); itr++) {
		*repulsiveForce += CalOneObjReplusive(*itr, RADIUS);
	}
}

//1�G�[�W�F���g����󂯂�˗͂̌v�Z
//������ri�͎��g�̔��a�Arj�͑���G�[�W�F���g�̔��a�AdeltaVt�͑������ɂ������g�̑��Α��x�x�N�g��
Vector Agent::CalOneAgtReplusive(Agent& agent, double ri, double rj) {
	double coef, d, cosTheta, deltaVt;
	Vector deltaV, replusive, n, t;
	
	replusive.ZeroVec();
	n.ZeroVec();
	t.ZeroVec();

	deltaV = *vel - *agent.vel;
	n = *pos - agent.GetPos();//�˗͂�^����G�[�W�F���g����A�˗͂��󂯂�G�[�W�F���g�ւ̑��Έʒu�x�N�g�����Z�o
	d = n.AbsValue();//�G�[�W�F���g�Ԃ̋����𑪒�

	if (d > VIEW) {
		return replusive;//���E�O�Ȃ�[���x�N�g����Ԃ�
	}

	n.Normalize();//�˗͂�^����G�[�W�F���g����A�˗͂��󂯂�G�[�W�F���g�ւ̒P�ʕ����x�N�g�����Z�o
	t = n;
	t.Rotate(M_PI / 2.0);//�˗͂�^����G�[�W�F���g����A�˗͂��󂯂�G�[�W�F���g�̕����֐����ȒP�ʃx�N�g�����Z�o

	//�������ݍ�p�͍��i�G�[�W�F���g�Ԃɏ�ɓ��������́j�̌v�Z
	coef = COEF_INTERACTION * exp((ri + rj - d) / COEF_REPLUSION);
	replusive += n * coef;

	if (ri + rj - d > 0) {
		//���̗͍��i�G�[�W�F���g�̏Փˎ��ɍ�p����B�i�s�����̋t�֍�p����́j�̌v�Z
		coef = COEF_REPLUSION_FORCE * (ri + rj - d);
		replusive += n * coef;

		//���C�͍��i�G�[�W�F���g�̏Փˎ��ɍ�p����B�ڐ������ɍ�p����́j�̌v�Z
		if (deltaV.AbsValue() > 0) {
			cosTheta = Vector::InnerProduct(t, deltaV) / deltaV.AbsValue();//���ς�p���āA�G�[�W�F���g�̐ڐ������x�N�g���Ƒ��Α��x�x�N�g���������p�̃R�T�C�������߂�
			deltaVt = deltaV.AbsValue() * cosTheta;//�G�[�W�F���g�̑��Α��x�x�N�g���̐ڐ����������̑傫�������߂�
			coef = COEF_FRICTION_FORCE * (ri + rj - d) * deltaVt;
			replusive += t * coef;
		}
	}

	return replusive;
}

//��Q������󂯂�˗͂̌v�Z ������ri�͎��g�̔��a
Vector Agent::CalOneObjReplusive(Object& object, double ri) {
	double coef, d; 
	double minD = 1000000.0;
	Vector replusive, minN, t;
	Vector n[4];

	//��Q���̊O���̊e�ӂɑ΂��āA�G�[�W�F���g�ւ̍ŒZ�����x�N�g�����Z�o����
	n[0] = posLineToPoint(pos->x, pos->y, object.GetPosUL().x, object.GetPosUL().y, object.GetPosBR().x, object.GetPosUL().y);//��̕�
	n[1] = posLineToPoint(pos->x, pos->y, object.GetPosUL().x, object.GetPosBR().y, object.GetPosBR().x, object.GetPosBR().y);//���̕�
	n[2] = posLineToPoint(pos->x, pos->y, object.GetPosUL().x, object.GetPosUL().y, object.GetPosUL().x, object.GetPosBR().y);//���̕�
	n[3] = posLineToPoint(pos->x, pos->y, object.GetPosBR().x, object.GetPosUL().y, object.GetPosBR().x, object.GetPosBR().y);//�E�̕�

	//�ŒZ�ƂȂ�x�N�g���Ƌ����̎Z�o
	for (int i = 0; i < 4; i++) {
		d = n[i].AbsValue();
		if (d < minD) {
			minN = n[i];
			minD = d;
		}
	}

	if (minD > VIEW) {
		return replusive;//���E�O�Ȃ�[���x�N�g����Ԃ�
	}
	
	minN.Normalize();//��Q������G�[�W�F���g�ւ̃x�N�g����P�ʕ����x�N�g����
	t = minN;
	t.Rotate(M_PI / 2.0);//n�ɐ����ȒP�ʃx�N�g�����Z�o
	
	//�������ݍ�p�͍��i��ɓ��������́j�̌v�Z
	coef = COEF_INTERACTION * exp((ri - minD) / COEF_REPLUSION);
	replusive += minN * coef;
	
	if (ri - minD > 0) {
		//���̗͍��̌v�Z
		coef = COEF_REPLUSION_FORCE * (ri- minD);
		replusive += minN * coef;
		
		//���C�͍��̌v�Z
		coef = COEF_FRICTION_FORCE * (ri - minD) * Vector::InnerProduct(*vel, t);
		replusive += t * coef;
	}
	
	return replusive;
}

//���x�x�N�g���̌v�Z
void Agent::CalVelocity() {
	Vector acceleration;

	acceleration = *drivingForce + (*repulsiveForce / MASS);
	*vel += acceleration * DELTA_T;
	//�ڕW���x�𒴂��Ȃ��悤�ɂ���
	if (vel->AbsValue() > targetSpeed) {
		vel->Normalize();
		vel->x *= targetSpeed;
		vel->y *= targetSpeed;
	}
}

//�G�[�W�F���g�̈ړ�
void Agent::Move(std::list<Object>& lObject) {
	*pos += *vel * DELTA_T;
}

//�ړI�n�ւ̓����̔���֐�
bool Agent::CheckArrived() {
	if (fabs(pos->x - goalPos->x) < 0.5 && fabs(pos->y - goalPos->y) < 0.5) {
		return true;
	}
	else {
		return false;
	}

}

//�����_����4����Ԃ��֐�
Direction Agent::GetRandDir() {
	int rnd = GetRand(3);

	switch (rnd) {
	case 0:
		return Direction::nW;
		break;
	case 1:
		return Direction::nE;
		break;
	case 2:
		return Direction::sW;
		break;
	default:
		return Direction::sE;
		break;
	}
}

//4���ɓ�����ʒu�x�N�g����Ԃ��֐�
Vector Agent::GetPosDir(Direction dir) {
	Vector vec;

	switch (dir) {
	case Direction::nW:
		vec.x = 1.0; vec.y = 1.0;
		break;
	case Direction::nE:
		vec.x = MAP_SIZE_W - 1.0; vec.y = 1.0;
		break;
	case Direction::sW:
		vec.x = 1.0; vec.y = MAP_SIZE_H - 1.0;
		break;
	default:
		vec.x = MAP_SIZE_W - 1.0; vec.y = MAP_SIZE_H - 1.0;
		break;
	}

	return vec;
}

//�G�[�W�F���g�ʒu�����炷�֐�
void Agent::ShiftPos(Direction dir, Vector& pos, double l) {
	int foo = 1000000;//�K���ɑ傫�Ȑ�����p�ӂ��AGetRand(�傫������)/�傫�������̌v�Z�ɂ���āA�^���I��0.0�`1.0�̎����̗����𐶐�������
	double theta = M_PI / 2.0 * GetRand(foo) / foo;//x�����玞�v���������0�`90�x�̃����_���Ȋp�x

	//�ʒu�x�N�g�����ǂ̏ی��̕������ɉ����āA�����ʒu�����炷�����i�p�x�j�����肷��
	switch (dir) {
	case Direction::nW:
		theta += 0.0;//�ʒu�x�N�g������Ԃ̍��������ɑ��݂���Ȃ�A��4�ی������ɂ��炷
		break;
	case Direction::nE:
		theta += M_PI / 2;
		break;
	case Direction::sW:
		theta += 3 * M_PI / 2;
		break;
	default:
		theta += M_PI;
		break;
	}

	pos.x += l * cos(theta);
	pos.y += l * sin(theta);
}

//�ڕW�n�_�̐ݒ�
void Agent::SetGoalPos(double posX, double posY) {
	goalPos->x = posX;
	goalPos->y = posY;
}

//�w�肵���n�_pos�𒆐S�Ƃ����A���ar�͈̔͂ɃG�[�W�F���g�����݂��邩���肷��֐�
bool Agent::CheckExistAgt(Vector pos, double r, std::list<Agent>& lAgent) {
	
	for (auto itr = lAgent.begin(); itr != lAgent.end();itr++) {
		if (Vector::AbsValue(itr->GetPos() - pos) <= r && id!=itr->id) {
			return true;
		}
	}
	
	return false;
}

//�`�ʊ֐�
void Agent::Draw(int originX, int originY, double drawMagnification) {
	int x, y;

	x = static_cast<int>(pos->x * drawMagnification);
	y = static_cast<int>(pos->y * drawMagnification);

	DrawCircle(x + originX, y + originY, 3, GetColor(255, 155, 155), TRUE);
	DrawCircle(x + originX, y + originY, 3, GetColor(0, 0, 0), FALSE);
}

//��������_�ւ̍ŒZ�����x�N�g�������߂�֐� ������x,y���_�̍��W�Ax1,y1,x2,y2�͐����̗��[�̍��W
Vector Agent::posLineToPoint(double x, double y, double x1, double y1, double x2, double y2) {
	Vector vec1, vec2;//�����̕Б��̒[����Ƃ����A�����Б��̒[�Ɠ_�ւ�2�x�N�g��
	Vector vec, tmpVec;
	double cosTheta;

	vec1.x = x - x1; vec1.y = y - y1;
	vec2.x = x2 - x1; vec2.y = y2 - y1;

	cosTheta = Vector::InnerProduct(vec1, vec2) / (vec1.AbsValue() * vec2.AbsValue());

	if (cosTheta < 0) {//2�x�N�g���̐����p�̃R�T�C�������̂Ƃ�
		vec.x = x - x1; vec.y = y - y1;
	}
	else if (vec1.AbsValue() * cosTheta > vec2.AbsValue()) {//2�x�N�g���̐����p�̃R�T�C���������A�_�Ɛ��̕Б��̒[�������x�N�g���̎ˉe���A�����̃x�N�g�����傫���Ƃ�
		vec.x = x - x2; vec.y = y - y2;
	}
	else {
		tmpVec.x = cosTheta * vec2.x; tmpVec.y = cosTheta * vec2.y;
		vec = vec1 - tmpVec;
	}

	return vec;
}