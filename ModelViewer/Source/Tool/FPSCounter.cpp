#pragma comment(lib, "winmm.lib")
#include "FPSCounter.h"

FPSCounter::FPSCounter() :
	FPS(60.0f),
	MIN_DELTA_MSEC(1000.0f / FPS),
	MAX_DELTA_MSEC(100.0f),
	NUM_SAMPLE(128){

	fps_			= 0.0f;
	delta_msec_list_.resize(NUM_SAMPLE, 0.0f);
	sum_delta_msec_ = 0.0f;
	frame_count_	= 0;

	// 生成時に最初のクロックカウント数・周波数を取得
	if (QueryPerformanceCounter(&counter_) != 0) {
		
		previous_counter_ = counter_;
		LARGE_INTEGER freq;
		QueryPerformanceFrequency(&freq);
		frequency_ = static_cast<float>(freq.QuadPart);
	}

	// Sleepの精度を1msecにする
	timeBeginPeriod(1);
}

FPSCounter::~FPSCounter() {

	// Sleepの精度を戻す
	timeEndPeriod(1);
}

/*!
* @brief フレームレートの取得
* @return フレームレート
*/
float FPSCounter::GetFPS() const {

	return fps_;
}

/*!
* @brief 経過時間計測
* @return 前回計測時からの経過時間
*/
float FPSCounter::CalcDeltaMSec() {

	// 前回計測時のクロックカウント数との差分から経過時間を計測
	if (QueryPerformanceCounter(&counter_) == 0) {
		return 0.0f;
	}

	__int64 delta_count = counter_.QuadPart - previous_counter_.QuadPart;
	float delta_msec = static_cast<float>(delta_count) * 1000.0f / frequency_;
	previous_counter_ = counter_;

	return delta_msec;
}

/*!
* @brief 平均フレームレートの計測
* @param delta_msec 経過時間
*/
float FPSCounter::CalcFPS(float delta_msec) {

	if (MAX_DELTA_MSEC < delta_msec) {
		return MIN_DELTA_MSEC;
	}

	// 経過時間リストの削除・追加処理
	delta_msec_list_.pop_front();
	delta_msec_list_.push_back(delta_msec);

	// サンプルから平均FPS算出
	float average_delta_msec = (sum_delta_msec_ + delta_msec) / NUM_SAMPLE;

	if (average_delta_msec == 0.0f) {
		return fps_;
	}

	fps_ = 1000.0f / average_delta_msec;

	// 経過時間の合計値の更新・1フレーム後の削除要素の減算と追加要素の加算
	sum_delta_msec_ += delta_msec - delta_msec_list_.front();

	return fps_;
}

/*!
* @brief 設定したフレームレートになるよう待機処理
*/
void FPSCounter::Wait() {

	// 1フレーム毎に待機時間を計算すると1msの誤差が大きくなるため
	// 複数フレームでかかる時間を基準に待機時間を計算する
	float should_take_time = MIN_DELTA_MSEC * frame_count_;
	float wait_time = should_take_time - sum_delta_msec_;
	wait_time = wait_time < 0.0f ? 0.0f : wait_time;

	Sleep(static_cast<unsigned int>(wait_time));

	// 基準となるフレーム数をサンプル数までインクリメント
	if (frame_count_ < NUM_SAMPLE) {
		frame_count_++;
	}
}
