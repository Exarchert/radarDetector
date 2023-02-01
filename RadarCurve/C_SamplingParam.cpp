#include "StdAfx.h"
#include "C_SamplingParam.h"

C_SamplingParam g_sample_param;
// C_SamplingParam g_eq_tg_sample_param;//地震触发采集参数
// C_SamplingParam g_eq_rt_sample_param;//地震实时采集参数
// C_SamplingParam g_eq_nm_sample_param;//地震单次采集参数
// C_SamplingParam g_uep_sample_param;//电磁触发采集参数
//C_SamplingParam g_uep_sample_param;//电磁实时采集参数
//C_SamplingParam g_uep_sample_param;//电磁单次采集参数

C_SamplingParam::C_SamplingParam(void)
{
	m_elec_duty_ratio = 10;
	for (int i = 0; i < USEP_PCB_NUM; i++)
	{
		SetPga(0x06, i);
		SetADC_check(true, i);
		for (int j = 0; j < USEP_PCB_TRACE_NUM; j++)
		{
			m_trace_switch[i][j] = true;
		}
	}
	
	m_line_num = 1;//测线号
	m_file_num = 1;//文件号
	m_conn_mode = 1;		//连接方式
	m_channel_num = 1;	//通道数
	m_sampling_rate = 5;	//采样率

	m_data_bits = 2;			//数据位数
	m_sample_mode = 1;	//采集模式
	m_trigger_mode = 2;	//触发模式
	m_points_num = 5000;		//采样点数
	m_buf_switch = false;		//缓冲区开关

	pga = 1;//增益
	m_adc_adjust = true;
	m_stacking_fold = 0;
	m_trigger_delay = 0;		//发送时延
	//电磁采集参数

	m_elec_single_sample_mode = 1;
	m_elec_trigger_mode = 2;
	m_elec_highspeed_mode = 0;
	m_elec_data_bits = 0;
	m_elec_sample_enable = 1;;
	m_elec_sample_points_num = 275;
	m_elec_sampling_rate = 5;

	//电磁采集器参数设置

	m_elec_frequency_rank = 16;		//频率等级
	m_elec_duty_ratio = 20;			//占空比
	m_elec_trigger_delay = 0;		//发送时延
	m_elec_send_time = 0;			//

	m_elec_cycles_times = 10000;		//激发次数
	m_elec_gps_start_time;		//GPS时间
	m_elec_wheelpulse_times = 0;	//测量轮脉冲次数
	m_elec_gps_sync_cycle = 16000;		//GPS同步周期
	//开始
	m_elec_start_longitude = 0;		//经度
	m_elec_start_EW_longitude = 0;	//东西经
	m_elec_start_latitude = 0;		//纬度
	m_elec_start_SN_latitude = 0;	//南北纬

	//结束
	m_elec_end_longitude = 0;		//经度
	m_elec_end_EW_longitude = 0;	//东西经
	m_elec_end_latitude = 0;		//纬度
	m_elec_end_SN_latitude = 0;	//南北纬

	//测量轮参数设置
	m_enable_trigger_wheel = true;			//是否使用测量轮触发
	m_trigger_wheel_com_id = 1;			//测量轮所在串口ID
	m_pluses_per_trigger = 136;				//测量轮多少脉冲触发一次
	m_sample_times_per_trigger = 0;	//测量轮触发一次要长传多少个采集请求
	m_wheel_forward_length = 0.3f;		//测量轮前进的长度
	m_wheel_radius = 0.047746483f;				//测量轮的半径

	m_eq_velocity = USEP_VELOCITY;
	m_elec_current = 10.0f;

	m_loop_type = 0;
	m_elec_sampling_rate = 3;
	//电磁微分脉冲采集器参数设置

	m_dp_frequency_rank = 8;		//DP频率等级
	m_dp_sampling_rate = 1;			//DP采样率
	m_dp_cycles_times = 100;		//DP总激发周期数
	m_dp_empty_cycles_times = 5;	//DP空激发周期数
	m_dp_sendwave_type = 0;			//DP发射波形类型
	m_dp_pga = 1;					//DP增益

	m_device_type = 0;	//0是低频设备，1是高频设备	E_DEVICE_UEP, E_DEVICE_DP
	m_device_childtype = 0;
}


C_SamplingParam::~C_SamplingParam(void)
{
}

/*
float C_SamplingParam::GetEquiptimeLapse(int SPS)
{
	float	equiptimelapse = 0.0;
	switch(SPS)
	{
	case 30000:
		equiptimelapse = 0.21;
		break;
	case 20000:
		equiptimelapse = 0.315;
		break;
	case 15000:
		equiptimelapse = 0.24;
		break;
	case 10000:
		equiptimelapse = 0.36;
		break;
	case 7500:
		equiptimelapse = 0.31;
		break;
	case 5000:
		equiptimelapse = 0.465;
		break;
	case 3750:
		equiptimelapse = 0.44;
		break;
	case 2500:
		equiptimelapse = 0.66;
		break;
	case 2000:
		equiptimelapse = 0.68;
		break;
	case 1000:
		equiptimelapse = 1.18;
		break;
	case 500:
		equiptimelapse = 2.18;
		break;
	case 100:
		equiptimelapse = 10.18;
		break;
	case 50:
		equiptimelapse = 20.18;
		break;
	default:
		break;
	}
	return equiptimelapse;
}
*/



int C_SamplingParam::GetGPSSyncCycle(int val)
{
	int m_gps_sync_cycle = 0;

	switch( val )
	{
	case 1:
		m_gps_sync_cycle = 16000;
		break;
	case 2:
		m_gps_sync_cycle = 8000;
		break;
	case 3:
		m_gps_sync_cycle = 4000;
		break;
	case 4:
		m_gps_sync_cycle = 2000;
		break;
	case 5:
		m_gps_sync_cycle = 1200;
		break;
	case 6:
		m_gps_sync_cycle = 1000;
		break;
	case 7:
//		m_gps_sync_cycle = 1000;
		m_gps_sync_cycle = 800;
		break;
	case 8:
//		m_gps_sync_cycle = 1000;
		m_gps_sync_cycle = 600;
		break;
	case 9:
		m_gps_sync_cycle = 500;
		break;
	case 10:
		m_gps_sync_cycle = 400;
//		m_gps_sync_cycle = 500;
		break;
	case 11:
		m_gps_sync_cycle = 250;
		break;
	case 12:
		m_gps_sync_cycle = 200;
		break;
	case 13:
		m_gps_sync_cycle = 160;
//		m_gps_sync_cycle = 200;
		break;
	case 14:
		m_gps_sync_cycle = 125;
		break;
	case 15:
		m_gps_sync_cycle = 120;
		break;
	case 16:
		m_gps_sync_cycle = 100;
		break;
	case 17:
		m_gps_sync_cycle = 80;
//		m_gps_sync_cycle = 100;
		break;
	case 18:
		m_gps_sync_cycle = 63;
		break;
	case 19:
		m_gps_sync_cycle = 40;
		break;
	case 20:
		m_gps_sync_cycle = 32;
		break;
	case 21:
		m_gps_sync_cycle = 20;
		break;
	case 22:
		m_gps_sync_cycle = 16;
		break;
	case 23:
		m_gps_sync_cycle = 14;
		break;
	case 24:
		m_gps_sync_cycle = 8;
		break;
	case 25:
		m_gps_sync_cycle = 5;
		break;
	default:
		break;
	}

	return m_gps_sync_cycle;
}


float C_SamplingParam::Elec_GetSampling_rate(int sr_n)
{
	float ti = 0.0;
	if ( m_device_type == 0 )
	{
		switch( sr_n )
		{
		case 1:
			ti = 52734;
			break;
		case 2:
			ti = 26376;
			break;
		case 3:
			ti = 17578;
			break;
		case 4:
			ti = 13183;
			break;
		case 5:
			ti = 10546;
			break;
		case 6:
			ti = 8789;
			break;
		case 7:
			ti = 7533;
			break;
		case 8:
			ti = 6591;
			break;
		case 9:
			ti = 5859;
			break;
		case 10:
			ti = 5273;
			break;
		case 11:
			ti = 4794;
			break;
		case 12:
			ti = 4394;
			break;
		case 13:
			ti = 4056;
			break;
		case 15:
			ti = 3515;
			break;
		case 18:
			ti = 2929;
			break;
		case 21:
			ti = 2511;
			break;
		case 26:
			ti = 2028;
			break;
		case 35:
			ti = 1506;
			break;
		case 52:
			ti = 1014;
			break;
		default:
			ti = 52734;
			break;
		}
	} 
	else if ( m_device_type == 1 )
	{
		switch( sr_n )
		{
		case 1:
			ti = 1000000;
			break;
		case 2:
			ti = 500000;
			break;
		case 3:
			ti = 250000;
			break;
		case 4:
			ti = 125000;
			break;
		default:
			ti = 125000;
			break;
		}
	} 
	else
	{
		ti = 52734;
	}

	return ti;
}


float C_SamplingParam::EQ_GetSampling_rate(int sr_n)
{
	float ti = 0.0;
	switch( sr_n )
	{
	case 1:
		ti = 48828;
		break;
	case 2:
		ti = 24414;
		break;
	case 3:
		ti = 12207;
		break;
	case 4:
		ti = 6103;
		break;
	case 5:
		ti = 3051;
		break;
	case 6:
		ti = 1525;
		break;
	default:
		ti = 48828;
		break;
	}

	return ti;
}


int	C_SamplingParam::Elec_GetSampling_rate_Combo_cn(int sr_n)
{
	int ti = 0;
	switch( sr_n )
	{
	case 1:
		ti = 1;
		break;
	case 2:
		ti = 2;
		break;
	case 3:
		ti = 3;
		break;
	case 4:
		ti = 4;
		break;
	case 5:
		ti = 5;
		break;
	case 6:
		ti = 6;
		break;
	case 7:
		ti = 7;
		break;
	case 8:
		ti = 8;
		break;
	case 9:
		ti = 9;
		break;
	case 10:
		ti = 10;
		break;
	case 11:
		ti = 11;
		break;
	case 12:
		ti = 12;
		break;
	case 13:
		ti = 13;
		break;
	case 15:
		ti = 14;
		break;
	case 18:
		ti = 15;
		break;
	case 21:
		ti = 16;
		break;
	case 26:
		ti = 17;
		break;
	case 35:
		ti = 18;
		break;
	case 52:
		ti = 19;
		break;
	default:
		ti = 1;
		break;
	}

	return ti;

}


int	C_SamplingParam::Elec_GetSampling_rate_cn(int sr_cn)
{
	int ti = 0;
	switch( sr_cn )
	{
	case 1:
		ti = 1;
		break;
	case 2:
		ti = 2;
		break;
	case 3:
		ti = 3;
		break;
	case 4:
		ti = 4;
		break;
	case 5:
		ti = 5;
		break;
	case 6:
		ti = 6;
		break;
	case 7:
		ti = 7;
		break;
	case 8:
		ti = 8;
		break;
	case 9:
		ti = 9;
		break;
	case 10:
		ti = 10;
		break;
	case 11:
		ti = 11;
		break;
	case 12:
		ti = 12;
		break;
	case 13:
		ti = 13;
		break;
	case 14:
		ti = 15;
		break;
	case 15:
		ti = 18;
		break;
	case 16:
		ti = 21;
		break;
	case 17:
		ti = 26;
		break;
	case 18:
		ti = 35;
		break;
	case 19:
		ti = 52;
		break;
	default:
		ti = 1;
		break;
	}

	return ti;

}


int C_SamplingParam::Elec_Get_Combo_Pga(int realpga)
{
	int combo_pga = 0;
	switch(realpga)
 	{
 	case 1:
 		combo_pga = 0;
 		break;
 	case 2:
 		combo_pga = 1;
 		break;
 	case 4:
 		combo_pga = 2;
 		break;
 	case 8:
 		combo_pga = 3;
 		break;
	case 16:
		combo_pga = 4;
		break;
	case 32:
		combo_pga = 5;
		break;
	case 64:
		combo_pga = 6;
		break;
	case 128:
		combo_pga = 7;
		break;
 	default:
 		break;
 	}

	return combo_pga;
}


int C_SamplingParam::Elec_Get_Real_Pga( int combopga )
{
	int real_pga = 0;
	switch(combopga)
	{
	case 0:
		real_pga = 1;
		break;
	case 1:
		real_pga = 2;
		break;
	case 2:
		real_pga = 4;
		break;
	case 3:
		real_pga = 8;
		break;
	case 4:
		real_pga = 16;
		break;
	case 5:
		real_pga = 32;
		break;
	case 6:
		real_pga = 64;
		break;
	case 7:
		real_pga = 128;
		break;
	default:
		break;
	}

	return real_pga;
}
