#ifndef _MY_MACRO__
#define _MY_MACRO__

// ���͸��״����Ϣ    
typedef struct StructSendInfo
{
	byte btSmpNumAdr;//��������			0x01
	byte btSmpNum;//0��256,1��512���Դ�����

	byte btSmpRateAdr;//������			0x02
	byte btSmpRate;//0��25.6G��1��12.8G���Դ�����

	byte btGainModeAdr;//����ģʽ			0x03
	byte btGainMode;//0��1��Ŀǰ�õ�1

	byte btGainValAdr;//����ֵ			0x04
	byte btGainVal;//0-100����-

	byte btTrigLevelAdr;//������ƽ			0x05
	byte btTrigLevel;//10

	//����λ��			0x06---�������-----------��֪����ɶ

	byte btTimeDelayAdr;//�ӳٵ���    0x07		
	byte btTimeDelay;//0-100������Ŀǰ��3

	byte btAddTimesAdr;//�ۼӴ���		0x08
	byte btAddTimes;//����Ϊn=0,1,2,3 =>ʵ��Ч��Ϊ 2�Ĵη�

	byte btReFreqAdr;//�ظ�Ƶ��		0x09
	byte btReFreq;//0

	byte btWheelAdr;//������ʹ��		0x0A
	byte btWheel;//0ʱ�䴥����1�ǲ�����

	byte btPreciseAdr;//�����־���		0x0B
	byte btPrecise;//����n=ÿ���������巢��һ�� = ʵ�ʾ���(��������) * ������ / �ܳ������ף�

	//�����־���		0x0C---�������-----------��֪����ɶ

	byte btSmpModeAdr;//������ʽ		0x0D
	byte btSmpMode;//������ʱΪ2��ʱ�䴥��Ϊ0

	byte btTrigModeAdr;//ɲ����ʽ	0x55		==85
	byte btTrigMode;//0

	byte btSteelDiameterAdr;//�̶�ֱ��	0x31		==49
	byte btSteelDiameter;//0x10

	byte btSteelRangeAdr;//�̶�����	0x32			==50
	byte btSteelRange;//0

	byte btSteelModeAdr;//�̶�ģʽ	0x30			==48
	byte btSteelMode;//1
}StuSendInfo;



#endif