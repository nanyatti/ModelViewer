#pragma once
#include <list>
#include <Windows.h>

/*!
* @brief FPS�E�o�ߎ��Ԃ��v��
*/
class FPSCounter {

private:

	const float FPS;			// �t���[�����[�g�̎w��
	const float MIN_DELTA_MSEC;	// 1�t���[��������̏������Ԃ̍ŏ��l
	const float MAX_DELTA_MSEC;	// 1�t���[��������̏������Ԃ̍ő�l
	const int	NUM_SAMPLE;		// FPS�v�����̃f�[�^�̃T���v����

public:

	FPSCounter();
	~FPSCounter();
	float GetFPS() const;
	float CalcDeltaMSec();
	float CalcFPS(float delta_msec);
	void Wait();

private:

	float fps_;							// �t���[�����[�g�̌v���l
	LARGE_INTEGER counter_;				// �N���b�N�J�E���g��
	LARGE_INTEGER previous_counter_;	// 1�t���[���O�̃N���b�N�J�E���g��
	float frequency_;					// ��b������̃N���b�N�J�E���g���i���g���j
	std::list<float> delta_msec_list_;	// �o�ߎ��ԃ��X�g�E�T���v���̊Ǘ�
	float sum_delta_msec_;				// �t���[��������̌o�ߎ��Ԃ̍��v�l�E�폜�v�f�͊܂܂Ȃ�
	int frame_count_;					// �v����
};