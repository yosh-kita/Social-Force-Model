#include "Agent.h"
#include "Dxlib.h"
#include "Vector.h"
#include "Parameter.h"
#include "Object.h"
#define _USE_MATH_DEFINES
#include <math.h>

int Agent::n = 0;

//エージェントの生成地点・移動目標地点は空間の4隅付近に設定するため、各隅を列挙型で定義
enum class Direction {
	nW = 0,
	nE = 1,
	sW = 2,
	sE = 3
};

Agent::Agent(std::list<Agent>& lAgent) {
	int foo = 1000000;//適当に大きな整数を用意し、GetRand(大きい整数)/大きい整数の計算によって、疑似的に0.0～1.0の実数の乱数を生成させる
	Direction start, goal;
	Vector vec;

	vel = new Vector;
	goalPos = new Vector;
	drivingForce = new Vector;
	repulsiveForce = new Vector;
	targetSpeed = MIN_TARGET_SPEED + ((MAX_TARGET_SPEED - MIN_TARGET_SPEED) * GetRand(foo) / foo);//歩行速度を最大値～最小値の範囲でランダムに設定する
	id = n;//エージェントの識別番号は生成順に設定
	n++;

	start = GetRandDir();//エージェントの生成地点をランダムに決定する

	//移動目標地点を、生成地点と被らないようにランダムに決定する
	do {
		goal = GetRandDir();
	} while (goal == start);

	vec = GetPosDir(start);//生成地点方向の空間の隅の座標を取得
	ShiftPos(start, vec, 1.0 * GetRand(foo) / foo);//生成地点を±1.0 mの範囲でずらす
	
	while (CheckExistAgt(vec, 1.0, lAgent)) {
		ShiftPos(start, vec, 1.0);//生成地点付近（半径1.0mの範囲）にエージェントが存在する場合、生成地点をずらして、エージェントの生成時の衝突を避ける
	}
	

	SetPos(vec.x, vec.y);

	vec = GetPosDir(goal);
	ShiftPos(goal, vec, 1.0 * GetRand(foo) / foo);//移動目標地点を±1.0 mの範囲でずらす
	SetGoalPos(vec.x, vec.y);

	graphHandle = LoadGraph("Graphics/Agt.png");
}

Agent::~Agent() {
	delete vel;
	delete goalPos;
	delete drivingForce;
	delete repulsiveForce;
}

//自己駆動力の計算
void Agent::CalDrivingForce() {
	Vector dirGoal;//目的地への方向ベクトル

	//現在地から目的地への単位ベクトルを算出
	dirGoal = *goalPos - *pos;
	dirGoal.Normalize();

	drivingForce->ZeroVec();
	*drivingForce = (dirGoal * targetSpeed - *vel) / COEF_OF_PROPULSION;
}

//斥力の計算
void Agent::CalRepulsiveForce(std::list<Agent>& lAgent, std::list<Object>& lObject) {
	repulsiveForce->ZeroVec();

	//周囲のエージェントから受ける斥力の計算
	for (auto itr = lAgent.begin(); itr != lAgent.end(); itr++) {
		if (id != itr->id) {
			*repulsiveForce += CalOneAgtReplusive(*itr, RADIUS, RADIUS);
		}
	}

	//周囲の障害物から受ける斥力の計算
	for (auto itr = lObject.begin(); itr != lObject.end(); itr++) {
		*repulsiveForce += CalOneObjReplusive(*itr, RADIUS);
	}
}

//1エージェントから受ける斥力の計算
//引数のriは自身の半径、rjは相手エージェントの半径、deltaVtは相手を基準にした自身の相対速度ベクトル
Vector Agent::CalOneAgtReplusive(Agent& agent, double ri, double rj) {
	double coef, d, cosTheta, deltaVt;
	Vector deltaV, replusive, n, t;
	
	replusive.ZeroVec();
	n.ZeroVec();
	t.ZeroVec();

	deltaV = *vel - *agent.vel;
	n = *pos - agent.GetPos();//斥力を与えるエージェントから、斥力を受けるエージェントへの相対位置ベクトルを算出
	d = n.AbsValue();//エージェント間の距離を測定

	if (d > VIEW) {
		return replusive;//視界外ならゼロベクトルを返す
	}

	n.Normalize();//斥力を与えるエージェントから、斥力を受けるエージェントへの単位方向ベクトルを算出
	t = n;
	t.Rotate(M_PI / 2.0);//斥力を与えるエージェントから、斥力を受けるエージェントの方向へ垂直な単位ベクトルを算出

	//反発相互作用力項（エージェント間に常に働く反発力）の計算
	coef = COEF_INTERACTION * exp((ri + rj - d) / COEF_REPLUSION);
	replusive += n * coef;

	if (ri + rj - d > 0) {
		//物体力項（エージェントの衝突時に作用する。進行方向の逆へ作用する力）の計算
		coef = COEF_REPLUSION_FORCE * (ri + rj - d);
		replusive += n * coef;

		//摩擦力項（エージェントの衝突時に作用する。接線方向に作用する力）の計算
		if (deltaV.AbsValue() > 0) {
			cosTheta = Vector::InnerProduct(t, deltaV) / deltaV.AbsValue();//内積を用いて、エージェントの接線方向ベクトルと相対速度ベクトルが成す角のコサインを求める
			deltaVt = deltaV.AbsValue() * cosTheta;//エージェントの相対速度ベクトルの接線方向成分の大きさを求める
			coef = COEF_FRICTION_FORCE * (ri + rj - d) * deltaVt;
			replusive += t * coef;
		}
	}

	return replusive;
}

//障害物から受ける斥力の計算 引数のriは自身の半径
Vector Agent::CalOneObjReplusive(Object& object, double ri) {
	double coef, d; 
	double minD = 1000000.0;
	Vector replusive, minN, t;
	Vector n[4];

	//障害物の外周の各辺に対して、エージェントへの最短距離ベクトルを算出する
	n[0] = posLineToPoint(pos->x, pos->y, object.GetPosUL().x, object.GetPosUL().y, object.GetPosBR().x, object.GetPosUL().y);//上の辺
	n[1] = posLineToPoint(pos->x, pos->y, object.GetPosUL().x, object.GetPosBR().y, object.GetPosBR().x, object.GetPosBR().y);//下の辺
	n[2] = posLineToPoint(pos->x, pos->y, object.GetPosUL().x, object.GetPosUL().y, object.GetPosUL().x, object.GetPosBR().y);//左の辺
	n[3] = posLineToPoint(pos->x, pos->y, object.GetPosBR().x, object.GetPosUL().y, object.GetPosBR().x, object.GetPosBR().y);//右の辺

	//最短となるベクトルと距離の算出
	for (int i = 0; i < 4; i++) {
		d = n[i].AbsValue();
		if (d < minD) {
			minN = n[i];
			minD = d;
		}
	}

	if (minD > VIEW) {
		return replusive;//視界外ならゼロベクトルを返す
	}
	
	minN.Normalize();//障害物からエージェントへのベクトルを単位方向ベクトル化
	t = minN;
	t.Rotate(M_PI / 2.0);//nに垂直な単位ベクトルを算出
	
	//反発相互作用力項（常に働く反発力）の計算
	coef = COEF_INTERACTION * exp((ri - minD) / COEF_REPLUSION);
	replusive += minN * coef;
	
	if (ri - minD > 0) {
		//物体力項の計算
		coef = COEF_REPLUSION_FORCE * (ri- minD);
		replusive += minN * coef;
		
		//摩擦力項の計算
		coef = COEF_FRICTION_FORCE * (ri - minD) * Vector::InnerProduct(*vel, t);
		replusive += t * coef;
	}
	
	return replusive;
}

//速度ベクトルの計算
void Agent::CalVelocity() {
	Vector acceleration;

	acceleration = *drivingForce + (*repulsiveForce / MASS);
	*vel += acceleration * DELTA_T;
	//目標速度を超えないようにする
	if (vel->AbsValue() > targetSpeed) {
		vel->Normalize();
		vel->x *= targetSpeed;
		vel->y *= targetSpeed;
	}
}

//エージェントの移動
void Agent::Move(std::list<Object>& lObject) {
	*pos += *vel * DELTA_T;
}

//目的地への到着の判定関数
bool Agent::CheckArrived() {
	if (fabs(pos->x - goalPos->x) < 0.5 && fabs(pos->y - goalPos->y) < 0.5) {
		return true;
	}
	else {
		return false;
	}

}

//ランダムに4隅を返す関数
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

//4隅に当たる位置ベクトルを返す関数
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

//エージェント位置をずらす関数
void Agent::ShiftPos(Direction dir, Vector& pos, double l) {
	int foo = 1000000;//適当に大きな整数を用意し、GetRand(大きい整数)/大きい整数の計算によって、疑似的に0.0～1.0の実数の乱数を生成させる
	double theta = M_PI / 2.0 * GetRand(foo) / foo;//x軸から時計周り方向の0～90度のランダムな角度

	//位置ベクトルがどの象限の方向かに応じて、生成位置をずらす方向（角度）を決定する
	switch (dir) {
	case Direction::nW:
		theta += 0.0;//位置ベクトルが空間の左隅方向に存在するなら、第4象限方向にずらす
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

//目標地点の設定
void Agent::SetGoalPos(double posX, double posY) {
	goalPos->x = posX;
	goalPos->y = posY;
}

//指定した地点posを中心とした、半径rの範囲にエージェントが存在するか判定する関数
bool Agent::CheckExistAgt(Vector pos, double r, std::list<Agent>& lAgent) {
	
	for (auto itr = lAgent.begin(); itr != lAgent.end();itr++) {
		if (Vector::AbsValue(itr->GetPos() - pos) <= r && id!=itr->id) {
			return true;
		}
	}
	
	return false;
}

//描写関数
void Agent::Draw(int originX, int originY, double drawMagnification) {
	int x, y;

	x = static_cast<int>(pos->x * drawMagnification);
	y = static_cast<int>(pos->y * drawMagnification);

	DrawCircle(x + originX, y + originY, 3, GetColor(255, 155, 155), TRUE);
	DrawCircle(x + originX, y + originY, 3, GetColor(0, 0, 0), FALSE);
}

//線分から点への最短距離ベクトルを求める関数 引数のx,yが点の座標、x1,y1,x2,y2は線分の両端の座標
Vector Agent::posLineToPoint(double x, double y, double x1, double y1, double x2, double y2) {
	Vector vec1, vec2;//線分の片側の端を基準とした、もう片側の端と点への2ベクトル
	Vector vec, tmpVec;
	double cosTheta;

	vec1.x = x - x1; vec1.y = y - y1;
	vec2.x = x2 - x1; vec2.y = y2 - y1;

	cosTheta = Vector::InnerProduct(vec1, vec2) / (vec1.AbsValue() * vec2.AbsValue());

	if (cosTheta < 0) {//2ベクトルの成す角のコサインが負のとき
		vec.x = x - x1; vec.y = y - y1;
	}
	else if (vec1.AbsValue() * cosTheta > vec2.AbsValue()) {//2ベクトルの成す角のコサインが正かつ、点と線の片側の端が成すベクトルの射影が、線分のベクトルより大きいとき
		vec.x = x - x2; vec.y = y - y2;
	}
	else {
		tmpVec.x = cosTheta * vec2.x; tmpVec.y = cosTheta * vec2.y;
		vec = vec1 - tmpVec;
	}

	return vec;
}