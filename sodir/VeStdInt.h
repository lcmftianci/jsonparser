/**
 * @file VeStdInt.h  �����˿���ֲ�Ļ�����������
 *
 * ��Ȩ����(c) 2016 - 2021 ����׿����ͨ�Ƽ��������ι�˾
 *
 *
 * @author ���ά <67904662@qq.com>
 * @version 1.0
 * @date 2016.10.3
 */
 
#ifndef VISIONESTDINT_H_
#define VISIONESTDINT_H_


typedef           char      ves8; 			//8bit�з�����
typedef  unsigned char	    veu8; 			//8bit�޷�����
typedef           short     ves16;			//16bit�з�����
typedef  unsigned short	    veu16;			//16bit�޷�����
typedef           int       ves32;			//32bit�з�����
typedef  unsigned int       veu32;			//32bit�޷�����

#if defined(_WIN32) && _MSC_VER < 1501      //vs2008������
typedef          __int64    ves64;
typedef unsigned __int64    veu64;
#else                                       //linux & dsp & vs2010
typedef          long long  ves64;
typedef unsigned long long  veu64;
#endif

typedef  float			    vef32;			 //32bit������
typedef  double			    vef64;			 //64bit������
typedef  unsigned char 	    vebool;			 //��������
typedef  unsigned int 	    vebool32;		 //32bit��������
typedef  void               vevoid;

#define vetrue              1                //����-��
#define vefalse             0                //����-��
#define venull              ((void *)0)      //��ָ��


#define vemax(a,b)          (((a) > (b)) ? (a) : (b))
#define vemin(a,b)          (((a) < (b)) ? (a) : (b))

#ifndef ve_inline
#if defined __cplusplus
#define ve_inline inline
#elif (defined WIN32 || defined WIN64) && !defined __GNUC__
#define ve_inline __inline
#else
#define ve_inline static
#endif
#endif /* ve_inline */

#ifdef WIN32
#define verestrict __restrict
#else
#define verestrict restrict
#endif

#endif //VISIONESTDINT_H_
