#include <iconv.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "tty.h"
#include"gprs.h"

void gprs_init()
{ 
	int i;
	for(i=0; i<7; i++) {
		tty_writecmd(cmd[i], strlen(cmd[i])); 
	}

}
void gprs_hold()
{
	tty_writecmd("at", strlen("at"));
	tty_writecmd("ath", strlen("ath"));
}

void gprs_ans()
{ 
	tty_writecmd("at", strlen("at"));
	tty_writecmd("ata", strlen("ata"));
}

void gprs_call(char *number, int num)			
{ 

	tty_write("atd", strlen("atd"));
	tty_write(number, num);
	tty_write(";\r", strlen(";\r"));
	usleep(200000);
}

//ʹ���ı�ģʽ���Ͷ���
void gprs_msg(char *number, int num)		
{ 
	char ctl[]={26,0};
	char text[]="Welcome to use up-tech embedded platform!";
	tty_writecmd("at", strlen("at"));
	tty_writecmd("at", strlen("at"));
	tty_writecmd("at+cmgf=1", strlen("at+cmgf=1"));	
	tty_write("at+cmgs=", strlen("at+cmgs="));

    tty_write("\"", strlen("\""));
    tty_write(number, strlen(number));
    tty_write("\"", strlen("\""));
    tty_write(";\r", strlen(";\r"));
	tty_write(text, strlen(text));
	tty_write(ctl, 1);
	usleep(300000);
}

void gprs_baud(char *baud,int num)
{
	tty_write("at+ipr=", strlen("at+ipr="));
	tty_writecmd(baud, strlen(baud) );
	usleep(200000);
}
//���ַ���ת������Ӧ�ı���λ
int gsmString2Bytes(const char *psrc,unsigned char* pdst,int nsrclength)
{
	int i;
	for(i=0;i<nsrclength;i+=2)
	{
		if(*psrc>='0'&&*psrc<='9')
			*pdst=(*psrc - '0')<<4;
		else
			*pdst=(*psrc - 'A'+10)<<4;

		psrc++;

		if((*psrc)>='0'&&(*psrc)<='9')
		{
			*pdst |=*psrc - '0';
		}
		else
		{
			*pdst |=*psrc - 'A'+10;	
		}
		psrc++;
		pdst++;
	}
	return nsrclength/2;
}

int gsmBytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
   	const char tab[]="0123456789ABCDEF";    // 0x0-0xf���ַ����ұ�
	int i;
    for(i=0; i<nSrcLength; i++)
   {
      // �����4λ
	      *pDst++ = tab[*pSrc >> 4];
								    
     // �����4λ
	      *pDst++ = tab[*pSrc & 0x0f];
													    
	       pSrc++;
    }
																    
	 // ����ַ����Ӹ�������
		   *pDst = '\0';
														    
    // ����Ŀ���ַ�������
	return nSrcLength * 2;
	
}
	
int gsmInvertNumbers(const char* pSrc, char* pDst, int nSrcLength)
{
    int nDstLength;   // Ŀ���ַ�������
	char ch;          // ���ڱ���һ���ַ�
	int i;	    
      // ���ƴ�����
	   nDstLength = nSrcLength;
					    
	  // �����ߵ�
	  for(i=0; i<nSrcLength;i+=2)
	   {
		   ch = *pSrc++;  // �����ȳ��ֵ��ַ�
	      *pDst++ = *pSrc++;   // ���ƺ���ֵ��ַ�
	      *pDst++ = ch;        // �����ȳ��ֵ��ַ�
	    }
																    
       // Դ��������������
	  if(nSrcLength & 1)
	    {
		 *(pDst-2) = 'F';     // ��'F'
	     nDstLength++;        // Ŀ�괮���ȼ�1
	   }
																									    
	   // ����ַ����Ӹ�������
	   *pDst = '\0';
																												    
	   // ����Ŀ���ַ�������
	 return nDstLength;
}
																															
int gsmSerializeNumbers(const char* pSrc, char* pDst, int nSrcLength)
{
	int nDstLength;   // Ŀ���ַ�������
	char ch;          // ���ڱ���һ���ַ�
	int i;    
 // ���ƴ�����
    nDstLength = nSrcLength;
					    
  // �����ߵ�
    for(i=0; i<nSrcLength;i+=2)
	 {
	   ch = *pSrc++;        // �����ȳ��ֵ��ַ�
	  *pDst++ = *pSrc++;   // ���ƺ���ֵ��ַ�
	  *pDst++ = ch;        // �����ȳ��ֵ��ַ�
	 }
																    
	// �����ַ���'F'��
	if(*(pDst-1) == 'F')
	 {
	    pDst--;
        nDstLength--;        // Ŀ���ַ������ȼ�1
	   }
																									     
	// ����ַ����Ӹ�������
	   *pDst = '\0';
																												    
   // ����Ŀ���ַ�������
    return nDstLength;
}

		
// UCS2����
// pSrc: Դ�ַ���ָ��
// pDst: Ŀ����봮ָ��
// nSrcLength: Դ�ַ�������
// ����: Ŀ����봮����
int gsmEncodeUcs2(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
    int nDstLength;        // UNICODE���ַ���Ŀ
    char  unicode_buf[200];      // UNICODE��������
    int i;
	char *p=pDst;
	printf("gb2312: %s\n",pSrc);
	
    // �ַ���-->UNICODE��
	g2u(pSrc,nSrcLength,unicode_buf,200);
	printf("nSrcLength: %d\n",nSrcLength);
	
	printf("unicode: %s\n",unicode_buf);	
	nDstLength=strlen(unicode_buf);
	    
	printf("dstlength: %d\n",nDstLength);
    // �ߵ��ֽڶԵ������
    for(i=0; i<nDstLength-1; i+=2)
    {
	//	printf("$$$$$: %c\n",unicode_buf[i]);
		*pDst=unicode_buf[i+1];
		 pDst++;
		 *pDst=unicode_buf[i];
		 pDst++;
         unicode_buf[i] & 0xff;	 
    }
 //  printf("UD: %s\n",p); 
    // ����Ŀ����봮���� i*/
    return nDstLength;
}

// UCS2����
// pSrc: Դ���봮ָ��
// pDst: Ŀ���ַ���ָ��
// nSrcLength: Դ���봮����
// ����: Ŀ���ַ�������
int gsmDecodeUcs2(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
    int nDstLength;        // UNICODE���ַ���Ŀ
    char unicode_buf[200];      // UNICODE��������
    int i;
	
	printf("srclength:%d\n",nSrcLength);
	
    // �ߵ��ֽڶԵ���ƴ��UNICODE
    for(i=0; i<nSrcLength/2+1;i+=2)
    {
        // �ȸ�λ�ֽ�
        unicode_buf[i+1] =*pSrc;
		pSrc++;
        // ���λ�ֽ�
        unicode_buf[i] =*pSrc;
		pSrc++;
    }
	
    //printf("Decode: %s\n",unicode_buf);
    // UNICODE��-->�ַ���
    u2g(unicode_buf,nSrcLength,pDst,nSrcLength);

	printf("decode gb2312: %s\n",pDst);
    // ����ַ����Ӹ�������    
    pDst[nDstLength] ='\0';    

    // ����Ŀ���ַ�������
    return nDstLength;
}

/*����ת��:��һ�ֱ���תΪ��һ�ֱ���*/
  int code_convert(char *from_charset,char *to_charset,const char *inbuf,int inlen,char *outbuf,int outlen)
 {
     iconv_t cd;
     int rc;
     char **pin = (char **)&inbuf;
     char **pout = (char **)&outbuf;
     cd = iconv_open(to_charset,from_charset);
     if (cd==0) return -1;
     memset(outbuf,0,outlen);
     if (iconv(cd,pin,&inlen,pout,&outlen)==-1) return -1;
     iconv_close(cd);
     return 0;
 }
  /*UNICODE��תΪGB2312��*/
 int u2g(const char *inbuf,int inlen,char *outbuf,int outlen)
 {
    return code_convert("UTF-16Le","gb2312",inbuf,inlen,outbuf,outlen);
 }
    /*GB2312��תΪUNICODE��*/
 int g2u(const char *inbuf,size_t inlen,char *outbuf,size_t outlen)
 {
   return code_convert("gb2312","UTF-16Le",inbuf,inlen,outbuf,outlen);
 }
 
		
int gsmEncodePdu(const SM_PARAM* pSrc, char* pDst)
{
    int nLength;             // �ڲ��õĴ�����
	int nDstLength;          // Ŀ��PDU������
    unsigned char buf[256];  // �ڲ��õĻ�����
	unsigned char UD[100];  
	int len;
	char my_buf[50];
	// SMSC��ַ��Ϣ��
    nLength = strlen(pSrc->SCA);    // SMSC��ַ�ַ����ĳ���   
//	nLength=strlen("���յ�");
	//printf("nLength: %d\n",nLength);
//	len=gsmEncodeUcs2("���յ�",buf,nLength);
//	gsmBytes2String(buf,UD,len);
//	printf("kongtiao ud: %s\n",UD);
    buf[0] = (char)((nLength & 1) == 0 ? nLength : nLength + 1) / 2 + 1;    // SMSC��ַ��Ϣ����
    buf[1] = 0x91;        // �̶�: �ù��ʸ�ʽ����
    nDstLength = gsmBytes2String(buf, pDst, 2);        // ת��2���ֽڵ�Ŀ��PDU��
	nDstLength += gsmInvertNumbers(pSrc->SCA, &pDst[nDstLength], nLength);    // ת��SMSC��Ŀ��PDU��
										    
   // TPDU�λ���������Ŀ���ַ��
    nLength = strlen(pSrc->TPA); 
	// TP-DA��ַ�ַ����ĳ���
    buf[0] = 0x11;            // �Ƿ��Ͷ���(TP-MTI=01)��TP-VP����Ը�ʽ(TP-VPF=10)
    buf[1] = 0;               // TP-MR=0
	buf[2] = (char)nLength;   // Ŀ���ַ���ָ���(TP-DA��ַ�ַ�����ʵ����)
    buf[3] = 0x91;            // �̶�: �ù��ʸ�ʽ����
	nDstLength += gsmBytes2String(buf, &pDst[nDstLength], 4);  // ת��4���ֽڵ�Ŀ��PDU��
	nDstLength += gsmInvertNumbers(pSrc->TPA, &pDst[nDstLength], nLength); // ת��TP-DA��Ŀ��PDU��
																			    
	// TPDU��Э���ʶ�����뷽ʽ���û���Ϣ��
	   nLength = strlen(pSrc->TP_UD);    // �û���Ϣ�ַ����ĳ���
	   buf[0] = pSrc->TP_PID;        // Э���ʶ(TP-PID)
	   buf[1] = pSrc->TP_DCS;        // �û���Ϣ���뷽ʽ(TP-DCS)
	   buf[2] = 0;            // ��Ч��(TP-VP)Ϊ5����
	 if(pSrc->TP_DCS == GSM_UCS2)
	  {
	      // UCS2���뷽ʽ
		  buf[3] = gsmEncodeUcs2(pSrc->TP_UD, &buf[4], nLength);
		 //gsmEncodeUcs2(pSrc->TP_UD,&UD[0],nLength);
          //gsmBytes2String(&buf[4],UD,buf[3]);

		  //printf("UD:%s\n",UD);
		  // ת��TP-DA��Ŀ��PDU��
	      nLength = buf[3] + 4;        // nLength���ڸö����ݳ���
	   }

    nDstLength += gsmBytes2String(buf, &pDst[nDstLength], nLength);        // ת���ö����ݵ�Ŀ��PDU��
	
	// ����Ŀ���ַ�������
	return nDstLength;
	
	}
	
	
// PDU���룬���ڽ��ա��Ķ�����Ϣ
// pSrc: ԴPDU��ָ��
// pDst: Ŀ��PDU����ָ��
// ����: �û���Ϣ������
int gsmDecodePdu(const char* pSrc, SM_PARAM* pDst)
{
	char *ptr;
	int i;
	char *convert_num=(char *)malloc(sizeof(char)*20);
	char *s_number=(char *)malloc(sizeof(char)*20);
	char buf[200];
	char b_buf[100];
	int nDstLength=0;
	for(i=0;i<14;i++)convert_num[i]='\0';
	strcpy(buf,pSrc);
	
	ptr=buf;
	ptr+=24;
	printf("token: %s\n",ptr);
	if(ptr!=NULL)
	{
		
		for(i=0;i<14;i++)
		{
			convert_num[i]=*ptr;
			ptr++;
		}
		printf("covert_num: %s\n",convert_num);
		
		gsmSerializeNumbers(convert_num,s_number,14);
		printf("s_number: %s\n",s_number);	
		
		strcpy(pDst->TPA,s_number);
		
		ptr+=20;

		printf("PDU unicode: %s\n",ptr);

		nDstLength=gsmString2Bytes(ptr,buf,strlen(ptr)*2);
		nDstLength = gsmDecodeUcs2(buf, pDst->TP_UD, nDstLength);    // ת����TP-DU
	
	return 1;
	}




}
/*
int gsmDecodePdu(const char* pSrc, SM_PARAM* pDst)
{
   int nDstLength;          // Ŀ��PDU������
   unsigned char tmp;       // �ڲ��õ���ʱ�ֽڱ���
   unsigned char buf[256];  // �ڲ��õĻ�����
   int len;
		  // SMSC��ַ��Ϣ��
   gsmString2Bytes(pSrc, &tmp, 2);    // ȡ����
   tmp = (tmp - 1) * 2;    // SMSC���봮����
   pSrc += 4;              // ָ�����
   gsmSerializeNumbers(pSrc, pDst->SCA, tmp);    // ת��SMSC���뵽Ŀ��PDU��
   printf("SMSC number: %s\n",pDst->SCA);
   pSrc += tmp;        // ָ�����
										    
 // TPDU�λ����������ظ���ַ��
   gsmString2Bytes(pSrc, &tmp, 2);    // ȡ��������
   pSrc += 2;        // ָ�����
  if(tmp & 0x80)
  {
	 // �����ظ���ַ��ȡ�ظ���ַ��Ϣ
     gsmString2Bytes(pSrc, &tmp, 2);    // ȡ����
	  if(tmp & 1) tmp += 1;    // ������ż��
		pSrc += 4;          // ָ�����
      gsmSerializeNumbers(pSrc, pDst->TPA, tmp);    // ȡTP-RA����
	  printf("TP_PA: %s\n",pDst->TPA);
	  pSrc += tmp;        // ָ�����
    }
																													    
   // TPDU��Э���ʶ�����뷽ʽ���û���Ϣ��
	gsmString2Bytes(pSrc, (unsigned char*)&pDst->TP_PID, 2);    // ȡЭ���ʶ(TP-PID)
	printf("TP_PID: %s\n",pDst->TP_PID);
    pSrc += 2;        // ָ�����
    gsmString2Bytes(pSrc, (unsigned char*)&pDst->TP_DCS, 2);    // ȡ���뷽ʽ(TP-DCS)
	printf("TP_DCS: %s\n",pDst->TP_DCS);
    pSrc += 2;        // ָ�����
    gsmSerializeNumbers(pSrc, pDst->TP_SCTS, 14);        // ����ʱ����ַ���(TP_SCTS)
	printf("TP_SCTS: %s\n",pDst->TP_SCTS);
    pSrc += 14;       // ָ�����
    gsmString2Bytes(pSrc, &tmp, 2);    // �û���Ϣ����(TP-UDL)
    pSrc += 2;        // ָ�����
	if(pDst->TP_DCS == GSM_UCS2)
	  {
	
		// UCS2����
		printf("before to bytes: %s\n",pSrc);
		printf("tmp: %d\n",tmp);
		len=strlen(pSrc);
		printf("len:%d\n",len);
		nDstLength = gsmString2Bytes(pSrc, buf,tmp*2);        // ��ʽת��
		nDstLength = gsmDecodeUcs2(buf, pDst->TP_UD, nDstLength);    // ת����TP-DU
	
		}
		// ����Ŀ���ַ�������
	
	return nDstLength;
	
}
token
*/
void init_SM_PARAM(SM_PARAM *psrc,char *number)
{
	strcpy(psrc->SCA,"8613800411500");
	strcpy(psrc->TPA,number);
	psrc->TP_PID=0;
	psrc->TP_DCS=GSM_UCS2;
	strcpy(psrc->TP_UD,"���п������");
}	
	
int gsmSendMessage(const SM_PARAM* pSrc)
{
    int nPduLength;        // PDU������
	unsigned char nSmscLength;    // SMSC������
	int nLength;           // �����յ������ݳ���
	char cmd[16];          // ���
    char pdu[512];         // PDU��
	char ans[128];         // Ӧ��
				
	printf("SM_PARM  before encode: %s\n",pSrc->TP_UD);	
	    
    nPduLength = gsmEncodePdu(pSrc, pdu);    // ����PDU����������PDU��
	strcat(pdu, "\x01a");        // ��Ctrl-Z����
	
	printf("SM_PARAM after encod:%s\n",pdu);
			
						    
    gsmString2Bytes(pdu, &nSmscLength, 2);    // ȡPDU���е�SMSC��Ϣ����
    nSmscLength++;        // ���ϳ����ֽڱ���
												    
   // �����еĳ��ȣ�������SMSC��Ϣ���ȣ��������ֽڼ�
   tty_writecmd("AT+CMGF=0",strlen("AT+CMGF=0"));
 	sprintf(cmd, "AT+CMGS=%d\r", nPduLength / 2 - nSmscLength);    // ��������
	

	tty_write(cmd, strlen(cmd));    // ��������	
	
	tty_write(pdu,strlen(pdu));

	  return 1;
  }
									
