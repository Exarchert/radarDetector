#ifndef __UEP_BLOCK_H_FILE__
#define __UEP_BLOCK_H_FILE__


//****************************** UEP **************************************//
/* uep�����ݿ�ṹ�� */

/*UEP�ļ��ܾ�ͷ*/
#pragma pack(push)
#pragma pack(1)
struct UEP_ReelBlock // *.uep
{
	DWORD workTag;//��ҵ��ʶ�ţ�����ʶ���룩
	DWORD lineNum;//���ߺ�

	DWORD triggerSum;//��������
	short traceSum;//ÿ����¼�ĵ���
	short frameSum;//�þ�ÿ���Ĳ�������
    
	float sampleInterval;//�þ�������
	float samplingTimelapse;//ԭʼ��¼������ʱ
	
	short sendtime;// ����ʱ��

	////////////////////////////////////////////////////////////
	//						������չ  �ɼ����� �� ���
	////////////////////////////////////////////////////////////
	int  m_has_sample_param;		//�Ƿ��вɼ�����
	int	 m_sample_param_pos;		//�ɼ����������ļ���ͷ��λ��
	int	 m_sample_param_size;		//�ɼ������ܴ�С

	int	 m_has_tags;						//�Ƿ��С��жϱꡱ
	int	 m_tags_pos;						//���жϱꡱ�����ļ���ͷ��λ��
	int	 m_tags_num;					//�ж��١��жϱꡱ
	int	 m_tags_begin_val;			//���ݰ���ʼ�ɼ�ʱ��־ֵ
	int	 m_tags_end_val;				//���ݰ���ֹ�ɼ�ʱ��־ֵ
	int	 m_is_time_tag;					//�Ƿ�Ϊʱ���
	int	 m_val_per_ch;					//ÿһ���������ݼ�����ٱ�ֵ


	int	 m_has_gps_gdjw;					//�Ƿ��й��羭γGPS
	 
	// ʣ���Զ���
	char untapped[330];	
};//��400�ֽ�
#pragma pack(pop)

///* ������ͷ��¼�飺һ������60�ֽڣ��������300�ֽ� */
//#pragma pack(push)
//#pragma pack(1)
//struct UEP_NewMastBlock 
//{
//	BOOL	bSwitch;		//1
//
//	int	nCycleNum;			//2 ��Ȧ����
//	
//	float fLength;			//3 ��
//	float fWidth;			//4 ��
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
//// һ��60���ֽ�
//};
//#pragma pack(pop)
//
///* ������ͷ��¼�飺600�ֽ� */
//#pragma pack(push)
//#pragma pack(1)
//struct UEP_MastBlock 
//{
//	// ������Ȧ
//	UEP_NewMastBlock UEPSendMast[5];	// 300�ֽ�
//	// ������Ȧ
//	UEP_NewMastBlock UEPRcvMast[5];		// 300�ֽ�
//
//	UEP_MastBlock()
//	{
//		// ������Ȧ1
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
//		// ������Ȧ1
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
///*������*/
//#pragma pack(push)
//#pragma pack(1)
//struct UEP_HeadBlock 
//{
//	int EmissionCurrent;// �������
//	int TurnoffCurrent;// �ضϵ���
//	int TransitionCurrent[20];// ���ɵ���
//};
//#pragma pack(pop)
//
///*�������ÿ�*/
//#pragma pack(push)
//#pragma pack(1)
//struct UEP_SamplingParamBlock {
//	bool	m_trace_switch[5][64];			//ͨ��
//	 //�ɼ����ò��� 49
//	int					m_line_num;//���ߺ�
//	int					m_file_num;//�ļ���
//	int					m_conn_mode;		//���ӷ�ʽ
//	unsigned int		m_channel_num;	//ͨ����
//	int					m_sampling_rate;	//������
//
//	int					m_data_bits;			//����λ�� 
//	unsigned char		m_sample_mode;	//�ɼ�ģʽ
//	unsigned char		m_trigger_mode;	//����ģʽ
//	unsigned short		m_points_num;		//��������
//	bool				m_buf_switch;		//����������
//
//	unsigned char		m_pga[5];		//��������
//	unsigned char		pga;
//	bool				m_adc_check[5];	//ϵͳ�Լ� 
//	bool				m_adc_adjust;	//ϵͳУ׼
//	unsigned int		m_stacking_fold;
//	int					m_trigger_delay;		//����ʱ��
//
//	//��Ųɼ�����22
//	int					m_elec_single_sample_mode;
//	int					m_elec_trigger_mode;
//	int					m_elec_highspeed_mode;
//	int					m_elec_data_bits;
//	int					m_elec_sample_enable;
//	unsigned short		m_elec_sample_points_num;
//
//	//��Ųɼ����������� 84
//	int					m_elec_frequency_rank;		//Ƶ�ʵȼ�
//	int					m_elec_duty_ratio;			//ռ�ձ�
//	int					m_elec_trigger_delay;		//����ʱ��
//	int					m_elec_send_time;			//
//
//	int					m_elec_cycles_times;		//��������
//	SYSTEMTIME			m_elec_gps_start_time;		//GPSʱ��  //32�ֽ�
//	int					m_elec_wheelpulse_times;	//�������������
//	int					m_elec_gps_sync_cycle;		//GPSͬ������
//	//��ʼ
//	float				m_elec_start_longitude;		//����
//	unsigned short		m_elec_start_EW_longitude;	//������
//	float				m_elec_start_latitude;		//γ��
//	unsigned short		m_elec_start_SN_latitude;	//�ϱ�γ
//
//	//����
//	float				m_elec_end_longitude;		//����
//	unsigned short		m_elec_end_EW_longitude;	//������
//	float				m_elec_end_latitude;		//γ��
//	unsigned short		m_elec_end_SN_latitude;	//�ϱ�γ
//
//	//�����ֲ������� 34
//	bool				m_enable_trigger_wheel;			//�Ƿ�ʹ�ò����ִ���
//	unsigned int		m_trigger_wheel_com_id;			//���������ڴ���ID
//	unsigned int		m_pluses_per_trigger;				//�����ֶ������崥��һ��
//	unsigned int		m_sample_times_per_trigger;	//�����ִ���һ��Ҫ�������ٸ��ɼ�����
//	float				m_wheel_forward_length;		//������ǰ���ĳ���
//	float				m_wheel_radius;				//�����ֵİ뾶
//
//	float				m_eq_velocity;			//��Ų��ٶ�
//	float				m_elec_current;				//�����Ȧ����
//
//	unsigned char		m_loop_type;				//�����Ȧ����
//	int					m_elec_sampling_rate;
//
//
//	//���΢������ɼ����������� 29
//	int					m_dp_frequency_rank;		//DPƵ�ʵȼ�
//	int					m_dp_sampling_rate;			//DP������
//	int					m_dp_cycles_times;			//DP�ܼ���������
//	int					m_dp_empty_cycles_times;	//DP�ռ���������
//	int					m_dp_sendwave_type;			//DP���䲨������
//	unsigned char		m_dp_pga;					//DP����
//
//	//�豸����
//	int					m_device_type;				//�豸����
//	int					m_device_childtype;				//�豸����
//};
///*struct UEP_SamplingParamBlock 
//{
//	bool	m_trace_switch[5][64];			//ͨ��
//	 //�ɼ����ò��� 41
//	int					m_line_num;//���ߺ�
//	int					m_file_num;//�ļ���
//	int					m_conn_mode;		//���ӷ�ʽ
//	unsigned int		m_channel_num;	//ͨ����
//	int					m_sampling_rate;	//������
//
//	int					m_data_bits;			//����λ��
//	unsigned char		m_sample_mode;	//�ɼ�ģʽ
//	unsigned char		m_trigger_mode;	//����ģʽ
//	unsigned short		m_points_num;		//��������
//	bool				m_buf_switch;		//����������
//
//	unsigned char		m_pga[5];		//��������
//	unsigned char		pga;
//	bool				m_adc_check[5];	//ϵͳ�Լ�
//	bool				m_adc_adjust;	//ϵͳУ׼
//	unsigned int		m_stacking_fold;
//	int					m_trigger_delay;		//����ʱ��
//
//	//��Ųɼ�����22
//	int					m_elec_single_sample_mode;
//	int					m_elec_trigger_mode;
//	int					m_elec_highspeed_mode;
//	int					m_elec_data_bits;
//	int					m_elec_sample_enable;
//	unsigned short		m_elec_sample_points_num;
//
//	//��Ųɼ����������� 84
//	int					m_elec_frequency_rank;		//Ƶ�ʵȼ�
//	int					m_elec_duty_ratio;			//ռ�ձ�
//	int					m_elec_trigger_delay;		//����ʱ��
//	int					m_elec_send_time;			//
//
//	int					m_elec_cycles_times;		//��������
//	SYSTEMTIME			m_elec_gps_start_time;		//GPSʱ��  //32�ֽ�
//	int					m_elec_wheelpulse_times;	//�������������
//	int					m_elec_gps_sync_cycle;		//GPSͬ������
//	//��ʼ
//	float				m_elec_start_longitude;		//����
//	unsigned short		m_elec_start_EW_longitude;	//������
//	float				m_elec_start_latitude;		//γ��
//	unsigned short		m_elec_start_SN_latitude;	//�ϱ�γ
//
//	//����
//	float				m_elec_end_longitude;		//����
//	unsigned short		m_elec_end_EW_longitude;	//������
//	float				m_elec_end_latitude;		//γ��
//	unsigned short		m_elec_end_SN_latitude;	//�ϱ�γ
//
//	//�����ֲ������� 34
//	bool				m_enable_trigger_wheel;			//�Ƿ�ʹ�ò����ִ���
//	unsigned int		m_trigger_wheel_com_id;			//���������ڴ���ID
//	unsigned int		m_pluses_per_trigger;				//�����ֶ������崥��һ��
//	unsigned int		m_sample_times_per_trigger;	//�����ִ���һ��Ҫ�������ٸ��ɼ�����
//	float				m_wheel_forward_length;		//������ǰ���ĳ���
//	float				m_wheel_radius;				//�����ֵİ뾶
//
//	float				m_eq_velocity;			//��Ų��ٶ�
//	float				m_elec_current;				//�����Ȧ����
//
//	unsigned char		m_loop_type;				//�����Ȧ����
//	int					m_elec_sampling_rate;
//
//
//	//���΢������ɼ����������� 29
//	int					m_dp_frequency_rank;		//DPƵ�ʵȼ�
//	int					m_dp_sampling_rate;			//DP������
//	int					m_dp_cycles_times;			//DP�ܼ���������
//	int					m_dp_empty_cycles_times;	//DP�ռ���������
//	int					m_dp_sendwave_type;			//DP���䲨������
//	unsigned char		m_dp_pga;					//DP����
//
//	//�豸����
//	int					m_device_type;				//�豸����
//	int					m_device_childtype;				//�豸����
//};*/
//#pragma pack(pop)
//
///*������*/
//#pragma pack(push)
//#pragma pack(1)
//struct UEP_GpsBlock 
//{
//	double longitude;// ����
//	double latitude;// γ��
//	double altitude;// �߳�
//	double gpsDistance;// gps����
//};
//#pragma pack(pop)

#endif // _UEP_BLOCK_H_FILE__