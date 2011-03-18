// ssl.cpp

#define _CRT_SECURE_NO_WARNINGS
#include "ssl.h"
#include "z.h"

#include <sspi.h>

//* PENDING:
//* Certificate Chain 

static void nop() {}

SSL_SOCKET :: SSL_SOCKET(SOCKET x,int Ty,PCCERT_CONTEXT pc)
{
	X = x;
	Type = Ty;
	hCS = 0;
	hCred.dwLower = 0;
	hCred.dwUpper = 0;
	hCtx.dwLower = 0;
	hCtx.dwUpper = 0;
	memset(dn,0,1000*sizeof(TCHAR));
	InitContext = false;
	ExtraDataSize = 0;
	PendingRecvDataSize = 0;
	OurCertificate = 0;
	IsExternalCert = false;
	if (pc)
	{
		OurCertificate = pc;
		IsExternalCert = true;
	}
}

SSL_SOCKET :: ~SSL_SOCKET()
{
	if (Type == 0)
	ClientOff();
	else
	ServerOff();

	if (hCtx.dwLower || hCtx.dwLower)
	{
		DeleteSecurityContext(&hCtx);
	}

	if (hCred.dwLower || hCred.dwLower)
	{
		FreeCredentialsHandle(&hCred);
	}

	if (OurCertificate && !IsExternalCert)
	{
		CertFreeCertificateContext(OurCertificate);
		OurCertificate = 0;
	}

	if (hCS)
	CertCloseStore(hCS,0);
	hCS = 0;
}


void SSL_SOCKET :: SetDestinationName(TCHAR* n)
{
	_tcscpy(dn,n);
}

int SSL_SOCKET :: ClientOff()
{
	// Client wants to disconnect

	SECURITY_STATUS ss;
	Z<SecBuffer> OutBuffers(100);
	DWORD dwType = SCHANNEL_SHUTDOWN;
	OutBuffers[0].pvBuffer   = &dwType;
	OutBuffers[0].BufferType = SECBUFFER_TOKEN;
	OutBuffers[0].cbBuffer   = sizeof(dwType);

	sbout.cBuffers  = 1;
	sbout.pBuffers  = OutBuffers;
	sbout.ulVersion = SECBUFFER_VERSION;

	for(;;)
	{
		ss =  ApplyControlToken(&hCtx, &sbout);
		if (FAILED(ss))
		return -1;


		DWORD           dwSSPIFlags;
		DWORD           dwSSPIOutFlags;
		dwSSPIFlags = ISC_REQ_SEQUENCE_DETECT  |  ISC_REQ_REPLAY_DETECT  | ISC_REQ_CONFIDENTIALITY  | ISC_RET_EXTENDED_ERROR | ISC_REQ_ALLOCATE_MEMORY | ISC_REQ_STREAM;

		OutBuffers[0].pvBuffer   = NULL;
		OutBuffers[0].BufferType = SECBUFFER_TOKEN;
		OutBuffers[0].cbBuffer   = 0;
		sbout.cBuffers  = 1;
		sbout.pBuffers  = OutBuffers;
		sbout.ulVersion = SECBUFFER_VERSION;

		ss = InitializeSecurityContext(&hCred,&hCtx,NULL,dwSSPIFlags,0,SECURITY_NATIVE_DREP, NULL,0,&hCtx,&sbout,&dwSSPIOutFlags,0);
		if (FAILED(ss))
		return -1;

		PBYTE           pbMessage;
		DWORD           cbMessage;
		pbMessage = (BYTE *)(OutBuffers[0].pvBuffer);
		cbMessage = OutBuffers[0].cbBuffer;

		if (pbMessage != NULL && cbMessage != 0) 
		{
			int rval = ssend_p((char*)pbMessage, cbMessage);
			FreeContextBuffer(pbMessage);
			return rval;
		}
		break;
	}
	return 1;
}

int SSL_SOCKET :: ServerOff()
{
	// Server wants to disconnect
	SECURITY_STATUS ss;
	Z<SecBuffer> OutBuffers(100);
	DWORD dwType = SCHANNEL_SHUTDOWN;
	OutBuffers[0].pvBuffer   = &dwType;
	OutBuffers[0].BufferType = SECBUFFER_TOKEN;
	OutBuffers[0].cbBuffer   = sizeof(dwType);

	sbout.cBuffers  = 1;
	sbout.pBuffers  = OutBuffers;
	sbout.ulVersion = SECBUFFER_VERSION;

	for(;;)
	{
		ss =  ApplyControlToken(&hCtx, &sbout);
		if (FAILED(ss))
		return -1;


		DWORD           dwSSPIFlags;
		DWORD           dwSSPIOutFlags;
		dwSSPIFlags = ISC_REQ_SEQUENCE_DETECT  |  ISC_REQ_REPLAY_DETECT  | ISC_REQ_CONFIDENTIALITY  | ISC_RET_EXTENDED_ERROR | ISC_REQ_ALLOCATE_MEMORY | ISC_REQ_STREAM;

		OutBuffers[0].pvBuffer   = NULL;
		OutBuffers[0].BufferType = SECBUFFER_TOKEN;
		OutBuffers[0].cbBuffer   = 0;
		sbout.cBuffers  = 1;
		sbout.pBuffers  = OutBuffers;
		sbout.ulVersion = SECBUFFER_VERSION;

		ss = AcceptSecurityContext(&hCred,&hCtx,NULL,dwSSPIFlags,SECURITY_NATIVE_DREP, NULL,&sbout,&dwSSPIOutFlags,0);
		if (FAILED(ss))
		return -1;

		PBYTE           pbMessage;
		DWORD           cbMessage;
		pbMessage = (BYTE *)(OutBuffers[0].pvBuffer);
		cbMessage = OutBuffers[0].cbBuffer;

		if (pbMessage != NULL && cbMessage != 0) 
		{
			int rval = ssend_p((char*)pbMessage, cbMessage);
			FreeContextBuffer(pbMessage);
			return rval;
		}
		break;
	}
	return 1;
}


int SSL_SOCKET :: rrecv_p(char *b, int sz)
{
	// same as recv, but forces reading ALL sz
	int rs = 0;
	for (;;)
	{
		int rval = recv(X, b + rs, sz - rs, 0);
		if (rval == 0 || rval == SOCKET_ERROR)
		return rs;
		rs += rval;
		if (rs == sz)
		return rs;
	}
}

int SSL_SOCKET :: ssend_p(char *b, int sz)
{
	// same as send, but forces reading ALL sz
	int rs = 0;
	for (;;)
	{
		int rval = send(X, b + rs, sz - rs, 0);
		if (rval == 0 || rval == SOCKET_ERROR)
		return rs;
		rs += rval;
		if (rs == sz)
		return rs;
	}
}

int SSL_SOCKET :: recv_p(char *b, int sz)
{
	return recv(X, b,sz, 0);
}

int SSL_SOCKET :: send_p(char *b, int sz)
{
	return send(X, b, sz, 0);
}

int SSL_SOCKET :: s_rrecv(char* b,int sz)
{
	int rs = 0;
	for (;;)
	{
		int rval = s_recv(b + rs, sz - rs);
		if (rval == 0 || rval == SOCKET_ERROR)
		return rs;
		rs += rval;
		if (rs == sz)
		return rs;
	}
}


int SSL_SOCKET :: s_recv(char* b,int sz)
{
	SecPkgContext_StreamSizes Sizes;
	SECURITY_STATUS ss = 0;
	ss = QueryContextAttributes(&hCtx,SECPKG_ATTR_STREAM_SIZES,&Sizes);
	if (FAILED(ss))
	return -1;

	int TotalR = 0;
	int pI = 0;
	SecBuffer Buffers[5] = {0};
	SecBuffer *     pDataBuffer;
	SecBuffer *     pExtraBuffer;
	Z<char> mmsg(Sizes.cbMaximumMessage*10);


	if (PendingRecvDataSize)
	{
		if (sz <= PendingRecvDataSize)
		{
			memcpy(b,PendingRecvData,sz);
			
			// 
			Z<char> dj(PendingRecvDataSize);
			memcpy(dj,PendingRecvData,PendingRecvDataSize);
			memcpy(PendingRecvData,dj + sz,PendingRecvDataSize - sz);
			PendingRecvDataSize -= sz;
			return sz;
		}
		// else , occupied already
		memcpy(b,PendingRecvData,PendingRecvDataSize);
		sz = PendingRecvDataSize;
		PendingRecvDataSize = 0;
		return sz;
	}

	for(;;)
	{
		unsigned int dwMessage = Sizes.cbMaximumMessage;
		
		if (dwMessage > Sizes.cbMaximumMessage)
		dwMessage = Sizes.cbMaximumMessage;

		int rval = 0;
		if (ExtraDataSize)
		{
			memcpy(mmsg + pI,ExtraData,ExtraDataSize);
			pI += ExtraDataSize;
			ExtraDataSize = 0;
		}
		else
		{
			rval = recv_p(mmsg + pI,dwMessage);
			if (rval == 0 || rval == -1)
			return rval;
			pI += rval;
		}


		Buffers[0].pvBuffer     = mmsg;
		Buffers[0].cbBuffer     = pI;
		Buffers[0].BufferType   = SECBUFFER_DATA;

		Buffers[1].BufferType   = SECBUFFER_EMPTY;
		Buffers[2].BufferType   = SECBUFFER_EMPTY;
		Buffers[3].BufferType   = SECBUFFER_EMPTY;

		sbin.ulVersion = SECBUFFER_VERSION;
		sbin.pBuffers = Buffers;
		sbin.cBuffers = 4;

		ss = DecryptMessage(&hCtx,&sbin,0,NULL);
		if (ss == SEC_E_INCOMPLETE_MESSAGE)
		continue;
		if (ss != SEC_E_OK && ss != SEC_I_RENEGOTIATE && ss != SEC_I_CONTEXT_EXPIRED)
		return -1;

		pDataBuffer  = NULL;
		pExtraBuffer = NULL;
		for (int i = 0; i < 4; i++) 
		{
			if (pDataBuffer == NULL && Buffers[i].BufferType == SECBUFFER_DATA) 
			{
				pDataBuffer = &Buffers[i];
			}
			if (pExtraBuffer == NULL && Buffers[i].BufferType == SECBUFFER_EXTRA) 
			{
				pExtraBuffer = &Buffers[i];
			}
		}
		if (pExtraBuffer)
		{
			ExtraDataSize = pExtraBuffer->cbBuffer;
			ExtraData.Resize(ExtraDataSize + 10);
			memcpy(ExtraData,pExtraBuffer->pvBuffer,ExtraDataSize);
			pI = 0;
		}

		if (ss == SEC_I_RENEGOTIATE)
		{
			ss = ClientLoop();
			if (FAILED(ss))
			return -1;
		}

		

		if (pDataBuffer == 0)
		break;

		TotalR = pDataBuffer->cbBuffer;
		if (TotalR <= sz)
		{
			memcpy(b,pDataBuffer->pvBuffer,TotalR);
		}
		else
		{
			TotalR = sz;
			memcpy(b,pDataBuffer->pvBuffer,TotalR);
			PendingRecvDataSize = pDataBuffer->cbBuffer - TotalR;
			PendingRecvData.Resize(PendingRecvDataSize + 100);
			PendingRecvData.clear();
			memcpy(PendingRecvData,(char*)pDataBuffer->pvBuffer + TotalR,PendingRecvDataSize);
		}


		break;
	}



	return TotalR;
}

int SSL_SOCKET :: s_ssend(char* b,int sz)
{
	// QueryContextAttributes
	// Encrypt Message
	// ssend

	SecPkgContext_StreamSizes Sizes;
	SECURITY_STATUS ss = 0;
	ss = QueryContextAttributes(&hCtx,SECPKG_ATTR_STREAM_SIZES,&Sizes);
	if (FAILED(ss))
	return -1;

	Z<SecBuffer> Buffers(100);
	int mPos = 0;
	for(;;)
	{
		Z<char> mmsg(Sizes.cbMaximumMessage*2);
		Z<char> mhdr(Sizes.cbHeader*2);
		Z<char> mtrl(Sizes.cbTrailer*2);

		unsigned int dwMessage = sz - mPos;
		if (dwMessage == 0)
		break; // all ok!

		if (dwMessage > Sizes.cbMaximumMessage)
		{
			dwMessage = Sizes.cbMaximumMessage;
		}
		memcpy(mmsg,b + mPos,dwMessage);
		mPos += dwMessage;


		Buffers[0].pvBuffer     = mhdr;
		Buffers[0].cbBuffer     = Sizes.cbHeader;
		Buffers[0].BufferType   = SECBUFFER_STREAM_HEADER;
		Buffers[2].pvBuffer     = mtrl;
		Buffers[2].cbBuffer     = Sizes.cbTrailer;
		Buffers[2].BufferType   = SECBUFFER_STREAM_TRAILER;
		Buffers[3].pvBuffer     = 0;
		Buffers[3].cbBuffer     = 0;
		Buffers[3].BufferType   = SECBUFFER_EMPTY;
		Buffers[1].pvBuffer     = mmsg;
		Buffers[1].cbBuffer     = dwMessage;
		Buffers[1].BufferType   = SECBUFFER_DATA;

		sbin.ulVersion = SECBUFFER_VERSION;
		sbin.pBuffers = Buffers;
		sbin.cBuffers = 4;

		ss = EncryptMessage(&hCtx,0,&sbin,0);
		if (FAILED(ss))
		return -1;


		// Send this message
		int rval;
		rval = ssend_p((char*)Buffers[0].pvBuffer,Buffers[0].cbBuffer);
		if (rval != Buffers[0].cbBuffer)
		return rval;
		rval = ssend_p((char*)Buffers[1].pvBuffer,Buffers[1].cbBuffer);
		if (rval != Buffers[1].cbBuffer)
		return rval;
		rval = ssend_p((char*)Buffers[2].pvBuffer,Buffers[2].cbBuffer);
		if (rval != Buffers[2].cbBuffer)
		return rval;
	}

	return sz;
}



int SSL_SOCKET :: ClientLoop()
{
	SECURITY_STATUS ss = SEC_I_CONTINUE_NEEDED;
	Z<char> t(0x11000);
	Z<SecBuffer> bufsi(100);
	Z<SecBuffer> bufso(100);
	int pt = 0;

	// Loop using InitializeSecurityContext until success
	for(;;)
	{
		if (ss != SEC_I_CONTINUE_NEEDED && ss != SEC_E_INCOMPLETE_MESSAGE && ss != SEC_I_INCOMPLETE_CREDENTIALS)
		break;

		DWORD dwSSPIFlags = ISC_REQ_SEQUENCE_DETECT   |
		ISC_REQ_REPLAY_DETECT     |
		ISC_REQ_CONFIDENTIALITY   |
		ISC_RET_EXTENDED_ERROR    |
		ISC_REQ_ALLOCATE_MEMORY   |
		ISC_REQ_STREAM;

		dwSSPIFlags |= ISC_REQ_MANUAL_CRED_VALIDATION;
		
		if (InitContext == 0)
		{
			// Initialize sbout
			bufso[0].pvBuffer   = NULL;
			bufso[0].BufferType = SECBUFFER_TOKEN;
			bufso[0].cbBuffer   = 0;
			sbout.ulVersion = SECBUFFER_VERSION;
			sbout.cBuffers = 1;
			sbout.pBuffers = bufso;
		}
		else
		{
			// Get Some data from the remote site

			// Add also extradata?
			if (ExtraDataSize)
			{
				memcpy(t,ExtraData,ExtraDataSize);
				pt += ExtraDataSize;
				ExtraDataSize = 0;
			}


			int rval = recv(X,t + pt,0x10000,0);
			if (rval == 0 || rval == -1)
			return rval;
			pt += rval;

			// Put this data into the buffer so InitializeSecurityContext will do

			bufsi[0].BufferType = SECBUFFER_TOKEN;
			bufsi[0].cbBuffer = pt;
			bufsi[0].pvBuffer = t;
			bufsi[1].BufferType = SECBUFFER_EMPTY;
			bufsi[1].cbBuffer = 0;
			bufsi[1].pvBuffer = 0;
			sbin.ulVersion = SECBUFFER_VERSION;
			sbin.pBuffers = bufsi;
			sbin.cBuffers = 2;

			bufso[0].pvBuffer  = NULL;
			bufso[0].BufferType= SECBUFFER_TOKEN;
			bufso[0].cbBuffer  = 0;
			sbout.cBuffers      = 1;
			sbout.pBuffers      = bufso;
			sbout.ulVersion     = SECBUFFER_VERSION;

		}

		DWORD dwSSPIOutFlags = 0;

		SEC_E_INTERNAL_ERROR;
		ss = InitializeSecurityContext(
		&hCred,
		InitContext ? &hCtx : 0,
		dn,
		dwSSPIFlags,
		0,
		0,//SECURITY_NATIVE_DREP,
		InitContext ? &sbin : 0,
		0,
		InitContext ? 0 : &hCtx,
		&sbout,
		&dwSSPIOutFlags,
		0);

		if (ss == SEC_E_INCOMPLETE_MESSAGE)
		continue; // allow more

		pt = 0;

		if (FAILED(ss))
		return -1;

		if (InitContext == 0 && ss != SEC_I_CONTINUE_NEEDED)
		return -1;

		// Handle possible ExtraData
		/*		if (bufsi[1].BufferType == SECBUFFER_EXTRA)
			{
			ExtraDataSize = bufsi[1].cbBuffer;
			ExtraData.Resize(ExtraDataSize + 10);
			memcpy(ExtraData,bufsi[1].pvBuffer,ExtraDataSize);
			}
*/

		if (!InitContext)
		{
			// Send the data we got to the remote part
			//cbData = Send(OutBuffers[0].pvBuffer,OutBuffers[0].cbBuffer);
			int rval = ssend_p((char*)bufso[0].pvBuffer,bufso[0].cbBuffer);
			FreeContextBuffer(bufso[0].pvBuffer);
			if (rval != bufso[0].cbBuffer)
			return -1;
			InitContext = true;
			continue;
		}

		// Pass data to the remote site
		int rval = ssend_p((char*)bufso[0].pvBuffer,bufso[0].cbBuffer);
		FreeContextBuffer(bufso[0].pvBuffer);
		if (rval != bufso[0].cbBuffer)
		return -1;


		if (ss == S_OK)
		break; // wow!!
		
	}
	return 0;
}

int SSL_SOCKET :: ClientInit(bool NoLoop)
{
	SECURITY_STATUS ss = 0;
	if (IsExternalCert)
	{
		;
	}
	else
	{
		OurCertificate = CreateOurCertificate();
	}

	// Configure our SSL SChannel
	memset(&m_SchannelCred,0,sizeof(m_SchannelCred));
	m_SchannelCred.dwVersion = SCHANNEL_CRED_VERSION;
	m_SchannelCred.dwFlags |= SCH_CRED_NO_DEFAULT_CREDS;
	m_SchannelCred.dwFlags |= SCH_CRED_NO_DEFAULT_CREDS | SCH_CRED_NO_SYSTEM_MAPPER | SCH_CRED_REVOCATION_CHECK_CHAIN;

	if (OurCertificate)
	{
		m_SchannelCred.cCreds     = 1;
		m_SchannelCred.paCred     = &OurCertificate;
	}

	// AcquireCredentialsHandle

	ss = AcquireCredentialsHandle(0,SCHANNEL_NAME,SECPKG_CRED_OUTBOUND,0,&m_SchannelCred,0,0,&hCred,0);
	//	ss = AcquireCredentialsHandle(0,UNISP_NAME,SECPKG_CRED_OUTBOUND,0,&m_SchannelCred,0,0,&hCred,0);
	if (FAILED(ss))
	return 0;

	if (NoLoop)
	return 0;
	return ClientLoop();
}

int SSL_SOCKET :: ServerLoop()
{
	// Loop AcceptSecurityContext
	SECURITY_STATUS ss = SEC_I_CONTINUE_NEEDED;
	Z<char> t(0x11000);
	Z<SecBuffer> bufsi(100);
	Z<SecBuffer> bufso(100);
	int pt = 0;

	// Loop using InitializeSecurityContext until success
	for(;;)
	{
		if (ss != SEC_I_CONTINUE_NEEDED && ss != SEC_E_INCOMPLETE_MESSAGE && ss != SEC_I_INCOMPLETE_CREDENTIALS)
		break;

		DWORD dwSSPIFlags = ISC_REQ_SEQUENCE_DETECT   |
		ISC_REQ_REPLAY_DETECT     |
		ISC_REQ_CONFIDENTIALITY   |
		ISC_RET_EXTENDED_ERROR    |
		ISC_REQ_ALLOCATE_MEMORY   |
		ISC_REQ_STREAM;

		dwSSPIFlags |= ISC_REQ_MANUAL_CRED_VALIDATION;
		
		// Get Some data from the remote site
		int rval = recv(X,t + pt,0x10000,0);
		if (rval == 0 || rval == -1)
		return -1;
		pt += rval;

		// Put this data into the buffer so InitializeSecurityContext will do
		bufsi[0].BufferType = SECBUFFER_TOKEN;
		bufsi[0].cbBuffer = pt;
		bufsi[0].pvBuffer = t;
		bufsi[1].BufferType = SECBUFFER_EMPTY;
		bufsi[1].cbBuffer = 0;
		bufsi[1].pvBuffer = 0;
		sbin.ulVersion = SECBUFFER_VERSION;
		sbin.pBuffers = bufsi;
		sbin.cBuffers = 2;

		bufso[0].pvBuffer  = NULL;
		bufso[0].BufferType= SECBUFFER_TOKEN;
		bufso[0].cbBuffer  = 0;
		bufso[1].BufferType = SECBUFFER_EMPTY;
		bufso[1].cbBuffer = 0;
		bufso[1].pvBuffer = 0;
		sbout.cBuffers      = 2;
		sbout.pBuffers      = bufso;
		sbout.ulVersion     = SECBUFFER_VERSION;


		SEC_E_INTERNAL_ERROR;
		DWORD flg = 0;
		ss = AcceptSecurityContext(
		&hCred,
		InitContext ? &hCtx : 0,
		&sbin,
		ASC_REQ_ALLOCATE_MEMORY,0,
		InitContext ? 0 : &hCtx,
		&sbout,
		&flg,
		0);

		InitContext = true;

		if (ss == SEC_E_INCOMPLETE_MESSAGE)
		continue; // allow more

		pt = 0;

		if (FAILED(ss))
		return -1;

		if (InitContext == 0 && ss != SEC_I_CONTINUE_NEEDED)
		return -1;

		// Pass data to the remote site
		rval = ssend_p((char*)bufso[0].pvBuffer,bufso[0].cbBuffer);
		FreeContextBuffer(bufso[0].pvBuffer);
		if (rval != bufso[0].cbBuffer)
		return -1;

		if (ss == S_OK)
		break; // wow!!
		
	}
	return 0;
}

SECURITY_STATUS SSL_SOCKET :: Verify(PCCERT_CONTEXT px)
{
	if (px == 0)
	return SEC_E_WRONG_PRINCIPAL;

	// Time
	int iRc = CertVerifyTimeValidity(NULL,px->pCertInfo);
	if (iRc != 0) 
	return SEC_E_CERT_EXPIRED;

	// Chain
	CERT_CHAIN_PARA ChainPara = {0};
	PCCERT_CHAIN_CONTEXT pChainContext = NULL;
	ChainPara.cbSize = sizeof(ChainPara);
	if (!CertGetCertificateChain(0,px,0,0,&ChainPara,0,0,&pChainContext)) 
	return SEC_E_INVALID_TOKEN;

	/*		ZeroMemory(&polHttps, sizeof(HTTPSPolicyCallbackData));
		polHttps.cbStruct           = sizeof(HTTPSPolicyCallbackData);
		polHttps.dwAuthType         = AUTHTYPE_SERVER;
		polHttps.fdwChecks          = dwCertFlags;
		polHttps.pwszServerName     = pwszServerName;

		memset(&PolicyPara, 0, sizeof(PolicyPara));
		PolicyPara.cbSize            = sizeof(PolicyPara);
		PolicyPara.pvExtraPolicyPara = &polHttps;

		memset(&PolicyStatus, 0, sizeof(PolicyStatus));
		PolicyStatus.cbSize = sizeof(PolicyStatus);

		if (!CertVerifyCertificateChainPolicy(
							CERT_CHAIN_POLICY_SSL,
							pChainContext,
							&PolicyPara,
							&PolicyStatus)) {
			Status = ::GetLastError();
			SetLastError(Status);
			break;
		}
*/

	PCCERT_CONTEXT j[2];
	j[0] = px;
	CERT_REVOCATION_STATUS cs = {0};
	cs.cbSize = sizeof(cs);
	SECURITY_STATUS ss = 
	CertVerifyRevocation(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,CERT_CONTEXT_REVOCATION_TYPE,
	1,(void**)j,0,0,&cs);


	if (pChainContext) 
	CertFreeCertificateChain(pChainContext);

	return ss;
}


void SSL_SOCKET :: GetCertificateInfoString(TCHAR* s)
{
	PCCERT_CONTEXT pRemoteCertContext = NULL;
	SECURITY_STATUS Status = QueryContextAttributes(&hCtx,SECPKG_ATTR_REMOTE_CERT_CONTEXT,(PVOID)&pRemoteCertContext);
	if (Status != SEC_E_OK) 
	return;

	CertGetNameString(   
	pRemoteCertContext,   
	CERT_NAME_FRIENDLY_DISPLAY_TYPE,   
	0,
	NULL,   
	s,   
	1000);
	CertFreeCertificateContext(pRemoteCertContext);
}

SECURITY_STATUS SSL_SOCKET :: VerifySessionCertificate()
{
	PCCERT_CONTEXT pRemoteCertContext = NULL;
	SECURITY_STATUS Status = QueryContextAttributes(&hCtx,SECPKG_ATTR_REMOTE_CERT_CONTEXT,(PVOID)&pRemoteCertContext);
	if (Status != SEC_E_OK) 
	return Status;
	Status = Verify(pRemoteCertContext);
	CertFreeCertificateContext(pRemoteCertContext);
	return Status;
}

void SSL_SOCKET :: NoFail(HRESULT hr)
{
	if (FAILED(hr))
	throw;
}

PCCERT_CONTEXT SSL_SOCKET :: CreateOurCertificate()
{
	// CertCreateSelfSignCertificate(0,&SubjectName,0,0,0,0,0,0);
	HRESULT hr = 0;
	HCRYPTPROV hProv = NULL;
	PCCERT_CONTEXT p = 0;
	HCRYPTKEY hKey = 0;
	CERT_NAME_BLOB sib = { 0 };
	BOOL AX = 0;

	// Step by step to create our own certificate
	try
	{
		// Create the subject
		char cb[1000] = {0};
		sib.pbData = (BYTE*)cb; 
		sib.cbData = 1000;
		wchar_t*	szSubject= L"CN=Certificate";
		if (!CertStrToName(CRYPT_ASN_ENCODING, szSubject,0,0,sib.pbData,&sib.cbData,NULL))
		throw;
		

		// Acquire Context
		wchar_t* pszKeyContainerName = L"Container";

		if (!CryptAcquireContext(&hProv,pszKeyContainerName,MS_DEF_PROV,PROV_RSA_FULL,CRYPT_NEWKEYSET | CRYPT_MACHINE_KEYSET))
		{
			hr = GetLastError();
			if (GetLastError() == NTE_EXISTS)
			{
				if (!CryptAcquireContext(&hProv,pszKeyContainerName,MS_DEF_PROV,PROV_RSA_FULL,CRYPT_MACHINE_KEYSET))
				{
					throw;
				}
			}
			else
			throw;
		}

		// Generate KeyPair
		if (!CryptGenKey(hProv, AT_KEYEXCHANGE, CRYPT_EXPORTABLE, &hKey))
		throw;

		// Generate the certificate
		CRYPT_KEY_PROV_INFO kpi = {0};
		kpi.pwszContainerName = pszKeyContainerName;
		kpi.pwszProvName = MS_DEF_PROV;
		kpi.dwProvType = PROV_RSA_FULL;
		kpi.dwFlags = CERT_SET_KEY_CONTEXT_PROP_ID;
		kpi.dwKeySpec = AT_KEYEXCHANGE;

		SYSTEMTIME et;
		GetSystemTime(&et);
		et.wYear += 1;

		CERT_EXTENSIONS exts = {0};
		p = CertCreateSelfSignCertificate(hProv,&sib,0,&kpi,NULL,NULL,&et,&exts);

		AX = CryptFindCertificateKeyProvInfo(p,CRYPT_FIND_MACHINE_KEYSET_FLAG,NULL) ;
		/*		hCS = CertOpenStore(CERT_STORE_PROV_MEMORY,0,0,CERT_STORE_CREATE_NEW_FLAG,0);
		AX = CertAddCertificateContextToStore(hCS,p,CERT_STORE_ADD_NEW,0);
		AX = CryptFindCertificateKeyProvInfo(p,CRYPT_FIND_MACHINE_KEYSET_FLAG,NULL);*/
	}
	
	catch(...)
	{
	}
	
	if (hKey)
	CryptDestroyKey(hKey);
	hKey = 0;
	
	if (hProv)
	CryptReleaseContext(hProv,0);
	hProv = 0;
	return p;
}

int SSL_SOCKET :: ServerInit(bool NoLoop)
{
	SECURITY_STATUS ss = 0;

	/*	if (wcslen(un))
		{
		// Find certificate in the store
		// Open Certificate Store
		hCS = CertOpenSystemStore(0,_T("MY"));
		if (!hCS)
			return -1;

		CERT_RDN cert_rdn;
		CERT_RDN_ATTR cert_rdn_attr;

		cert_rdn.cRDNAttr = 1;
		cert_rdn.rgRDNAttr = &cert_rdn_attr;

		cert_rdn_attr.pszObjId = szOID_COMMON_NAME;
		cert_rdn_attr.dwValueType = CERT_RDN_ANY_TYPE;
		cert_rdn_attr.Value.cbData = (DWORD)wcslen(un);

		cert_rdn_attr.Value.pbData = (BYTE *)un;
		OurCertificate = CertFindCertificateInStore(hCS, X509_ASN_ENCODING | PKCS_7_ASN_ENCODING ,0,cft,&cert_rdn,NULL);
		}*/
	if (IsExternalCert)
	{
		;
	}
	else
	{
		//BOOL AX;
		OurCertificate = CreateOurCertificate();
	}

	// Configure our SSL SChannel
	memset(&m_SchannelCred,0,sizeof(m_SchannelCred));
	m_SchannelCred.dwVersion = SCHANNEL_CRED_VERSION;
	m_SchannelCred.dwFlags |= SCH_CRED_NO_DEFAULT_CREDS;
	m_SchannelCred.dwFlags = SCH_CRED_NO_DEFAULT_CREDS | SCH_CRED_NO_SYSTEM_MAPPER | SCH_CRED_REVOCATION_CHECK_CHAIN;
	m_SchannelCred.hRootStore = hCS;
	m_SchannelCred.dwMinimumCipherStrength = 128;




	if (OurCertificate)
	{
		m_SchannelCred.cCreds     = 1;
		m_SchannelCred.paCred     = &OurCertificate;
	}

	// AcquireCredentialsHandle

	ss = AcquireCredentialsHandle(0,SCHANNEL_NAME,SECPKG_CRED_INBOUND,0,&m_SchannelCred,0,0,&hCred,0);
	//	ss = AcquireCredentialsHandle(0,UNISP_NAME,SECPKG_CRED_INBOUND,0,&m_SchannelCred,0,0,&hCred,0);
	if (FAILED(ss))
	return -1;

	if (NoLoop)
	return 0;
	return ServerLoop();
}
