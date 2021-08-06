
#ifndef __QRCODE_H_
#define __QRCODE_H_

#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int

#define QRVersion u8

#define CorrectionLevel_L 1
#define CorrectionLevel_M 2
#define CorrectionLevel_Q 3
#define CorrectionLevel_H 4
#define QR_CORRECTION_LEVEL CorrectionLevel_M


#define Mode_ECI 0x07
#define Mode_Num 0x01
#define Mode_Char 0x02
#define Mode_Byte 0x04
#define Mode_CN 0x0D
#define Mode_StructLink 0x03
#define Mode_FNC1 0x05
#define Mode_FNC2 0x09
#define QR_ENCODE_MODE Mode_Char

//#define LOG




#define QRError_MEMSIZE 0x01


typedef struct {

	u16 MemLen;
	u16 FlowLen;
	u8* Flow;

}BitFlow;

typedef struct {

	u8 a;//

}DataCodeTypedef;


typedef struct {

	u8 BlockLen;
	u8 InfoLen;
	u8* OperaPoly;  
	u8* GenPoly;

}CorEncodeTypedef;

typedef struct {

	u8 BlockNum;		//总块数
	u8* BlockInfo;		//每块信息(数据码长度，纠错码长度) 第一块[0:1] 第二块[2:3] ......
	u8** InfoCode;		//数据码	
	u8** CorCode;		//纠错码

}BlockCode;

typedef struct {

	u8 Height;
	u8 Wide;
	u8* Pat;

}Pattern;


typedef struct {

	u8 Version;
	u8 CorLevel;
	u8 EncodeMode;
	u8 Mask;
	u8** bitFlow;

}QRCode;

QRCode qrcode;


void QR_Config(u8 CorLevel,u8 Mask);

void QR_Encode(u8* raw);

void QR_CodePrinter(QRCode qr);

QRVersion QR_GetVersion(u8 *raw);

void QR_AnaData(u8 *raw,u16 len);

void QR_DataCodeEncode(CorEncodeTypedef** blocks, BitFlow* dataFlow);

CorEncodeTypedef** QR_GetBlocks(u8 version,u8 corlevel);

void QR_CorCodeEncode(CorEncodeTypedef* cb);

u8* QR_CharEncode(u8 *raw, QRCode* qrcode);

void QR_SetData(u8** qrFlow, u8** mask,BitFlow* dataFlow,u8 width);

void QR_SetVersion(QRCode qr);

u8* QR_GenPloy(u8 CorNum);

void QR_SetFormat(u8** flow,u8** mask,u8 version);

void QR_SetDot(u8** flow,u8 x,u8 y);

void QR_SetMaskDot(u8** flow, u8 x, u8 y,u16 data);

void QR_PastePattern(u8* pat,u8 x,u8 y,u8** flow,u8** mask);


void Test(void);



u8 BitFlowAppend(BitFlow* bf, u8* appendFlow, u16 len);
u8 BitFlowStrAppend(BitFlow* bf, u8* str);

void BitFlowFill(BitFlow* bf);

void BitFlowPrint(u8* flow,u16 len);






#endif // !__QRCODE_H_

