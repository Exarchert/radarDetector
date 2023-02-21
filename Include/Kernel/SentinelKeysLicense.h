/***********************************************************************************************
 * ����        : SentinelKeysLicense.h
 * ����֤      : LicenseSharing
 * ������      : Sentinel Keys  v1.0.1
 * ����        : ���ڹ��߰�����֤���Ե�ͷ�ļ���
 * Ŀ��        : ��Ҫ�����ļ����뵽���ĳ�������Ӧ������֤���ԡ�

 * ����        : 12/17/2010 (MM/DD/YYYY)

 * ���� - �����ļ��������������κα༭�����ܱ����ǣ�
 ***********************************************************************************************/

 #include "SentinelKeys.h" 

#define  DEVELOPERID                                         0xA90B4F0A

#define  LICENSEID                                           0x84DF
#define  SP_LICENSESHARING_AES                               0x1

/*  ������Կ: */

unsigned char SOFTWARE_KEY [SP_SOFTWARE_KEY_LEN] = {
 0x49, 0x21, 0xB7, 0xC0, 0x18, 0xD7, 0xD5, 0xC4, 0x0A, 0xBC, 0x4E, 0xD2, 0xFE, 0xEE, 0x8C, 0x53,
 0x33, 0xC7, 0x7D, 0x94, 0x4A, 0xA5, 0x33, 0x39, 0xFE, 0xCA, 0x70, 0x0F, 0xD0, 0x6D, 0x1C, 0xF3,
 0x20, 0x6A, 0x7F, 0x86, 0xCB, 0xED, 0x98, 0xB6, 0x03, 0x27, 0x54, 0x3D, 0x7A, 0x21, 0xCC, 0x25,
 0x8B, 0x00, 0xF7, 0xDF, 0x8E, 0x05, 0xF0, 0x57, 0x38, 0x66, 0x0B, 0x38, 0x17, 0x3B, 0x38, 0xF2,
 0x60, 0x36, 0x3B, 0x7A, 0xBD, 0xA9, 0x70, 0x3F, 0x90, 0x11, 0x56, 0xAA, 0x7A, 0x50, 0x72, 0xB0,
 0x91, 0x1F, 0x2B, 0x24, 0x48, 0x1D, 0x2C, 0xED, 0xEF, 0x87, 0xBA, 0xC1, 0xC4, 0x39, 0x4A, 0xB6,
 0x36, 0xE4, 0xC7, 0x36, 0xEC, 0xC9, 0xCA, 0x0A, 0x31, 0x7D, 0xE4, 0xFC, 0xE8, 0xC5, 0xB5, 0x3A
 };


/* ��ѯ-��Ӧ����:     20*/
#define SP_NUM_OF_QR                               20

/* ��ѯ���ݳ���:     4*/
#define SP_LEN_OF_QR                               4

/* �˲�ѯ-��Ӧ���ɺ�����������:   LicenseSharing */
unsigned char QUERY_SP_LICENSESHARING_AES_SP_SIMPLE_QUERY[SP_NUM_OF_QR]  [SP_LEN_OF_QR] = {
{0x13,0x2A,0x35,0x3E},
{0x76,0x37,0x2F,0x19},
{0x44,0x57,0x08,0x15},
{0x0E,0x58,0x02,0x36},
{0x7A,0x3F,0x6F,0x72},
{0x37,0x72,0x00,0x58},
{0x0C,0x1C,0x4F,0x07},
{0x51,0x08,0x6D,0x4E},
{0x1C,0x45,0x3A,0x61},
{0x64,0x45,0x16,0x2B},
{0x6A,0x27,0x42,0x36},
{0x13,0x67,0x5F,0x31},
{0x04,0x0B,0x1B,0x4D},
{0x26,0x16,0x47,0x24},
{0x1A,0x14,0x61,0x36},
{0x1C,0x60,0x11,0x16},
{0x06,0x19,0x7C,0x21},
{0x61,0x50,0x39,0x0A},
{0x51,0x3D,0x20,0x55},
{0x23,0x64,0x67,0x74}
};
unsigned char RESPONSE_SP_LICENSESHARING_AES_SP_SIMPLE_QUERY[SP_NUM_OF_QR] [SP_LEN_OF_QR] = {
{0x70,0x10,0x16,0x99},
{0x57,0x75,0x7C,0xAC},
{0x5C,0x2B,0xBC,0xC3},
{0xB4,0x13,0x38,0x21},
{0x3B,0x51,0x87,0xD4},
{0x3A,0x5E,0xC5,0x77},
{0x6B,0xEF,0x55,0x19},
{0x45,0x68,0x5C,0xE1},
{0xFD,0x73,0x80,0x52},
{0xEB,0xC5,0xD3,0x23},
{0xE2,0x69,0x64,0x2B},
{0x00,0x80,0xDB,0x19},
{0x69,0x6F,0x5C,0xE1},
{0x8C,0x58,0xB8,0x10},
{0x1D,0x8C,0x2E,0x60},
{0x6E,0x1C,0xA1,0x19},
{0x45,0x19,0x31,0x05},
{0xEB,0x3A,0xE2,0x60},
{0xC6,0x32,0xAC,0xA7},
{0x29,0xE8,0xF4,0x05}
};
