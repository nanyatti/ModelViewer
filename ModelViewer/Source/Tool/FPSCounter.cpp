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

	// �������ɍŏ��̃N���b�N�J�E���g���E���g�����擾
	if (QueryPerformanceCounter(&counter_) != 0) {
		
		previous_counter_ = counter_;
		LARGE_INTEGER freq;
		QueryPerformanceFrequency(&freq);
		frequency_ = static_cast<float>(freq.QuadPart);
	}

	// Sleep�̐��x��1msec�ɂ���
	timeBeginPeriod(1);
}

FPSCounter::~FPSCounter() {

	// Sleep�̐��x��߂�
	timeEndPeriod(1);
}

/*!
* @brief �t���[�����[�g�̎擾
* @return �t���[�����[�g
*/
float FPSCounter::GetFPS() const {

	return fps_;
}

/*!
* @brief �o�ߎ��Ԍv��
* @return �O��v��������̌o�ߎ���
*/
float FPSCounter::CalcDeltaMSec() {

	// �O��v�����̃N���b�N�J�E���g���Ƃ̍�������o�ߎ��Ԃ��v��
	if (QueryPerformanceCounter(&counter_) == 0) {
		return 0.0f;
	}

	__int64 delta_count = counter_.QuadPart - previous_counter_.QuadPart;
	float delta_msec = static_cast<float>(delta_count) * 1000.0f / frequency_;
	previous_counter_ = counter_;

	return delta_msec;
}

/*!
* @brief ���σt���[�����[�g�̌v��
* @param delta_msec �o�ߎ���
*/
float FPSCounter::CalcFPS(float delta_msec) {

	if (MAX_DELTA_MSEC < delta_msec) {
		return MIN_DELTA_MSEC;
	}

	// �o�ߎ��ԃ��X�g�̍폜�E�ǉ�����
	delta_msec_list_.pop_front();
	delta_msec_list_.push_back(delta_msec);

	// �T���v�����畽��FPS�Z�o
	float average_delta_msec = (sum_delta_msec_ + delta_msec) / NUM_SAMPLE;

	if (average_delta_msec == 0.0f) {
		return fps_;
	}

	fps_ = 1000.0f / average_delta_msec;

	// �o�ߎ��Ԃ̍��v�l�̍X�V�E1�t���[����̍폜�v�f�̌��Z�ƒǉ��v�f�̉��Z
	sum_delta_msec_ += delta_msec - delta_msec_list_.front();

	return fps_;
}

/*!
* @brief �ݒ肵���t���[�����[�g�ɂȂ�悤�ҋ@����
*/
void FPSCounter::Wait() {

	// 1�t���[�����ɑҋ@���Ԃ��v�Z�����1ms�̌덷���傫���Ȃ邽��
	// �����t���[���ł����鎞�Ԃ���ɑҋ@���Ԃ��v�Z����
	float should_take_time = MIN_DELTA_MSEC * frame_count_;
	float wait_time = should_take_time - sum_delta_msec_;
	wait_time = wait_time < 0.0f ? 0.0f : wait_time;

	Sleep(static_cast<unsigned int>(wait_time));

	// ��ƂȂ�t���[�������T���v�����܂ŃC���N�������g
	if (frame_count_ < NUM_SAMPLE) {
		frame_count_++;
	}
}
