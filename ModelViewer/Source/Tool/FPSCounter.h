#pragma once
#include <list>
#include <Windows.h>

/*!
* @brief FPS・経過時間を計測
*/
class FPSCounter {

private:

	const float FPS;			// フレームレートの指定
	const float MIN_DELTA_MSEC;	// 1フレーム当たりの処理時間の最小値
	const float MAX_DELTA_MSEC;	// 1フレーム当たりの処理時間の最大値
	const int	NUM_SAMPLE;		// FPS計測時のデータのサンプル数

public:

	FPSCounter();
	~FPSCounter();
	float GetFPS() const;
	float CalcDeltaMSec();
	float CalcFPS(float delta_msec);
	void Wait();

private:

	float fps_;							// フレームレートの計測値
	LARGE_INTEGER counter_;				// クロックカウント数
	LARGE_INTEGER previous_counter_;	// 1フレーム前のクロックカウント数
	float frequency_;					// 一秒あたりのクロックカウント数（周波数）
	std::list<float> delta_msec_list_;	// 経過時間リスト・サンプルの管理
	float sum_delta_msec_;				// フレーム当たりの経過時間の合計値・削除要素は含まない
	int frame_count_;					// 計測回数
};