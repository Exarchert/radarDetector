#ifndef _MY_MACRO__
#define _MY_MACRO__

// 发送给雷达的信息    
typedef struct StructSendInfo
{
	byte btSmpNumAdr;//采样点数			0x01
	byte btSmpNum;//0是256,1是512，以此类推

	byte btSmpRateAdr;//采样率			0x02
	byte btSmpRate;//0是25.6G，1是12.8G，以此类推

	byte btGainModeAdr;//增益模式			0x03
	byte btGainMode;//0或1，目前用的1

	byte btGainValAdr;//增益值			0x04
	byte btGainVal;//0-100的数-

	byte btTrigLevelAdr;//触发电平			0x05
	byte btTrigLevel;//10

	//比特位数			0x06---这个不传-----------不知道是啥

	byte btTimeDelayAdr;//延迟调整    0x07		
	byte btTimeDelay;//0-100的数，目前用3

	byte btAddTimesAdr;//累加次数		0x08
	byte btAddTimes;//设置为n=0,1,2,3 =>实际效果为 2的次方

	byte btReFreqAdr;//重复频率		0x09
	byte btReFreq;//0

	byte btWheelAdr;//测量轮使能		0x0A
	byte btWheel;//0时间触发，1是测量轮

	byte btPreciseAdr;//测量轮精度		0x0B
	byte btPrecise;//设置n=每隔几个脉冲发送一次 = 实际精度(多少厘米) * 脉冲数 / 周长（厘米）

	//测量轮精度		0x0C---这个不传-----------不知道是啥

	byte btSmpModeAdr;//采样方式		0x0D
	byte btSmpMode;//测量轮时为2，时间触发为0

	byte btTrigModeAdr;//刹车方式	0x55		==85
	byte btTrigMode;//0

	byte btSteelDiameterAdr;//固定直径	0x31		==49
	byte btSteelDiameter;//0x10

	byte btSteelRangeAdr;//固定区域	0x32			==50
	byte btSteelRange;//0

	byte btSteelModeAdr;//固定模式	0x30			==48
	byte btSteelMode;//1
}StuSendInfo;



#endif