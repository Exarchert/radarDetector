#ifndef  H_TMIGS_C_SAMPLINGPARAM
#define  H_TMIGS_C_SAMPLINGPARAM


#define USEP_PCB_NUM		5
#define USEP_PCB_CHILD_NUM	4
#define USEP_PCB_TRACE_NUM	64
#define USEP_VELOCITY		300
#define USEP_MIN_VELOCITY	10


class C_SamplingParam
{
public:
	C_SamplingParam(void);
     ~C_SamplingParam(void);

	 bool	m_trace_switch[USEP_PCB_NUM][USEP_PCB_TRACE_NUM];			//通道

	 //测线号
	 int Line_num() const { return m_line_num; }
	 void Line_num(int val) { m_line_num = val; }
	 //文件号
	 int File_num() const { return m_file_num; }
	 void File_num(int val) { m_file_num = val; }

	int Conn_mode() const { return m_conn_mode; }
	void Conn_mode(int val) { m_conn_mode = val; }

	unsigned int Channel_num() const { return m_channel_num; }
	void Channel_num(unsigned int val) { m_channel_num = val; }

	int Sampling_rate_sn() const { return m_sampling_rate; }
	void Sampling_rate_sn(int val) { m_sampling_rate = val; }

	int Data_bits() const { return m_data_bits; }
	void Data_bits(int val) { m_data_bits = val; }

	unsigned char Sample_mode() const { return m_sample_mode; }
	void Sample_mode(unsigned char val) { m_sample_mode = val; }

	unsigned char Trigger_mode() const { return m_trigger_mode; }
	void Trigger_mode(unsigned char val) { m_trigger_mode = val; }

	unsigned short Points_num() const { return m_points_num; }
	void Points_num(unsigned short val) { m_points_num = val; }

	bool Buf_switch() const { return m_buf_switch; }
	void Buf_switch(bool val) { m_buf_switch = val; }

	unsigned char GetPga(int indx) const { return m_pga[indx]; }
	void SetPga(unsigned char val, int indx) { m_pga[indx] = val; }

	unsigned char Pga() const { return pga; }
	void Pga(unsigned char val) { pga = val; }

	bool GetADC_check(int indx) const { return m_adc_check[indx]; }
	void SetADC_check(bool val, int indx) { m_adc_check[indx] = val; }

	bool ADC_adjust() const { return m_adc_adjust;}
	void ADC_adjust(bool val) { m_adc_adjust = val; }

	int Trigger_delay() const { return m_trigger_delay; }
	void Trigger_delay(int val) { m_trigger_delay = val; }

	//电磁采集参数集

	int Elec_single_sample_mode() const { return m_elec_single_sample_mode; }
	void Elec_single_sample_mode(int val) { m_elec_single_sample_mode = val; }

	int Elec_trigger_mode() const { return m_elec_trigger_mode; }
	void Elec_trigger_mode(int val) { m_elec_trigger_mode = val; }

	int Elec_highspeed_mode() const { return m_elec_highspeed_mode; }
	void Elec_highspeed_mode(int val) { m_elec_highspeed_mode = val; }

	int Elec_data_bits() const { return m_elec_data_bits; }
	void Elec_data_bits(int val) { m_elec_data_bits = val; }

	int Elec_sample_enable() const { return m_elec_sample_enable; }
	void Elec_sample_enable(int val) { m_elec_sample_enable = val; }

	unsigned short Elec_sample_points_num() const { return m_elec_sample_points_num; }
	void Elec_sample_points_num(unsigned short val) { m_elec_sample_points_num = val; }

	int Elec_Sampling_rate_sn() const { return m_elec_sampling_rate; }
	void Elec_Sampling_rate_sn(int val) { m_elec_sampling_rate = val; }

	int Elec_frequency_rank() const { return m_elec_frequency_rank; }
	void Elec_frequency_rank(int val) { m_elec_frequency_rank = val; }

	int Elec_cycles_times() const { return m_elec_cycles_times; }
	void Elec_cycles_times(int val) { m_elec_cycles_times = val; }

	int Elec_send_time() const { return m_elec_send_time; }
	void Elec_send_time(int val) { m_elec_send_time = val; }

	int Elec_trigger_delay() const { return m_elec_trigger_delay; }
	void Elec_trigger_delay(int val) { m_elec_trigger_delay = val; }

	int Elec_duty_ratio() const { return m_elec_duty_ratio; }
	void Elec_duty_ratio(int val) { m_elec_duty_ratio = val; }

	SYSTEMTIME Elec_gps_start_time() const { return m_elec_gps_start_time; }
	void Elec_gps_start_time(SYSTEMTIME val) { m_elec_gps_start_time = val; }

	int Elec_wheelpulse_times() const { return m_elec_wheelpulse_times; }
	void Elec_wheelpulse_times(int val)	{ m_elec_wheelpulse_times = val; }

	int Elec_gps_sync_cycle() const { return m_elec_gps_sync_cycle; }
	void Elec_gps_sync_cycle( int val ) { m_elec_gps_sync_cycle = val; }

	//开始经纬
	float Elec_start_longitude() const { return m_elec_start_longitude; }
	void Elec_start_longitude( float val ) { m_elec_start_longitude = val; }

	unsigned short Elec_start_EW_longitude() const { return m_elec_start_EW_longitude; }
	void Elec_start_EW_longitude( unsigned short val ) { m_elec_start_EW_longitude = val; }

	float Elec_start_latitude() const { return m_elec_start_latitude; }
	void Elec_start_latitude( float val ) { m_elec_start_latitude = val; }

	unsigned short Elec_start_SN_latitude() const { return m_elec_start_SN_latitude; }
	void Elec_start_SN_latitude( unsigned short val ) { m_elec_start_SN_latitude = val; }
	//结束经纬
	float Elec_end_longitude() const { return m_elec_end_longitude; }
	void Elec_end_longitude( float val ) { m_elec_end_longitude = val; }

	unsigned short Elec_end_EW_longitude() const { return m_elec_end_EW_longitude; }
	void Elec_end_EW_longitude( unsigned short val ) { m_elec_end_EW_longitude = val; }

	float Elec_end_latitude() const { return m_elec_end_latitude; }
	void Elec_end_latitude( float val ) { m_elec_end_latitude = val; }

	unsigned short Elec_end_SN_latitude() const { return m_elec_end_SN_latitude; }
	void Elec_end_SN_latitude( unsigned short val ) { m_elec_end_SN_latitude = val; }



	//测量轮参数6个
	bool Enable_trigger_wheel() const { return m_enable_trigger_wheel; }
	void Enable_trigger_wheel(bool val) { m_enable_trigger_wheel = val; }

	unsigned int Trigger_wheel_com_id() const { return m_trigger_wheel_com_id; }
	void Trigger_wheel_com_id(unsigned int val) { m_trigger_wheel_com_id = val; }

	unsigned int Pluses_per_trigger() const { return m_pluses_per_trigger; }
	void Pluses_per_trigger(unsigned int val) { m_pluses_per_trigger = val; }

	unsigned int Sample_times_per_trigger() const { return m_sample_times_per_trigger; }
	void Sample_times_per_trigger(unsigned int val) { m_sample_times_per_trigger = val; }

	float Wheel_Forward_Length() const { return m_wheel_forward_length; }
	void Wheel_Forward_Length(float val) { m_wheel_forward_length = val; }

	float Wheel_Radius()const { return m_wheel_radius; }
	void Wheel_Radius(float val) { m_wheel_radius = val; }


//	float GetEquiptimeLapse(int SPS);

	//GPS同步周期
	int GetGPSSyncCycle(int val);

	//获得采样率
	float EQ_GetSampling_rate(int sr_n);
	float Elec_GetSampling_rate(int sr_n);
	int	Elec_GetSampling_rate_Combo_cn(int sr_n);
	int	Elec_GetSampling_rate_cn(int sr_cn);

	//叠加次数
	unsigned int Stacking_fold() const { return m_stacking_fold; }
	void Stacking_fold(unsigned int val) { m_stacking_fold = val; }

	float EQ_velocity()const { return m_eq_velocity; }
	void EQ_velocity(float val) { m_eq_velocity = val; }

	float Elec_Current() const { return m_elec_current; }
	void Elec_Current(float val) { m_elec_current = val; }

	// 线圈类型
	unsigned char Elec_loop_type() const { return m_loop_type; }
	void Elec_loop_type(unsigned char val) { m_loop_type = val; }

	//获得增益
	int Elec_Get_Combo_Pga( int realpga );
	int Elec_Get_Real_Pga( int combopga );


	//DP
	int DP_Frequency_rank_sn() const { return m_dp_frequency_rank; }
	void DP_Frequency_rank_sn(int val) { m_dp_frequency_rank = val; }

	int DP_Sampling_rate_sn() const { return m_dp_sampling_rate; }
	void DP_Sampling_rate_sn(int val) { m_dp_sampling_rate = val; }

	int DP_Cycles_times() const { return m_dp_cycles_times; }
	void DP_Cycles_times(int val) { m_dp_cycles_times = val; }

	int DP_Empty_cycles_times() const { return m_dp_empty_cycles_times; }
	void DP_Empty_cycles_times(int val) { m_dp_empty_cycles_times = val; }

	int DP_Sendwave_type() const { return m_dp_sendwave_type; }
	void DP_Sendwave_type(int val) { m_dp_sendwave_type = val; }

	unsigned char DP_Pga_sn() const { return m_dp_pga; }
	void DP_Pga_sn(unsigned char val) { m_dp_pga = val; }

	//电磁设备类型
	int Elec_Device_Type() const { return m_device_type; }
	void Elec_Device_Type(int val) { m_device_type = val; }
	//电磁设备子类型
	int Elec_Device_ChildType() const { return m_device_childtype; }
	void Elec_Device_ChildType(int val) { m_device_childtype = val; }

public:

	//采集常用参数 41
	int					m_line_num;//测线号
	int					m_file_num;//文件号
	int					m_conn_mode;		//连接方式
	unsigned int		m_channel_num;	//通道数
	int					m_sampling_rate;	//采样率

	int					m_data_bits;			//数据位数
	unsigned char		m_sample_mode;	//采集模式
	unsigned char		m_trigger_mode;	//触发模式
	unsigned short		m_points_num;		//采样点数
	bool				m_buf_switch;		//缓冲区开关

	unsigned char		m_pga[USEP_PCB_NUM];		//各板增益
	unsigned char		pga;
	bool				m_adc_check[USEP_PCB_NUM];	//系统自检
	bool				m_adc_adjust;	//系统校准
	unsigned int		m_stacking_fold;
	int					m_trigger_delay;		//发送时延

	//电磁采集参数24
	int					m_elec_single_sample_mode;
	int					m_elec_trigger_mode;
	int					m_elec_highspeed_mode;
	int					m_elec_data_bits;
	int					m_elec_sample_enable;
	unsigned short		m_elec_sample_points_num;

	//电磁采集器参数设置 114
	int					m_elec_frequency_rank;		//频率等级
	int					m_elec_duty_ratio;			//占空比
	int					m_elec_trigger_delay;		//发送时延
	int					m_elec_send_time;			//

	int					m_elec_cycles_times;		//激发次数
	SYSTEMTIME			m_elec_gps_start_time;		//GPS时间  //32字节
	int					m_elec_wheelpulse_times;	//测量轮脉冲次数
	int					m_elec_gps_sync_cycle;		//GPS同步周期
	//开始
	float				m_elec_start_longitude;		//经度
	unsigned short		m_elec_start_EW_longitude;	//东西经
	float				m_elec_start_latitude;		//纬度
	unsigned short		m_elec_start_SN_latitude;	//南北纬

	//结束
	float				m_elec_end_longitude;		//经度
	unsigned short		m_elec_end_EW_longitude;	//东西经
	float				m_elec_end_latitude;		//纬度
	unsigned short		m_elec_end_SN_latitude;	//南北纬

	//测量轮参数设置 57
	bool				m_enable_trigger_wheel;			//是否使用测量轮触发
	unsigned int		m_trigger_wheel_com_id;			//测量轮所在串口ID
	unsigned int		m_pluses_per_trigger;				//测量轮多少脉冲触发一次
	unsigned int		m_sample_times_per_trigger;	//测量轮触发一次要长传多少个采集请求
	float				m_wheel_forward_length;		//测量轮前进的长度
	float				m_wheel_radius;				//测量轮的半径

	float				m_eq_velocity;			//电磁波速度
	float				m_elec_current;				//电磁线圈电流

	unsigned char		m_loop_type;				//电磁线圈类型
	int					m_elec_sampling_rate;


	//电磁微分脉冲采集器参数设置

	int					m_dp_frequency_rank;		//DP频率等级
	int					m_dp_sampling_rate;			//DP采样率
	int					m_dp_cycles_times;			//DP总激发周期数
	int					m_dp_empty_cycles_times;	//DP空激发周期数
	int					m_dp_sendwave_type;			//DP发射波形类型
	unsigned char		m_dp_pga;					//DP增益

	//设备类型
	int					m_device_type;				//设备类型
	int					m_device_childtype;				//设备类型

};// 参数 

extern C_SamplingParam g_sample_param; //采集系统参数
// extern C_SamplingParam g_eq_tg_sample_param;//地震触发采集参数
// extern C_SamplingParam g_eq_rt_sample_param;//地震实时采集参数
// extern C_SamplingParam g_eq_nm_sample_param;//地震单次采集参数
// extern C_SamplingParam g_uep_sample_param;//电磁触发采集参数
//extern C_SamplingParam g_uep_sample_param;//电磁实时采集参数
//extern C_SamplingParam g_uep_sample_param;//电磁单次采集参数

#endif