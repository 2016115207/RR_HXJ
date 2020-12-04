#include "LockNew.h"

extern char key[]  ; ///= "sdfujujhgggvh";	//���ڼ��ܵ�����ַ���������λ������һ��
extern char DecryptChar(char a, char b);
extern char EncryptChar(char a, char b);
//�ͻ�id  rng����� rigcodeע���� 9λ
int Regist(u32 cid,u32 rng,u32 rigcode,u32* Date)
{
    //rigcode ������ע���� xxx xxx xxx
    char ULC[9];
    char decodeData[9];
    char temp;
    int i;
    if(rigcode/1000000000 != 0) //�յ���������9λ
    {
        return -1;
    }
    ULC[0] = KB2ASC(rigcode/100000000); //9λ
    ULC[1] = KB2ASC(rigcode/10000000%10); //8
    ULC[2] = KB2ASC(rigcode/1000000%10);
    ULC[3] = KB2ASC(rigcode/100000%10);
    ULC[4] = KB2ASC(rigcode/10000%10);
    ULC[5] = KB2ASC(rigcode/1000%10);
    ULC[6] = KB2ASC(rigcode/100%10);
    ULC[7] = KB2ASC(rigcode/10%10);
    ULC[8] = KB2ASC(rigcode%10);
    for ( i = 0; i < 9; i++) //��24���ַ����н������
    {
        char a = ULC[i];
        char b = key[i % 13];
        decodeData[i] = DecryptChar(a, b);
    }
    //����cid���%4��0���� 1.2�Ե� 2.ͷβ�Ե� 3.˳��
    switch((rng+cid*1000)%4)
    {
    case 0:
        for(i=0; i<4; i++)
        {
            temp = decodeData[i];
            decodeData[i] = decodeData[8-i];
            decodeData[8-i] =  temp;
        }
        break;
    case 1:
		for(i=0;i<6;i+=2)
		{
			temp = decodeData[i];
			decodeData[i] = decodeData[i+1] ;
			decodeData[i+1] = temp;
		}
        break;
    case 2:
		for(i=0;i<3;i+=3)
		{
			temp = decodeData[i];
			decodeData[i] = decodeData[i+2];
			decodeData[i+2]=temp;
		}
        break;
	default:  //˳��
		break;
    }
    //���ܣ��ó�������
	for (i = 0; i < 9; i++) //
    {
        char a = decodeData[i];
        char b = key[i % 13];
        decodeData[i] = DecryptChar(a, b);
    } 
	u8 year,month,day;
	if( (rng/100) == ASC2KB(decodeData[1]) && (rng/10%10) == ASC2KB(decodeData[4])&&(rng%10) == ASC2KB(decodeData[7]))
	{
		year =  ASC2KB(decodeData[0])*10+ASC2KB(decodeData[2]);
		month = ASC2KB(decodeData[3])*10+ASC2KB(decodeData[5]);
		day = ASC2KB(decodeData[6])*10+ASC2KB(decodeData[8]);
		if(year<=30&&year>=20&&month>0&&month<13&&day>0&&day<32)
		{
			//�����ڼ��ܱ���
			*Date = year+month*100+day*10000;
			return 0;
		} else 
		{
			return -1;
		}
	}else 
	{
		return -1;
	}

}

