#include "QRCode.h"
#include <stdio.h>


const u16 MaxNumLen[5][11] = { {208,359,567,807,1079,1383,1568,1936,2336,2768,3232} ,//数据位数
	{25,47,77,114,154,195,224,279,335,395,468} ,//最大字母字符位数
	{20,38,61,90,112,154,178,221,262,311,366},
	{16,29,47,67,87,108,125,157,189,221,259},
	{10,20,35,50,64,84,93,122,143,174,200}
};

//const u16 DataNum[4][11] = {	//各纠错版本数据码数
//	{19,34,55,80,108,136,156,194,232,274,324},
//	{16,28,44,64,86,108,124,154,182,216,254},
//	{13,22,34,48,62,76,88,110,132,154,180},
//	{9,16,26,36,46,60,66,86,100,122,140}
//};


// 码字分块表
//先分等级在按纠错级别填入 级别索引[0:3]
//(低四位为第一个块位置，高四位为该纠错级别块类数)
//后接分块详情 三个字符一组[块数，块总码数，块数据码数]
const u8 V1[] = {0x11,0x12,0x13,0x14,1,26,19,1,26,16,1,26,13,1,26,9};
const u8 V2[] = {0x11,0x12,0x13,0x14,1,44,34,1,44,28,1,44,22,1,44,16};
const u8 V3[] = {0x11,0x12,0x13,0x14,1,70,55,1,70,44,2,35,17,2,35,13};
const u8 V4[] = {0x11,0x12,0x13,0x14,1,100,80,2,50,32,2,50,24,4,25,9};
const u8 V5[] = {0x11,0x12,0x23,0x25,1,134,108,2,67,43,2,33,15,2,34,16,2,33,11,2,34,12};
const u8 V6[] = {0x11,0x12,0x13,0x14,2,86,68,4,43,27,4,43,19,4,43,15};
const u8 V7[] = {0x11,0x12,0x23,0x25,2,98,78,4,49,31,2,32,14,4,33,15,4,39,13,1,40,14};
const u8 V8[] = {0x11};//有空补吧
const u8 V9[1];
const u8 V10[1];
const u8 V11[1];
const u8* BlockDivide[]={V1,V2,V3,V4,V5,V6,V7,V8,V9,V10,V11};

const u8 GaloisToInt[] = {
	1,2, 4, 8, 16, 32, 64, 128, 29, 58, 116, 232, 205, 135, 19, 38, 76, 152, 45, 90, 180,
117, 234, 201, 143, 3, 6, 12, 24, 48, 96, 192, 157, 39, 78, 156, 37, 74, 148, 53, 106,
212, 181, 119, 238, 193, 159, 35, 70, 140, 5, 10, 20, 40, 80, 160, 93, 186, 105, 210, 185,
111, 222, 161, 95, 190, 97, 194, 153, 47, 94, 188, 101, 202, 137, 15, 30, 60, 120, 240, 253,
231, 211, 187, 107, 214, 177, 127, 254, 225, 223, 163, 91, 182, 113, 226, 217, 175, 67, 134, 17,
34, 68, 136, 13, 26, 52, 104, 208, 189, 103, 206, 129, 31, 62, 124, 248, 237, 199, 147, 59,
118, 236, 197, 151, 51, 102, 204, 133, 23, 46, 92, 184, 109, 218, 169, 79, 158, 33, 66, 132,
21, 42, 84, 168, 77, 154, 41, 82, 164, 85, 170, 73, 146, 57, 114, 228, 213, 183, 115, 230,
209, 191, 99, 198, 145, 63, 126, 252, 229, 215, 179, 123, 246, 241,255, 227, 219, 171, 75, 150,
49, 98, 196, 149, 55, 110, 220, 165, 87, 174, 65, 130, 25, 50, 100, 200, 141, 7, 14, 28,
56, 112, 224, 221, 167, 83, 166, 81, 162, 89, 178, 121, 242, 249, 239, 195, 155, 43, 86, 172,
69, 138, 9, 18, 36, 72, 144, 61, 122, 244, 245, 247, 243, 251, 235, 203, 139, 11, 22, 44,
88, 176, 125, 250, 233, 207, 131, 27, 54, 108, 216, 173, 71, 142,1
};


const u8 IntToGalois[] = {
	0,0,1,25,2,50,26,198,3,223,51,238,27,104,199,75,4,100,224,14,52,
141,239,129,28,193,105,248,200,8,76,113,5,138,101,47,225,36,15,33,53,
147,142,218,240,18,130,69,29,181,194,125,106,39,249,185,201,154,9,120,77,
228,114,166,6,191,139,98,102,221,48,253,226,152,37,179,16,145,34,136,54,
208,148,206,143,150,219,189,241,210,19,92,131,56,70,64,30,66,182,163,195,
72,126,110,107,58,40,84,250,133,186,61,202,94,155,159,10,21,121,43,78,
212,229,172,115,243,167,87,7,112,192,247,140,128,99,13,103,74,222,237,49,
197,254,24,227,165,153,119,38,184,180,124,17,68,146,217,35,32,137,46,55,
63,209,91,149,188,207,205,144,135,151,178,220,252,190,97,242,86,211,171,20,
42,93,158,132,60,57,83,71,109,65,162,31,45,67,216,183,123,164,118,196,
23,73,236,127,12,111,246,108,161,59,82,41,157,85,170,251,96,134,177,187,
204,62,90,203,89,95,176,156,169,160,81,11,245,22,235,122,117,44,215,79,
174,213,233,230,231,173,232,116,214,244,234,168,80,88,175
};

const u8 ByteFlip[] = {
	0,128,64,192,32,160,96,224,16,144,80,208,48,176,112,240,8,136,72,200,40,
168,104,232,24,152,88,216,56,184,120,248,4,132,68,196,36,164,100,228,20,
148,84,212,52,180,116,244,12,140,76,204,44,172,108,236,28,156,92,220,60,
188,124,252,2,130,66,194,34,162,98,226,18,146,82,210,50,178,114,242,10,
138,74,202,42,170,106,234,26,154,90,218,58,186,122,250,6,134,70,198,38,
166,102,230,22,150,86,214,54,182,118,246,14,142,78,206,46,174,110,238,30,
158,94,222,62,190,126,254,1,129,65,193,33,161,97,225,17,145,81,209,49,
177,113,241,9,137,73,201,41,169,105,233,25,153,89,217,57,185,121,249,5,
133,69,197,37,165,101,229,21,149,85,213,53,181,117,245,13,141,77,205,45,
173,109,237,29,157,93,221,61,189,125,253,3,131,67,195,35,163,99,227,19,
147,83,211,51,179,115,243,11,139,75,203,43,171,107,235,27,155,91,219,59,
187,123,251,7,135,71,199,39,167,103,231,23,151,87,215,55,183,119,247,15,
143,79,207,47,175,111,239,31,159,95,223,63,191,127,255
};

const u8 C7[] = {0,87,229,146,149,238,102,21};
const u8 C8[] = {0,175,238,208,249,215,252,196,28};
const u8 C9[] = {0,95,246,137,231,235,149,11,123,36};
const u8 C10[] = {0,251,67,46,61,118,70,64,94,32,45};
const u8 C11[] = {0,220,192,91,194,172,177,209,114,227,10,55};
const u8 C12[] = {0};
const u8 C13[] = {0};
const u8 C14[] = {0};
const u8 C15[] = {0};
const u8 C16[] = {0};
const u8 C17[] = {0};
const u8 C18[] = {0};
const u8 C19[] = {0};
const u8 C20[] = {0};
const* GenPoly[] = {C7,C8,C9,C10,C11,C12,C13,C14,C15,C16,C17,C18,C19,C20};

const u16 DataFlowLen[4][11] = {
	{152,272,440,640,864,1088,1248,1552,1856,2192,2592},
	{128,224,352,512,688,864,992,1232,1456,1728,2032},
	{104,176,272,384,496,608,704,880,1056,1232,1440},
	{72,128,208,288,368,480,528,688,800,976,1120}
};

const u8 DataNumLen[3][4] = {
	{10,9,8,8},//V1-9
	{12,11,16,10},//V10-26
	{14,13,16,12}//V27-40
};

const u8 AlignPatPos[40] = {
	0,0,6,18,6,22,6,26,6,30,6,34,6,22,38,6,24,42,6,26,46,6,28,50,6,30,54,6,32,58
};

const u16 VersionFlow[4][8] = {
	{0xEF88,0xE5E6,0xFB54,0xF13A,0xCC5E,0xC630,0xD882,0xD2EC},
	{0xA824,0xA24A,0xBCF8,0xB696,0x8BF2,0x819C,0x9F2E,0x9540},
	{0x6ABE,0x60D0,0x7E62,0x740C,0x4968,0x4306,0x5DB4,0x57DA},
	{0x2D12,0x277C,0x39CE,0x33A0,0xEC40,0x4AA0,0x1A18,0x1076}
};

u8 FindPattern[8] = {0x07,0x7F,0x41,0x5D,0x5D,0x5D,0x41,0x7F };
u8 AlignPattern[6] = {0x05,0x1F,0x11,0x15,0x11,0x1F};

u16 DataNum = 0;
u8 BlockNum = 0;


u8 QR_Mask = 0;
u8 QR_CorLevel = QR_CORRECTION_LEVEL;
u8 QR_EncodeMode = QR_ENCODE_MODE;

//查表获得版本
QRVersion QR_GetVersion(u8 *raw,u8 corlevel)
{

	u16 loop = 0;
	u8 version = 0;


	while (raw[loop++] != '\0');
	DataNum = loop - 1;
	loop = loop & 0x1 ? (loop >> 1) * 11 + 6 : (loop >> 1) * 11;
	while (DataNum >= MaxNumLen[corlevel][version++]);



	return version;

}


//获取内容符合何种编码
void QR_AnaData(u8* raw, u16 len)
{

	u16 loop;

	for (loop=0;loop<len;loop++)
	{
		if (raw[loop] > '9');
	}


}

// 对纠错等级 掩码形式进行设置
void QR_Config(u8 CorLevel, u8 Mask)
{
	QR_CorLevel = CorLevel;
	QR_Mask = Mask;
}



//
void QR_DataCodeEncode(CorEncodeTypedef** blocks, BitFlow* dataFlow)
{
	u8 loop;
	u8 loop2;
	u8 writeP = dataFlow->FlowLen>>3;
	u8 temp = blocks[0]->InfoLen;
	u8 IsInfo=1;

	if (writeP) {   // 区分数据码与纠错码
		IsInfo = 0;
		temp = dataFlow->MemLen>>3;
	}
	else
	{
		if (blocks[0]->InfoLen < blocks[BlockNum - 1]->InfoLen)
			temp = blocks[BlockNum - 1]->InfoLen;
	}
		

	for ( loop = 0; loop < temp; loop++)
	{
		for (loop2 = 0; loop2 < BlockNum; loop2++)
		{
			if (IsInfo)
			{
				if (loop < blocks[loop2]->InfoLen)
				{
					dataFlow->FlowLen += 8;
					dataFlow->Flow[writeP++] = blocks[loop2]->OperaPoly[loop];
				}
			}
			else if (loop < (blocks[loop2]->BlockLen - blocks[loop2]->InfoLen)) 
			{
				dataFlow->FlowLen += 8;
				dataFlow->Flow[writeP++] = blocks[loop2]->OperaPoly[loop];
			}
		}
	}
	




}
// 
CorEncodeTypedef** QR_GetBlocks(u8 version, u8 corlevel)
{

	u8 loop;
	u8 blockInfo=BlockDivide[version][corlevel];		//取出块种类 位置信息
	CorEncodeTypedef** CorBlocks;

	if ((blockInfo>>4)==1)	BlockNum = BlockDivide[version][(blockInfo&0x07)*3+1];	//一种块
	else BlockNum = BlockDivide[version][(blockInfo & 0x07)*3+1]+ BlockDivide[version][(blockInfo & 0x07)*3+4];	//两种块

	CorBlocks = calloc(BlockNum,4);
	blockInfo = (blockInfo << 4)+ BlockDivide[version][(blockInfo&0x07)*3+1];

	for ( loop = 0; loop < BlockNum; loop++)	//获取每块信息
	{
		CorBlocks[loop] = calloc(1,sizeof(CorEncodeTypedef));
		if (blockInfo & 0x07) {
			CorBlocks[loop]->BlockLen = BlockDivide[version][(blockInfo >> 4) * 3 + 2];
			CorBlocks[loop]->InfoLen = BlockDivide[version][(blockInfo-- >> 4) * 3 + 3];
		}
		else {
			CorBlocks[loop]->BlockLen = BlockDivide[version][(blockInfo >> 4) * 3 + 5];
			CorBlocks[loop]->InfoLen = BlockDivide[version][(blockInfo >> 4) * 3 + 6];
		}
		CorBlocks[loop]->GenPoly = QR_GenPloy(CorBlocks[loop]->BlockLen - CorBlocks[loop]->InfoLen);
	}


	return CorBlocks;


}

void QR_CorCodeEncode(CorEncodeTypedef* cb)
{
	u8 loop;
	u8 loop2;
	u8 operaCodeLen = cb->InfoLen - 1;
	u8 corNum = cb->BlockLen - cb->InfoLen;
	u8 operaA;
	u16 temp;
	u8 sp=0;
	//if (corNum > cb->InfoLen)operaCodeLen = corNum;
	//else operaCodeLen = cb->InfoLen - 1;

	for ( loop = 0; loop < cb->InfoLen; loop++)	//数据码有几项就进行几次运算
	{

		operaA = cb->OperaPoly[0];
		for ( loop2 = 0; loop2 < corNum; loop2++)	//遍历infocode
		{
			
			temp = cb->GenPoly[loop2+1] + IntToGalois[operaA];
			if (temp > 254)temp -= 255;
			cb->OperaPoly[loop2] = cb->OperaPoly[loop2 + 1] ^ GaloisToInt[temp];
			//printf("%d,", cb->OperaPoly[loop2]);
		}
		

		if (operaCodeLen > corNum) {

			for (; loop2 < operaCodeLen; loop2++)
			{
				cb->OperaPoly[loop2] = cb->OperaPoly[loop2 + 1];
				//printf("%d,", cb->OperaPoly[loop2]);
			}
			operaCodeLen--;
			//printf("kkk");
		}
		else if (operaCodeLen == corNum) {
			if (sp)cb->OperaPoly[--loop2] = GaloisToInt[temp];
			else sp = 1;
		}
		else
		{
			for (; loop2 > operaCodeLen;)
			{
				cb->OperaPoly[--loop2] = GaloisToInt[temp];
				//printf("%d,", cb->OperaPoly[loop2]);
			}
			operaCodeLen=corNum;
			//printf("mmm");
		}
		
		//printf("\n");

	}

}



u8* QR_CharEncode(u8* raw,QRCode* qrcode)
{
	u16 loopP = 0;
	u16 loopMax = 0;
	u8 temp1;
	u16 temp2;
	BitFlow dataFlow;
	u8 tempBuf[2];
	





	dataFlow.MemLen = DataFlowLen[qrcode->CorLevel - 1][qrcode->Version - 1];
	dataFlow.Flow = calloc((dataFlow.MemLen >> 3) + 1, 1);
	dataFlow.FlowLen = 0;
	
	tempBuf[0] = 0x02;
	BitFlowAppend(&dataFlow, tempBuf,4);	//模式标识符
	tempBuf[0] = (u8)DataNum;
	tempBuf[1] = 0;
	BitFlowAppend(&dataFlow,tempBuf, DataNumLen[qrcode->Version>9?qrcode->Version>26?2:1:0][1]);	//字符数标识符

	
	for (loopP = 0; loopP < DataNum; loopP++)
	{
		if (raw[loopP]<':')
		{
			if (raw[loopP] < '0') 
			{
				if (raw[loopP] == '/')temp1 = 43;
				else if (raw[loopP] == '.')temp1 = 42;
				else if (raw[loopP] == '-')temp1 = 41;
				else if (raw[loopP] == '+')temp1 = 40;
				else if (raw[loopP] == '*')temp1 = 39;
				else if (raw[loopP] == '%')temp1 = 38;
				else if (raw[loopP] == '$')temp1 = 37;
				else if (raw[loopP] == ' ')temp1 = 36;
			}else temp1 = raw[loopP] - 48;
		}else if(raw[loopP] == ':')temp1 = 44;
		else temp1 = raw[loopP] - 55;

		if (loopP & 0x0001) {
			temp2 += temp1;
			tempBuf[0] = (u8)temp2;
			tempBuf[1] = temp2>>8;
			BitFlowAppend(&dataFlow,tempBuf, 11);
		}
		else temp2 = temp1*45;
	}

	if (loopP&0x0001)	//单数添加尾部
	{
		tempBuf[0] = temp1;
		BitFlowAppend(&dataFlow, tempBuf, 6);
	}

	tempBuf[0] = 0;
	BitFlowAppend(&dataFlow, tempBuf, 8-(dataFlow.FlowLen&0x0007));
	
	BitFlowFill(&dataFlow);



	

	return dataFlow.Flow;


}

void QR_SetData(u8** qrFlow,u8** mask, BitFlow* dataFlow,u8 width)
{
	u16 loop;
	u8 temp=0;
	u8 offset=7;
	u8 writeP=0;
	u8 setX=width-1;
	u8 setY=width-1;
	u8 direction = 0;
	

	

	for ( loop = 0; loop < dataFlow->MemLen;)
	{
		
		if (mask[setY >> 3][setX] & (0x01 << (setY & 0x07)));//mask有则跳过
		/*else if (dataFlow->Flow[writeP] & (0x01 << offset--)) {
			QR_SetDot(qrFlow,setX,setY);
			QR_SetDot(mask, setX, setY);
			loop++;
		}*/
		else {
			QR_SetMaskDot(qrFlow, setX, setY, dataFlow->Flow[writeP] & (0x01 << offset--));
			QR_SetDot(mask, setX, setY);
			loop++;
		}

		if (offset==255)
		{
			offset = 7;
			writeP++;
		}

		
		if (direction==0)	//正常移动
		{
			setX--;
			direction = 1;
		}else if (direction == 1)
		{
			setX++;
			setY--;
			direction = 0;
		}else if (direction == 2)
		{
			setX--;
			direction = 3;
		}else
		{
			setX++;
			setY++;
			direction = 2;
		}

		if (setY == 255)		//换列
		{
			if (setX==8)
			{
				setX--;
			}
			setX -= 2;
			setY = 0;
			direction = 2;
			continue;
		}
		if (setY == width )
		{
			setX -= 2;
			setY--;
			direction = 0;
			continue;
		}
		/*if (writeP==20)
		{
			printf("\n---%d---\n",(dataFlow->Flow[writeP]));
			return;
		}*/

	}



}

void QR_SetVersion(QRCode qr)
{
	
	u8 loop;
	u8 loop2 = qr.Version*4+16;
	u16 versionFlow = VersionFlow[qr.CorLevel -1][qr.Mask];

	for ( loop = 0; loop < 6; loop++)	//0-5位
	{
		if (versionFlow & 0x8000)
		{
			QR_SetDot(qr.bitFlow, loop, 8);
			QR_SetDot(qr.bitFlow, 8, loop2--);
		}
		else loop2--;
		
		versionFlow = versionFlow << 1;
	}
	if (versionFlow&0x8000)
	{
		QR_SetDot(qr.bitFlow, ++loop, 8);	//6
		QR_SetDot(qr.bitFlow, 8, loop2--);
	}
	else {
		loop++;
		loop2--;
	}
	versionFlow = versionFlow << 1;
	if (versionFlow & 0x8000)
	{
		QR_SetDot(qr.bitFlow, ++loop, 8);  //7
		QR_SetDot(qr.bitFlow, loop2++,8 );
	}
	else {
		loop++;
		loop2++;
	}
	versionFlow = versionFlow << 1;
	if (versionFlow & 0x8000)
	{
		QR_SetDot(qr.bitFlow, 8, --loop);  //8
		QR_SetDot(qr.bitFlow, loop2++, 8);
	}
	else {
		loop++;
		loop2++;
	}
	versionFlow = versionFlow << 1;

	for (loop = 5; loop != 255; loop--)
	{
		if (versionFlow&0x8000)
		{
			QR_SetDot(qr.bitFlow, 8, loop);
			QR_SetDot(qr.bitFlow, loop2++, 8);
		}
		else loop2++;
		
		versionFlow = versionFlow << 1;
	}
	


}



u8* QR_GenPloy(u8 CorNum)
{

	u8 loop,loopMax;
	u8 temp;
	u8* gp;
	u8 gpT;
	u16 sumTemp;
	
	if (CorNum < 7)return NULL;

	gp = calloc(CorNum + 1, 1);

	for ( loop = 0; loop < 8; loop++)	//纠错码最少为7 生成多项式最少为8项
	{
		gp[loop] = C7[loop];
	}

	loopMax = CorNum + 1;
	for ( loop = 8; loop < loopMax; loop++)
	{
		gpT = 0;
		for ( temp = 1; temp < loop; temp++)
		{		
			sumTemp = gpT + loop - 1;
			if (sumTemp > 254)sumTemp -= 255;
			gpT = gp[temp];
			gp[temp] = GaloisToInt[sumTemp] ^ GaloisToInt[gpT];
			gp[temp] = IntToGalois[gp[temp]];
		}
		gp[temp] = gpT + loop - 1;

	}

	/*for ( loop = 0; loop <= CorNum; loop++)
	{
		printf("%d,",gp[loop]);
	}*/

	return gp;

}

void QR_SetFormat(u8** flow, u8** mask, u8 version)
{

	u8 wide = (version << 2) + 17;
	u8 loopx;
	u8 loopy;
	u8 temp1;
	u8* tempP;
	u8 temp3;

	//添加查找块与分隔符
	QR_PastePattern(FindPattern, 0, 0, flow, mask);
	QR_PastePattern(FindPattern, wide-7, 0, flow, mask);
	QR_PastePattern(FindPattern, 0,wide-7,  flow, mask);

	

	//添加对齐图案
	temp1 = (version / 7) + 2;
	if (temp1 == 2)tempP = &(AlignPatPos[temp1*version-2]);
	else if (temp1 == 3)tempP = &(AlignPatPos[temp1 * version - 9]);
	else if (temp1 == 4)tempP = &(AlignPatPos[temp1 * version - 7]);//以下未更正
	else if (temp1 == 5)tempP = &(AlignPatPos[temp1 * version - 7]);
	else if (temp1 == 6)tempP = &(AlignPatPos[temp1 * version - 7]);
	else tempP = &(AlignPatPos[temp1 * version - 7]);
	
	

	for ( loopx = 0; loopx < temp1; loopx++)
	{
		for ( loopy = 0; loopy < temp1; loopy++)
		{
			if (mask[tempP[loopy] >> 3][tempP[loopx]] & 0x01 << (tempP[loopy] & 0x07));
			else QR_PastePattern(AlignPattern, tempP[loopx] - 2, tempP[loopy] - 2, flow, mask);		
		}
	}

	


	//添加时序线
	temp1 = wide - 7;
	for ( loopx = 8; loopx < temp1; loopx+=2)
	{
		QR_SetDot(flow,6,loopx);
		QR_SetDot(flow,loopx, 6);
	}
	for (loopx = 7; loopx < temp1; loopx ++)
	{
		QR_SetDot(mask, 6, loopx);
		QR_SetDot(mask, loopx, 6);
	}





	//添加其他固定格式
	QR_SetDot(flow, 8, wide-8);//  添加角点
	temp1 = 9;
	for ( loopx= 0; loopx < temp1; loopx++)
	{
		QR_SetDot(mask, loopx, 7);
		QR_SetDot(mask, loopx,8);
		QR_SetDot(mask, loopx, wide-8);
		QR_SetDot(mask, wide-8 , loopx);
	}
	temp1 = 7;
	for (loopx = 0; loopx < temp1; loopx++)
	{
		QR_SetDot(mask, 7, loopx);
		QR_SetDot(mask,8, loopx);
		QR_SetDot(mask, 7,wide-7+ loopx);
		QR_SetDot(mask, 8, wide-7+loopx);
		QR_SetDot(mask, wide-7+loopx, 7);
		QR_SetDot(mask, wide-7+loopx, 8);
	}

	if (version>6)	//版本大于8添加预留版本信息区
	{
		temp1 = 6;
		for (loopx = 0; loopx < temp1; loopx++)
		{
			QR_SetDot(mask, loopx, wide - 9);
			QR_SetDot(mask, loopx, wide - 10);
			QR_SetDot(mask, loopx, wide - 11);
			QR_SetDot(mask, wide - 9, loopx);
			QR_SetDot(mask, wide - 10, loopx);
			QR_SetDot(mask, wide - 11, loopx);
		}
	}



}

__inline void QR_SetDot(u8** flow, u8 x, u8 y)
{
	flow[y >> 3][x] |= 0x01 << (y & 0x07);
}

void QR_SetMaskDot(u8** flow, u8 x, u8 y, u16 data)
{
	if (QR_Mask==0)
	{
		if ((x + y) & 0x01) {
			if (data)flow[y >> 3][x] |= 0x01 << (y & 0x07);
		}else if (data == 0) flow[y >> 3][x] |= 0x01 << (y & 0x07);
	}


}



void QR_PastePattern(u8* pat, u8 x, u8 y, u8** flow,u8** mask)
{

	u8 loop;
	u8 loop2=pat[0]+x;
	u8 offsetY = y & 0x07;
	u8 writePy = y >> 3;
	u8 maskPat;

	if (pat[0] == 7)maskPat = 0x7F;
	else maskPat = 0x1F;

	for (loop = x; loop < loop2; loop++)
	{
	
		flow[writePy][loop] |= pat[loop-x+1] << offsetY;
		mask[writePy][loop] |= maskPat << offsetY;
		
	}
	if (offsetY)
	{
		writePy++;
		offsetY = 8 - offsetY;
		for (loop = x; loop < loop2; loop++)
		{

			flow[writePy][loop] |= pat[loop-x+1] >> offsetY;
			mask[writePy][loop] |= maskPat >> offsetY;

		}
	}


}



u8 BitFlowAppend(BitFlow* bf,u8* appendFlow, u16 len)
{
	// 正序追加版本
	//u16 loop,loopLen = len>>3;
	//u8 offset = (u8)bf->FlowLen & 0x07;	//获得多余比特数
	//u8 writeP = bf->FlowLen >> 3;
	//if ((bf->FlowLen + len) > bf->MemLen)return QRError_MEMSIZE;	//长度越界判断
	//if (offset)	//存在偏置分步复制
	//{
	//	for (loop = 0; loop < loopLen; loop++)
	//	{
	//		bf->Flow[writeP++] |= appendFlow[loop] << offset;
	//		bf->Flow[writeP] |= appendFlow[loop] >> offset;
	//	}
	//	bf->Flow[writeP] |= appendFlow[loop] << offset;  //补齐尾字节
	//	if (offset + (len & 0x0007) > 8)bf->Flow[++writeP] |= appendFlow[loop] >> (8-offset);
	//	
	//}
	//else    //对齐直接复制
	//{
	//	for ( loop = 0; loop < loopLen; loop++)
	//	{
	//		bf->Flow[writeP++] = appendFlow[loop];
	//	}
	//	bf->Flow[writeP] |= appendFlow[loop];  //补齐尾字节
	//}
	//
	//bf->FlowLen += len;
	//offset = (u8)bf->FlowLen & 0x07;
	//if (offset != 0)bf->Flow[(bf->FlowLen >> 3)] &= 0xFF >> offset;  //清理尾部
	//上为正序追加版本
	// 实际需要逆序追加
	u16 loop;
	u8* writeP;
	writeP = bf->Flow + (bf->FlowLen >> 3);
//	u8 offsetW = bf->FlowLen & 0x07;	//正写
	u8 offsetW = 7 - (bf->FlowLen & 0x07);   //倒写
	u8* readP;
	u8 offsetR = (len - 1) & 0x07;

	readP = appendFlow + ((len - 1) >> 3);
	
	
	if ((bf->FlowLen + len) > bf->MemLen)return QRError_MEMSIZE;	//长度越界判断
	for (loop=0;loop<len;loop++) {
	//	*writeP |= (((*readP) >> offsetR--) & 0x01) << offsetW++;	//正写
		*writeP |= (((*readP) >> offsetR--) & 0x01) << offsetW--;	//倒写
		if (offsetR==255)
		{
			readP--;
			offsetR = 7;
		}
		if (offsetW==255)
		{
			writeP++;
			offsetW = 7;
		}
	}
	
	bf->FlowLen += len;

	return 0;
}

u8 BitFlowStrAppend(BitFlow* bf, u8* str)
{
	u16 readP = 0;

	while (str[readP] != '\0')
	{
		if (str[readP++]=='1')  bf->Flow[((bf->FlowLen++) >> 3)] |= (0x01 << ((bf->FlowLen) & 0x07));
		else {
		readP++;
		bf->FlowLen++;
		}
	}
	return 0;
}

void BitFlowFill(BitFlow* bf)
{
	u8 loop = bf->FlowLen >> 3;
	u8 loopEnd = bf->MemLen >> 3;
	u8 temp = 0;
	for (;loop<loopEnd;loop++)
	{
		if (temp) {
			bf->Flow[loop] = 0x11;
			temp = 0;
		}
		else {
			bf->Flow[loop] = 0xEC;
			temp = 1;
		}
	}
	bf->FlowLen = bf->MemLen;
}






void BitFlowPrint(u8* flow,u16 len)
{
	u16 loop;
	u8* readP;
	readP = flow;
	u8 offset = 0;
	for (loop = 0; loop < len; loop++)
	{
		if ((*readP) & (0x01 << offset++))putchar('1');
	//	if (bf.Flow[loop >> 3]&(0x01<<(u8)loop))putchar('1');      //低位到高位
	//	if (bf.Flow[loop >> 3] & (0x80 >> loop%8))putchar('1');   //高位到低位
		else putchar('0');
		if (offset == 8) {
			offset = 0;
			readP++;
		}
		if ((loop%8)==7)putchar(' ');	
		if ((loop % 64) == 63)printf("\n");
	}

}

void QR_Encode(u8* raw)
{

	u8 loop=0;
	u8 loop2;
	u8 temp;
	u8* tempP;
	u8** backup;
	u8* InfoCode;
	
	CorEncodeTypedef** Blocks;
	BitFlow dataFlow;
	
	
#ifdef LOG
	printf("RAW String:");
	while (raw[loop] != '\0')printf("%c", raw[loop++]);
	printf("\n\n---Getting Version/CorrectionLevel/Mask/Mode---\n");
#endif // LOG


	qrcode.EncodeMode = QR_EncodeMode;
	qrcode.CorLevel = QR_CorLevel;	
	qrcode.Mask = QR_Mask;
	qrcode.Version = QR_GetVersion(raw, qrcode.CorLevel);

#ifdef LOG
	printf("QRVersion: %d    CorLevel: %d    Mask: %d    Mode: %d\n",qrcode.EncodeMode,qrcode.CorLevel,qrcode.Mask,qrcode.EncodeMode);
#endif // LOG


#ifdef LOG
	printf("\n---Getting InfoCode---\n");
#endif // LOG


	//	获取信息编码
	InfoCode = QR_CharEncode(raw, &qrcode);
	

#ifdef LOG
	for ( loop = 0; loop < ((DataFlowLen[qrcode.CorLevel - 1][qrcode.Version - 1])>>3); loop++)
	{
		printf("%3d,",InfoCode[loop]);
		if (loop%16==15)printf("\n");
	}
	printf("\n");
#endif // LOG



#ifdef LOG
	printf("\n---Getting Blocks---\n");
#endif // LOG

	
	//	获取分块方式	顺便加上了生成多项式
	Blocks = QR_GetBlocks(qrcode.Version-1,qrcode.CorLevel-1);


#ifdef LOG
	printf("Block\tBlockLen  InfoCodeNum   CorCodeNum\n");
	for (loop = 0; loop < BlockNum; loop++)printf("%d\t%d\t  %d\t\t%d\n", loop, Blocks[loop]->BlockLen, Blocks[loop]->InfoLen, Blocks[loop]->BlockLen - Blocks[loop]->InfoLen);
	printf("\n");
#endif // LOG

	

#ifdef LOG
	printf("---Fill Blocks with InfoCode---\n");
#endif // LOG


	//	将infoCode填入分块 
	tempP = InfoCode;
	for ( loop = 0; loop < BlockNum; loop++)
	{
		Blocks[loop]->OperaPoly = tempP;
		tempP += Blocks[loop]->InfoLen;
	}

	dataFlow.MemLen = MaxNumLen[0][qrcode.Version - 1];	//交织信息码
	dataFlow.Flow = calloc((dataFlow.MemLen >> 3) + 1, 1);
	dataFlow.FlowLen = 0;
	QR_DataCodeEncode(Blocks,&dataFlow);


#ifdef LOG
	for (loop = 0; loop < dataFlow.MemLen >> 3; loop++)
	{
		printf("%4d,", dataFlow.Flow[loop]);
		if (loop%16==15)printf("\n");
	}
	printf("\n");

//	for (loop = 0; loop < BlockNum; loop++)printf("Block %d:  InfoCodeNum %d  CorrectionCodeNum %d\n", (int)loop, Blocks[loop]->InfoLen, Blocks[loop]->BlockLen - Blocks[loop]->InfoLen);
#endif // LOG

	
#ifdef LOG
	printf("\n---Generate CorrectionCode---\n");
#endif // LOG

	// 生成纠错码
	for ( loop = 0; loop < BlockNum; loop++)
	{
		QR_CorCodeEncode(Blocks[loop]);

#ifdef LOG
		printf("Block %d :",loop);
		for (loop2 = 0; loop2 < Blocks[loop]->BlockLen - Blocks[loop]->InfoLen; loop2++) printf("%d ", Blocks[loop]->OperaPoly[loop2]);
		printf("\n");
#endif // LOG	

	}
	
#ifdef LOG
	printf("\n---Fill Blocks with CorCode---\n");
#endif // LOG


	QR_DataCodeEncode(Blocks, &dataFlow);//  交织纠错码

#ifdef LOG
	for (loop = 0; loop < dataFlow.MemLen >> 3; loop++)
	{
		printf("%4d,", dataFlow.Flow[loop]);
		if (loop % 16 == 15)printf("\n");
	}
#endif // LOG

	free(InfoCode);
	for ( loop = 0; loop < BlockNum; loop++)
	{
		free(Blocks[loop]);
	}
	free(Blocks);

	
	//申请内存   列横式
	loop = (qrcode.Version << 2) + 17;
	loop2 = (qrcode.Version >> 1) + 3;
	
	qrcode.bitFlow = calloc(loop2,sizeof(u8*));
	backup = calloc(loop2, sizeof(u8*));
	for (temp = 0; temp < loop2; temp++)
	{
		qrcode.bitFlow[temp] = calloc(loop,1);
		backup[temp] = calloc(loop, 1);
	}
	
	// 填入基本格式
	QR_SetFormat(qrcode.bitFlow, backup, qrcode.Version);	
	QR_CodePrinter(qrcode);
	//qrcode.bitFlow = mask;
	//QR_CodePrinter(qrcode);
	
	QR_SetData(qrcode.bitFlow,backup,&dataFlow,qrcode.Version*4+17);   //填入数据码

	free(dataFlow.Flow);

	QR_SetVersion(qrcode);

	QR_CodePrinter(qrcode);

	/*qrcode.bitFlow = backup;
	QR_CodePrinter(qrcode);*/
}




void QR_CodePrinter(QRCode qr)
{
	u8 loop,loop2;
	u8 width = qr.Version * 4 + 17;
	

	printf("\n\nVersion: %d\tWidth: %d\tCorLevel: %d\tMask: %d\n",qr.Version,width,qr.CorLevel,qr.Mask);
	printf("    ");
	for (loop = 0; loop < width; loop++)
	{
		printf("%2d",loop);
	}
	printf("\n  ");
	for ( loop = 0; loop < width+2; loop++)
	{
		printf("□");
	}
	printf("\n");
	for (loop = 0; loop < width;loop++)
	{
		
		printf("%2d□",loop);
		for ( loop2 = 0; loop2 < width; loop2++)
		{
			if (qr.bitFlow[loop >> 3][loop2] & (0x01 << (loop & 0x07)))printf("■");
			else printf("  ");
		}
		printf("□%2d",loop);
		printf("\n");
	}
	printf("  ");
	for (loop = 0; loop < width + 2; loop++)
	{
		printf("□");
	}
	printf("\n\n");
}






void Test(void)
{
	int loop;
	int GA = 1;
	int temp;
	for ( loop = 0; loop < 256; loop++)
	{
		/*if (GA>255)
		{
			GA ^= 285;
		}
		printf("%d,",GA);
		GA *= 2;
		if (loop%20==0)
		{
			printf("\n");
		}*/

		/*for (temp= 0; temp < 256; temp++)
		{
			if (GaloisToInt[temp] == loop )
			{
				printf("%d,",temp);
				break;
			}
		}
		if (loop%20==0)
		{
			printf("\n");
		}*/
		temp = 0;
		temp += (loop & 0x01) << 7;
		temp += (loop & 0x02) << 5;
		temp += (loop & 0x04) << 3;
		temp += (loop & 0x08) << 1;
		temp += (loop & 0x10) >> 1;
		temp += (loop & 0x20) >> 3;
		temp += (loop & 0x40) >> 5;
		temp += (loop & 0x80) >> 7;

		printf("%d,",temp);

		if (loop % 20 == 0)
		{
			printf("\n");
		}

	//	if (IntToGalois[GaloisToInt[loop]] == loop)printf("%d OK\n", loop);
	//	else printf("%d Fail\n",loop);
	}

	/*CorEncodeTypedef** tt = DataCode_GetCorBlocks(0,1);

	for ( loop = 0; loop < 1; loop++)
	{
		printf("BLOCK %d\tBlockLen %d\tDataLen %d",loop,tt[loop]->BlockLen,tt[loop]->InfoLen );
	}*/


}

