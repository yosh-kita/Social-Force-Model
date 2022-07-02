#pragma once

const static double MAP_SIZE_W = 50.0;//空間の横の長さ
const static double MAP_SIZE_H = 50.0;//空間の縦の長さ
const static int MAX_NUM_OF_AGENT = 1000;//エージェントの空間内最大人数
const static double MAX_TARGET_SPEED = 1.8;//エージェントの自由歩行速度の最大値[m/s]
const static double MIN_TARGET_SPEED = 0.6;//エージェントの自由歩行速度の最大値[m/s]
const static double VIEW = 10.0; //エージェントの視野
const static double MASS = 80.0;//エージェントの質量[kg]
const static double RADIUS = 0.25; //エージェントの半径[m]
const static double DELTA_T = 0.1;//シミュレーション中の1ステップ辺りの時間
const static double EXPECTED_INFLOW = 2.0;//エージェントの1秒辺りの流入人数期待値[人/s]

//Social Force Modelの力の計算に必要なパラメータ
const static double COEF_OF_PROPULSION = 0.5;//推進力係数[s]
const static double COEF_INTERACTION = 2000.0; //インタラクション作用[N]
const static double COEF_REPLUSION = 0.08; //反発作用[m]
const static double COEF_REPLUSION_FORCE = 120000.0; //反発力係数[kg*s^-2]
const static double COEF_FRICTION_FORCE = 240000.0;//摩擦力係数[kg*m^-1*s^-1]