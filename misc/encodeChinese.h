// Big5 => GBK��

void BIG52GBK(char *szBuf)
{
　 if(!strcmp(szBuf, ""))
　　 return;
　 int nStrLen = strlen(szBuf);
　 wchar_t *pws = new wchar_t[nStrLen + 1];
　 try
　 {
　　 int nReturn = MultiByteToWideChar(950, 0, szBuf, nStrLen, pws, nStrLen + 1);
　　 BOOL bValue = false;
　　 nReturn = WideCharToMultiByte(936, 0, pws, nReturn, szBuf, nStrLen + 1, "?", &bValue);
　　 szBuf[nReturn] = 0;
　 }
　 __finally
　 {
　　 delete[] pws;
　 }
}

//---------------------------------------------------------------------------
// GBK => Big5

void GBK2BIG5(char *szBuf)
{
　 if(!strcmp(szBuf, ""))
　　 return ;
　 int nStrLen = strlen(szBuf);
　 wchar_t *pws = new wchar_t[nStrLen + 1];
　 __try
　 {
　　 MultiByteToWideChar(936, 0, szBuf, nStrLen, pws, nStrLen + 1);
　　 BOOL bValue = false;
　　 WideCharToMultiByte(950, 0, pws, nStrLen, szBuf, nStrLen + 1, "?", &bValue);
　　 szBuf[nStrLen] = 0;
　 }
　 __finally
　 {
　　 delete[] pws;
　 }
}

//----------------------------------------------------------------------------
// GB2312 => GBK
void GB2GBK(char *szBuf)
{
　 if(!strcmp(szBuf, ""))
　　 return;
　 int nStrLen = strlen(szBuf);
　 WORD wLCID = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC);
　 int nReturn = LCMapString(wLCID, LCMAP_TRADITIONAL_CHINESE, szBuf, nStrLen, NULL, 0);
　 if(!nReturn)
　　 return;
　 char *pcBuf = new char[nReturn + 1];
　 __try
　 {
　　 wLCID = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC);
　　 LCMapString(wLCID, LCMAP_TRADITIONAL_CHINESE, szBuf, nReturn, pcBuf, nReturn + 1);
　　 strncpy(szBuf, pcBuf, nReturn);
　 }
　 __finally
　 {
　　 delete[] pcBuf;
　 }
}

//---------------------------------------------------------------------------
// GBK =ゝGB2312

void GBK2GB(char *szBuf)
{
　 if(!strcmp(szBuf, ""))
　　 return;
　 int nStrLen = strlen(szBuf);
　 WORD wLCID = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_BIG5);
　 int nReturn = LCMapString(wLCID, LCMAP_SIMPLIFIED_CHINESE, szBuf, nStrLen, NULL, 0);
　 if(!nReturn)
　　 return;
　 char *pcBuf = new char[nReturn + 1];
　 __try
　 {
　　 wLCID = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_BIG5);
　　 LCMapString(wLCID, LCMAP_SIMPLIFIED_CHINESE, szBuf, nReturn, pcBuf, nReturn + 1);
　　 strncpy(szBuf, pcBuf, nReturn);
　 }
　 __finally
　 {
　　 delete []pcBuf;
　 }
}
