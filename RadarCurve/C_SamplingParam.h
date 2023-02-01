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

	 bool	m_trace_switch[USEP_PCB_NUM][USEP_PCB_TRACE_NUM];			//ͨ��

	 //���ߺ�
	 int Line_num() const { return m_line_num; }
	 void Line_num(int val) { m_line_num = val; }
	 //�ļ���
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

	//��Ųɼ�������

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

	//��ʼ��γ
	float Elec_start_longitude() const { return m_elec_start_longitude; }
	void Elec_start_longitude( float val ) { m_elec_start_longitude = val; }

	unsigned short Elec_start_EW_longitude() const { return m_elec_start_EW_longitude; }
	void Elec_start_EW_longitude( unsigned short val ) { m_elec_start_EW_longitude = val; }

	float Elec_start_latitude() const { return m_elec_start_latitude; }
	void Elec_start_latitude( float val ) { m_elec_start_latitude = val; }

	unsigned short Elec_start_SN_latitude() const { return m_elec_start_SN_latitude; }
	void Elec_start_SN_latitude( unsigned short val ) { m_elec_start_SN_latitude = val; }
	//������γ
	float Elec_end_longitude() const { return m_elec_end_longitude; }
	void Elec_end_longitude( float val ) { m_elec_end_longitude = val; }

	unsigned short Elec_end_EW_longitude() const { return m_elec_end_EW_longitude; }
	void Elec_end_EW_longitude( unsigned short val ) { m_elec_end_EW_longitude = val; }

	float Elec_end_latitude() const { return m_elec_end_latitude; }
	void Elec_end_latitude( float val ) { m_elec_end_latitude = val; }

	unsigned short Elec_end_SN_latitude() const { return m_elec_end_SN_latitude; }
	void Elec_end_SN_latitude( unsigned short val ) { m_elec_end_SN_latitude = val; }



	//�����ֲ���6��
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

	//GPSͬ������
	int GetGPSSyncCycle(int val);

	//��ò�����
	float EQ_GetSampling_rate(int sr_n);
	float Elec_GetSampling_rate(int sr_n);
	int	Elec_GetSampling_rate_Combo_cn(int sr_n);
	int	Elec_GetSampling_rate_cn(int sr_cn);

	//���Ӵ���
	unsigned int Stacking_fold() const { return m_stacking_fold; }
	void Stacking_fold(unsigned int val) { m_stacking_fold = val; }

	float EQ_velocity()const { return m_eq_velocity; }
	void EQ_velocity(float val) { m_eq_velocity = val; }

	float Elec_Current() const { return m_elec_current; }
	void Elec_Current(float val) { m_elec_current = val; }

	// ��Ȧ����
	unsigned char Elec_loop_type() const { return m_loop_type; }
	void Elec_loop_type(unsigned char val) { m_loop_type = val; }

	//�������
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

	//����豸����
	int Elec_Device_Type() const { return m_device_type; }
	void Elec_Device_Type(int val) { m_device_type = val; }
	//����豸������
	int Elec_Device_ChildType() const { return m_device_childtype; }
	void Elec_Device_ChildType(int val) { m_device_childtype = val; }

public:

	//�ɼ����ò��� 41
	int					m_line_num;//���ߺ�
	int					m_file_num;//�ļ���
	int					m_conn_mode;		//���ӷ�ʽ
	unsigned int		m_channel_num;	//ͨ����
	int					m_sampling_rate;	//������

	int					m_data_bits;			//����λ��
	unsigned char		m_sample_mode;	//�ɼ�ģʽ
	unsigned char		m_trigger_mode;	//����ģʽ
	unsigned short		m_points_num;		//��������
	bool				m_buf_switch;		//����������

	unsigned char		m_pga[USEP_PCB_NUM];		//��������
	unsigned char		pga;
	bool				m_adc_check[USEP_PCB_NUM];	//ϵͳ�Լ�
	bool				m_adc_adjust;	//ϵͳУ׼
	unsigned int		m_stacking_fold;
	int					m_trigger_delay;		//����ʱ��

	//��Ųɼ�����24
	int					m_elec_single_sample_mode;
	int					m_elec_trigger_mode;
	int					m_elec_highspeed_mode;
	int					m_elec_data_bits;
	int					m_elec_sample_enable;
	unsigned short		m_elec_sample_points_num;

	//��Ųɼ����������� 114
	int					m_elec_frequency_rank;		//Ƶ�ʵȼ�
	int					m_elec_duty_ratio;			//ռ�ձ�
	int					m_elec_trigger_delay;		//����ʱ��
	int					m_elec_send_time;			//

	int					m_elec_cycles_times;		//��������
	SYSTEMTIME			m_elec_gps_start_time;		//GPSʱ��  //32�ֽ�
	int					m_elec_wheelpulse_times;	//�������������
	int					m_elec_gps_sync_cycle;		//GPSͬ������
	//��ʼ
	float				m_elec_start_longitude;		//����
	unsigned short		m_elec_start_EW_longitude;	//������
	float				m_elec_start_latitude;		//γ��
	unsigned short		m_elec_start_SN_latitude;	//�ϱ�γ

	//����
	float				m_elec_end_longitude;		//����
	unsigned short		m_elec_end_EW_longitude;	//������
	float				m_elec_end_latitude;		//γ��
	unsigned short		m_elec_end_SN_latitude;	//�ϱ�γ

	//�����ֲ������� 57
	bool				m_enable_trigger_wheel;			//�Ƿ�ʹ�ò����ִ���
	unsigned int		m_trigger_wheel_com_id;			//���������ڴ���ID
	unsigned int		m_pluses_per_trigger;				//�����ֶ������崥��һ��
	unsigned int		m_sample_times_per_trigger;	//�����ִ���һ��Ҫ�������ٸ��ɼ�����
	float				m_wheel_forward_length;		//������ǰ���ĳ���
	float				m_wheel_radius;				//�����ֵİ뾶

	float				m_eq_velocity;			//��Ų��ٶ�
	float				m_elec_current;				//�����Ȧ����

	unsigned char		m_loop_type;				//�����Ȧ����
	int					m_elec_sampling_rate;


	//���΢������ɼ�����������

	int					m_dp_frequency_rank;		//DPƵ�ʵȼ�
	int					m_dp_sampling_rate;			//DP������
	int					m_dp_cycles_times;			//DP�ܼ���������
	int					m_dp_empty_cycles_times;	//DP�ռ���������
	int					m_dp_sendwave_type;			//DP���䲨������
	unsigned char		m_dp_pga;					//DP����

	//�豸����
	int					m_device_type;				//�豸����
	int					m_device_childtype;				//�豸����

};// ���� 

extern C_SamplingParam g_sample_param; //�ɼ�ϵͳ����
// extern C_SamplingParam g_eq_tg_sample_param;//���𴥷��ɼ�����
// extern C_SamplingParam g_eq_rt_sample_param;//����ʵʱ�ɼ�����
// extern C_SamplingParam g_eq_nm_sample_param;//���𵥴βɼ�����
// extern C_SamplingParam g_uep_sample_param;//��Ŵ����ɼ�����
//extern C_SamplingParam g_uep_sample_param;//���ʵʱ�ɼ�����
//extern C_SamplingParam g_uep_sample_param;//��ŵ��βɼ�����

#endif