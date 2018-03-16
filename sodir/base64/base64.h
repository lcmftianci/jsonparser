#include <string>
using namespace std;

class CBase64
{
 public:
  CBase64();
  ~CBase64();
  bool static Encode(const unsigned char *pIn, unsigned long uInLen, string& strOut);
  bool static  Decode(const string& strIn, unsigned char * pOut, unsigned long *uOutLen);

}; 
