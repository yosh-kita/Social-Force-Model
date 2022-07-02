#include "Manager.h"
#include "Vector.h"
#include "DxLib.h"
#include "Map.h"
#include "Agent.h"
#include "Object.h"
#include "Parameter.h"

Manager::Manager() {
	map = new Map(MAP_SIZE_W, MAP_SIZE_H);//空間の生成

	//空間の外壁に障害物を生成
	CreateObj(0.0, 0.0, 0.5, MAP_SIZE_H);//空間左の壁の生成
	CreateObj(MAP_SIZE_W- 0.5, 0.0, MAP_SIZE_W, MAP_SIZE_H);//空間右の壁の生成
	CreateObj(0.0, 0.0, MAP_SIZE_W, 0.5);//空間上の壁の生成
	CreateObj(0.0, MAP_SIZE_H - 0.5, MAP_SIZE_W, MAP_SIZE_H);//空間下の壁の生成
}

Manager::~Manager() {
	delete map;
	lAgent.clear();
	lObject.clear();
}

void Manager::Process() {
	double drawMagnification;
	int wSizeX, wSizeY;
	int originX, originY;//描写時の原点の座標


	GetDrawScreenSize(&wSizeX, &wSizeY);//画面サイズの確認
	drawMagnification = (wSizeY / map->getH() > wSizeX / map->getW()) ? (wSizeY - 200.0) / map->getH() : (wSizeX - 200.0) / map->getW();//描写時の倍率を設定する
										
	//描写時の原点の座標を計算(画面の中心と空間の中心が一致するようにする)
	originX = (wSizeX / 2) - static_cast<int>(map->getW() * drawMagnification / 2);
	originY = (wSizeY / 2) - static_cast<int>(map->getH() * drawMagnification / 2);

	//エンターキーが押されるまでシミュレーションを実行する
	while (CheckHitKey(KEY_INPUT_RETURN) == 0) {
		ProcessMessage();

		if (lAgent.size()<MAX_NUM_OF_AGENT) {
			CreateAgt();//エージェントの生成
		}

		//エージェントの移動
		for (auto itr = lAgent.begin(); itr != lAgent.end();) {
			itr->CalDrivingForce();//自己駆動力の計算
			itr->CalRepulsiveForce(lAgent, lObject);//斥力の計算
			itr->CalVelocity();//速度の計算
			itr->Move(lObject);//移動

			//目標地点に到着したエージェントは削除する
			if (itr->CheckArrived()) {
				itr = lAgent.erase(itr); 
			}
			else {
				itr++;
			}

		}

		ClearDrawScreen();//画面のクリア
		map->Draw(originX, originY, drawMagnification);//空間の描写
		////障害物の描写
		for (auto itr = lObject.begin(); itr != lObject.end(); itr++) {
			itr->Draw(originX, originY, drawMagnification);
		}
		//エージェントの描写
		for (auto itr = lAgent.begin(); itr != lAgent.end(); itr++) {
			itr->Draw(originX, originY, drawMagnification);
		}
		ScreenFlip();//画面の表示

	}
}

//エージェントの生成
void Manager::CreateAgt() {
	Agent* agent;
	Vector pos;
	int foo = 1000000;//適当に大きな整数を用意し、GetRand(大きい整数)/大きい整数の計算によって、疑似的に0.0～1.0の実数の乱数を生成させる

	//エージェントの1秒辺りの流入人数期待値に応じた確率で、エージェントを生成させる
	if (100.0 * GetRand(foo) / foo <= 100.0 * EXPECTED_INFLOW * DELTA_T) {
		agent = new Agent(lAgent);
		lAgent.push_back(*agent);
	}
}

//左上の座標(x1,y1)、右下の座標(x2,y2)の障害物の生成
void Manager::CreateObj(double x1, double y1, double x2, double y2) {
	Object* object;
	double x, y;//障害物の中心座標
	Vector posUpL, posBoR;//左上と右下の位置ベクトル

	//各引数を位置ベクトルの成分に代入
	posUpL.x = x1; posUpL.y = y1;
	posBoR.x = x2; posBoR.y = y2;

	//中心座標の算出
	x = (x2 - x1) / 2;
	y = (y2 - y1) / 2;

	object = new Object(x, y, &posUpL, &posBoR);
	lObject.push_back(*object);
}