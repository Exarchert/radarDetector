#ifndef __UEP_BLOCK_H_FILE__
#define __UEP_BLOCK_H_FILE__


//****************************** UEP **************************************//
/* uep各数据块结构体 */

/*UEP文件总卷头*/
#pragma pack(push)
#pragma pack(1)
struct UEP_ReelBlock // *.uep
{
	DWORD workTag;//作业标识号（任务识别码）
	DWORD lineNum;//测线号

	DWORD triggerSum;//激发次数
	short traceSum;//每个记录的道数
	short frameSum;//该卷每道的采样点数
    
	float sampleInterval;//该卷采样间隔
	float samplingTimelapse;//原始记录采样延时
	
	short sendtime;// 发射时间

	////////////////////////////////////////////////////////////
	//						新增扩展  采集参数 于 打标
	////////////////////////////////////////////////////////////
	int  m_has_sample_param;		//是否有采集参数
	int	 m_sample_param_pos;		//采集参数距离文件开头的位置
	int	 m_sample_param_size;		//采集参数总大小

	int	 m_has_tags;						//是否有“中断标”
	int	 m_tags_pos;						//“中断标”距离文件开头的位置
	int	 m_tags_num;					//有多少“中断标”
	int	 m_tags_begin_val;			//数据包起始采集时标志值
	int	 m_tags_end_val;				//数据包终止采集时标志值
	int	 m_is_time_tag;					//是否为时间标
	int	 m_val_per_ch;					//每一道采样数据间隔多少标值


	int	 m_has_gps_gdjw;					//是否有国电经纬GPS
	 
	// 剩余自定义
	char untapped[330];	
};//共400字节
#pragma pack(pop)

///* 各天线头记录块：一个天线60字节，五个天线300字节 */
//#pragma pack(push)
//#pragma pack(1)
//struct UEP_NewMastBlock 
//{
//	BOOL	bSwitch;		//1
//
//	int	nCycleNum;			//2 线圈匝数
//	
//	float fLength;			//3 长
//	float fWidth;			//4 宽
//
//	float x1;				//5
//	float y1;				//6
//
//	float x2;				//7
//	float y2;				//8
//
//	COLORREF color;			//9
//
//
//	float fStartElectro;	//10
//
//	float z1;
//	float z2;
//
//	float xzAngle;
//	float yzAngle;
//
//	int type;
//// 一共60个字节
//};
//#pragma pack(pop)
//
///* 总天线头记录块：600字节 */
//#pragma pack(push)
//#pragma pack(1)
//struct UEP_MastBlock 
//{
//	// 发送线圈
//	UEP_NewMastBlock UEPSendMast[5];	// 300字节
//	// 接受线圈
//	UEP_NewMastBlock UEPRcvMast[5];		// 300字节
//
//	UEP_MastBlock()
//	{
//		// 发送线圈1
//		UEPSendMast[0].bSwitch = TRUE;
//		UEPSendMast[0].nCycleNum = 100;
//		UEPSendMast[0].fLength = 1.0;
//		UEPSendMast[0].fWidth = 1.0;
//		UEPSendMast[0].x1 = -5.0;
//
//		UEPSendMast[0].y1 = 0.0;
//		UEPSendMast[0].x2 = 5.0;
//		UEPSendMast[0].y2 = 0.0;
//		UEPSendMast[0].color = RGB(255, 255, 0);
//		UEPSendMast[0].fStartElectro = 15.0;
//
//		UEPSendMast[0].z1 = 0.0;
//		UEPSendMast[0].z2 = 0.0;
//		UEPSendMast[0].xzAngle = 0.0;
//		UEPSendMast[0].yzAngle = 0.0;
//		UEPSendMast[0].type = 0;
//
//
//		// 接受线圈1
//		UEPRcvMast[0].bSwitch = TRUE;
//		UEPRcvMast[0].nCycleNum = 100;
//		UEPRcvMast[0].fLength = 1.0;
//		UEPRcvMast[0].fWidth = 1.0;
//		UEPRcvMast[0].x1 = -5.0;
//
//		UEPRcvMast[0].y1 = 0.0;
//		UEPRcvMast[0].x2 = 5.0;
//		UEPRcvMast[0].y2 = 0.0;
//		UEPRcvMast[0].color = RGB(255, 255, 0);
//		UEPRcvMast[0].fStartElectro = 15.0;
//
//		UEPRcvMast[0].z1 = 0.0;
//		UEPRcvMast[0].z2 = 0.0;
//		UEPRcvMast[0].xzAngle = 0.0;
//		UEPRcvMast[0].yzAngle = 0.0;
//		UEPRcvMast[0].type = 0;
//	}
//};
//#pragma pack(pop)
//
///*电流块*/
//#pragma pack(push)
//#pragma pack(1)
//struct UEP_HeadBlock 
//{
//	int EmissionCurrent;// 发射电流
//	int TurnoffCurrent;// 关断电流
//	int TransitionCurrent[20];// 过渡电流
//};
//#pragma pack(pop)
//
///*参数设置块*/
//#pragma pack(push)
//#pragma pack(1)
//struct UEP_SamplingParamBlock {
//	bool	m_trace_switch[5][64];			//通道
//	 //采集常用参数 49
//	int					m_line_num;//测线号
//	int					m_file_num;//文件号
//	int					m_conn_mode;		//连接方式
//	unsigned int		m_channel_num;	//通道数
//	int					m_sampling_rate;	//采样率
//
//	int					m_data_bits;			//数据位数 
//	unsigned char		m_sample_mode;	//采集模式
//	unsigned char		m_trigger_mode;	//触发模式
//	unsigned short		m_points_num;		//采样点数
//	bool				m_buf_switch;		//缓冲区开关
//
//	unsigned char		m_pga[5];		//各板增益
//	unsigned char		pga;
//	bool				m_adc_check[5];	//系统自检 
//	bool				m_adc_adjust;	//系统校准
//	unsigned int		m_stacking_fold;
//	int					m_trigger_delay;		//发送时延
//
//	//电磁采集参数22
//	int					m_elec_single_sample_mode;
//	int					m_elec_trigger_mode;
//	int					m_elec_highspeed_mode;
//	int					m_elec_data_bits;
//	int					m_elec_sample_enable;
//	unsigned short		m_elec_sample_points_num;
//
//	//电磁采集器参数设置 84
//	int					m_elec_frequency_rank;		//频率等级
//	int					m_elec_duty_ratio;			//占空比
//	int					m_elec_trigger_delay;		//发送时延
//	int					m_elec_send_time;			//
//
//	int					m_elec_cycles_times;		//激发次数
//	SYSTEMTIME			m_elec_gps_start_time;		//GPS时间  //32字节
//	int					m_elec_wheelpulse_times;	//测量轮脉冲次数
//	int					m_elec_gps_sync_cycle;		//GPS同步周期
//	//开始
//	float				m_elec_start_longitude;		//经度
//	unsigned short		m_elec_start_EW_longitude;	//东西经
//	float				m_elec_start_latitude;		//纬度
//	unsigned short		m_elec_start_SN_latitude;	//南北纬
//
//	//结束
//	float				m_elec_end_longitude;		//经度
//	unsigned short		m_elec_end_EW_longitude;	//东西经
//	float				m_elec_end_latitude;		//纬度
//	unsigned short		m_elec_end_SN_latitude;	//南北纬
//
//	//测量轮参数设置 34
//	bool				m_enable_trigger_wheel;			//是否使用测量轮触发
//	unsigned int		m_trigger_wheel_com_id;			//测量轮所在串口ID
//	unsigned int		m_pluses_per_trigger;				//测量轮多少脉冲触发一次
//	unsigned int		m_sample_times_per_trigger;	//测量轮触发一次要长传多少个采集请求
//	float				m_wheel_forward_length;		//测量轮前进的长度
//	float				m_wheel_radius;				//测量轮的半径
//
//	float				m_eq_velocity;			//电磁波速度
//	float				m_elec_current;				//电磁线圈电流
//
//	unsigned char		m_loop_type;				//电磁线圈类型
//	int					m_elec_sampling_rate;
//
//
//	//电磁微分脉冲采集器参数设置 29
//	int					m_dp_frequency_rank;		//DP频率等级
//	int					m_dp_sampling_rate;			//DP采样率
//	int					m_dp_cycles_times;			//DP总激发周期数
//	int					m_dp_empty_cycles_times;	//DP空激发周期数
//	int					m_dp_sendwave_type;			//DP发射波形类型
//	unsigned char		m_dp_pga;					//DP增益
//
//	//设备类型
//	int					m_device_type;				//设备类型
//	int					m_device_childtype;				//设备类型
//};
///*struct UEP_SamplingParamBlock 
//{
//	bool	m_trace_switch[5][64];			//通道
//	 //采集常用参数 41
//	int					m_line_num;//测线号
//	int					m_file_num;//文件号
//	int					m_conn_mode;		//连接方式
//	unsigned int		m_channel_num;	//通道数
//	int					m_sampling_rate;	//采样率
//
//	int					m_data_bits;			//数据位数
//	unsigned char		m_sample_mode;	//采集模式
//	unsigned char		m_trigger_mode;	//触发模式
//	unsigned short		m_points_num;		//采样点数
//	bool				m_buf_switch;		//缓冲区开关
//
//	unsigned char		m_pga[5];		//各板增益
//	unsigned char		pga;
//	bool				m_adc_check[5];	//系统自检
//	bool				m_adc_adjust;	//系统校准
//	unsigned int		m_stacking_fold;
//	int					m_trigger_delay;		//发送时延
//
//	//电磁采集参数22
//	int					m_elec_single_sample_mode;
//	int					m_elec_trigger_mode;
//	int					m_elec_highspeed_mode;
//	int					m_elec_data_bits;
//	int					m_elec_sample_enable;
//	unsigned short		m_elec_sample_points_num;
//
//	//电磁采集器参数设置 84
//	int					m_elec_frequency_rank;		//频率等级
//	int					m_elec_duty_ratio;			//占空比
//	int					m_elec_trigger_delay;		//发送时延
//	int					m_elec_send_time;			//
//
//	int					m_elec_cycles_times;		//激发次数
//	SYSTEMTIME			m_elec_gps_start_time;		//GPS时间  //32字节
//	int					m_elec_wheelpulse_times;	//测量轮脉冲次数
//	int					m_elec_gps_sync_cycle;		//GPS同步周期
//	//开始
//	float				m_elec_start_longitude;		//经度
//	unsigned short		m_elec_start_EW_longitude;	//东西经
//	float				m_elec_start_latitude;		//纬度
//	unsigned short		m_elec_start_SN_latitude;	//南北纬
//
//	//结束
//	float				m_elec_end_longitude;		//经度
//	unsigned short		m_elec_end_EW_longitude;	//东西经
//	float				m_elec_end_latitude;		//纬度
//	unsigned short		m_elec_end_SN_latitude;	//南北纬
//
//	//测量轮参数设置 34
//	bool				m_enable_trigger_wheel;			//是否使用测量轮触发
//	unsigned int		m_trigger_wheel_com_id;			//测量轮所在串口ID
//	unsigned int		m_pluses_per_trigger;				//测量轮多少脉冲触发一次
//	unsigned int		m_sample_times_per_trigger;	//测量轮触发一次要长传多少个采集请求
//	float				m_wheel_forward_length;		//测量轮前进的长度
//	float				m_wheel_radius;				//测量轮的半径
//
//	float				m_eq_velocity;			//电磁波速度
//	float				m_elec_current;				//电磁线圈电流
//
//	unsigned char		m_loop_type;				//电磁线圈类型
//	int					m_elec_sampling_rate;
//
//
//	//电磁微分脉冲采集器参数设置 29
//	int					m_dp_frequency_rank;		//DP频率等级
//	int					m_dp_sampling_rate;			//DP采样率
//	int					m_dp_cycles_times;			//DP总激发周期数
//	int					m_dp_empty_cycles_times;	//DP空激发周期数
//	int					m_dp_sendwave_type;			//DP发射波形类型
//	unsigned char		m_dp_pga;					//DP增益
//
//	//设备类型
//	int					m_device_type;				//设备类型
//	int					m_device_childtype;				//设备类型
//};*/
//#pragma pack(pop)
//
///*电流块*/
//#pragma pack(push)
//#pragma pack(1)
//struct UEP_GpsBlock 
//{
//	double longitude;// 经度
//	double latitude;// 纬度
//	double altitude;// 高程
//	double gpsDistance;// gps距离
//};
//#pragma pack(pop)

#endif // _UEP_BLOCK_H_FILE__