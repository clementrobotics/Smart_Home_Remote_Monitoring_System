#include<string.h>
#include"gprs.h"
																													
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


int main(){
char str[]="683199182806F0";
char *pdst=(char *)malloc(sizeof(char)*20);

gsmSerializeNumbers(str,pdst,14);
printf("pdst: %s\n",pdst);




return 0;
}
