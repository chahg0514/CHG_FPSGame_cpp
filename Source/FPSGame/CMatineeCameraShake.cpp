
// Fill out your copyright notice in the Description page of Project Settings.


#include "CMatineeCameraShake.h"
UCMatineeCameraShake::UCMatineeCameraShake()
{
	OscillationDuration = 0.1f;
	//화면 흔들림 지속시간(0미만은 무한, 0은 진동없음)
	OscillationBlendInTime = 0.0f;
	//진동이 0에서 1까지 조정되는 블렌드 인 시간
	//점점 진동이 커지는 시간
	OscillationBlendOutTime = 0.0f;
	//진동이 0에서 1까지 조정되는 블렌드 아웃 시간
	//점점 진동이 작아지는 시간
	LocOscillation.Y.Amplitude = 2.0f; //Y축 진폭
	LocOscillation.Z.Amplitude = 3.0f;
	LocOscillation.Y.Frequency = 2.0f; //Y축 빈도
	LocOscillation.Z.Frequency = 3.0f;

}

