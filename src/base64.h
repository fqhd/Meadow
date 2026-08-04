// Source - https://stackoverflow.com/a/13935718
// Posted by LihO, modified by community. See post 'Timeline' for change history
// Retrieved 2026-08-04, License - CC BY-SA 4.0

#ifndef _BASE64_H_
#define _BASE64_H_

#include <vector>
#include <string>
typedef unsigned char BYTE;

std::string base64_encode(BYTE const* buf, unsigned int bufLen);
std::vector<BYTE> base64_decode(std::string const&);

#endif
