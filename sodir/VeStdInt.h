/**
 * @file VeStdInt.h  定义了可移植的基本数据类型
 *
 * 版权所有(c) 2016 - 2021 北京卓视智通科技有限责任公司
 *
 *
 * @author 吴柯维 <67904662@qq.com>
 * @version 1.0
 * @date 2016.10.3
 */
 
#ifndef VISIONESTDINT_H_
#define VISIONESTDINT_H_


typedef           char      ves8; 			//8bit有符号数
typedef  unsigned char	    veu8; 			//8bit无符号数
typedef           short     ves16;			//16bit有符号数
typedef  unsigned short	    veu16;			//16bit无符号数
typedef           int       ves32;			//32bit有符号数
typedef  unsigned int       veu32;			//32bit无符号数

#if defined(_WIN32) && _MSC_VER < 1501      //vs2008及以下
typedef          __int64    ves64;
typedef unsigned __int64    veu64;
#else                                       //linux & dsp & vs2010
typedef          long long  ves64;
typedef unsigned long long  veu64;
#endif

typedef  float			    vef32;			 //32bit浮点数
typedef  double			    vef64;			 //64bit浮点数
typedef  unsigned char 	    vebool;			 //布尔变量
typedef  unsigned int 	    vebool32;		 //32bit布尔变量
typedef  void               vevoid;

#define vetrue              1                //布尔-真
#define vefalse             0                //布尔-假
#define venull              ((void *)0)      //空指针


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
