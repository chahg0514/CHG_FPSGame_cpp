
// Fill out your copyright notice in the Description page of Project Settings.


#include "CMatineeCameraShake.h"
UCMatineeCameraShake::UCMatineeCameraShake()
{
	OscillationDuration = 0.1f;
	//ȭ�� ��鸲 ���ӽð�(0�̸��� ����, 0�� ��������)
	OscillationBlendInTime = 0.0f;
	//������ 0���� 1���� �����Ǵ� ���� �� �ð�
	//���� ������ Ŀ���� �ð�
	OscillationBlendOutTime = 0.0f;
	//������ 0���� 1���� �����Ǵ� ���� �ƿ� �ð�
	//���� ������ �۾����� �ð�
	LocOscillation.Y.Amplitude = 2.0f; //Y�� ����
	LocOscillation.Z.Amplitude = 3.0f;
	LocOscillation.Y.Frequency = 2.0f; //Y�� ��
	LocOscillation.Z.Frequency = 3.0f;

}

