#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

class CBase64
{
 public:
  CBase64();
  ~CBase64();
  bool static Encode(const unsigned char *pIn, unsigned long uInLen, string& strOut);
  bool static  Decode(const string& strIn, unsigned char * pOut, unsigned long *uOutLen);
  int  static base64Decode(char * in,int inlen);
  bool static base64Encode(char *bindata, size_t inlen, char **out, size_t *outlen);
}; 
