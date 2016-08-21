#include "rtp_unicast_api.h"

#include "rtpsession.h"
#include "rtpsessionparams.h"
#include "rtpudpv4transmitter.h"
#include "rtpipv4address.h"
#include "rtperrors.h"
#include "rtppacket.h"

using namespace jrtplib;
using namespace std;

#if 1
#define debug_printf(fmt,arg...) do{if (1||strstr(__FILE__,"Cprasexml.cpp")==NULL){\
				printf("[%s:%d]"fmt,__FILE__,__LINE__,##arg);}\
			}while(0)
#else
#define debug_printf(fmt,arg...)
#endif

class RTPUnicastSession : public RTPSession
{
protected:
	void OnRTPPacket(RTPPacket *pack,const RTPTime &receivetime,const RTPAddress *senderaddress);
	void OnRTCPCompoundPacket(RTCPCompoundPacket *pack,const RTPTime &receivetime, const RTPAddress *senderaddress);
};

void RTPUnicastSession::OnRTPPacket(RTPPacket *pack,const RTPTime &receivetime,const RTPAddress *senderaddress)  
{  
	return;
}  

void RTPUnicastSession::OnRTCPCompoundPacket(RTCPCompoundPacket *pack,const RTPTime &receivetime, const RTPAddress *senderaddress)  
{  
	RTCPPacket *rtcppack;
	
	pack->GotoFirstPacket();
	
	while ((rtcppack=pack->GetNextPacket())!=NULL) 
	{
		if (rtcppack->IsKnownFormat()) 
		{
			switch (rtcppack->GetPacketType()) 
			{
				case RTCPPacket::RR:
					struct in_addr addr;
					addr.s_addr=htonl(((RTPIPv4Address*)senderaddress)->GetIP());
					debug_printf("RR from %s is no date:\n",inet_ntoa(addr));
					break;

				default:
					break;
			}
		}
	}
	
	return;
}  

static RTPUnicastSession rtpUnicastSession[MAX_RTP_UNICAST_ONLINE_NUM];
static bool	rtpUnicastSessionFlag[MAX_RTP_UNICAST_ONLINE_NUM];
static pthread_t rtpUnicastRecvThread[MAX_RTP_UNICAST_ONLINE_NUM]; 
static RTP_UNICAST_RECVMSG_FUNC rtpUnicastRecvFunc[MAX_RTP_UNICAST_ONLINE_NUM];

signed int rtp_unicast_init(void)
{
	int index = 0;

	for (index = 0; index < MAX_RTP_UNICAST_ONLINE_NUM; index++)
	{
		rtpUnicastSessionFlag[index] = RTP_UNICAST_FALSE;
		rtpUnicastRecvFunc[index] = NULL;
	}
	
	return RTP_UNICAST_SUCCESS;
}

signed int rtp_unicast_deinit(void)
{
	int index = 0;

	for (index = 0; index < MAX_RTP_UNICAST_ONLINE_NUM; index++)
	{
		if(rtpUnicastSessionFlag[index])
		{
			rtp_unicast_destory(index);
		}
	}
	
	return RTP_UNICAST_SUCCESS;
}

signed int rtp_unicast_create(int port, int payloadtype, const char *ipaddr, int *rtphandle)
{
	int ret = -1;
	int index = 0;
	RTPSessionParams sessionparams;
	RTPTime miniInterval(1.0);
	RTPUDPv4TransmissionParams transparams;
	
	if ((NULL == ipaddr) || (NULL == rtphandle))
	{
		return RTP_UNICAST_NULL;
	}
	
	for (index = 0; index < MAX_RTP_UNICAST_ONLINE_NUM; index++)
	{
		if(!rtpUnicastSessionFlag[index])
		{
			break;
		}
	}

	if ((index >= MAX_RTP_UNICAST_ONLINE_NUM) || (port <= 0))
	{
		return RTP_UNICAST_FAILURE;
	}

	sessionparams.SetOwnTimestampUnit(1.0 / 90000.0);;
	sessionparams.SetMaximumPacketSize(348*188);
	sessionparams.SetSessionBandwidth(1024 * 1024 * 10);	/*10Mbps*/
	sessionparams.SetSourceTimeoutMultiplier(10.0);
	sessionparams.SetUsePollThread(true);
	sessionparams.SetMinimumRTCPTransmissionInterval(miniInterval);

	transparams.SetRTPSendBuffer(64*1024);
	transparams.SetPortbase(port);

	ret = rtpUnicastSession[index].Create(sessionparams, &transparams);
	if (0 != ret)
	{
		printf("Error:%s-%d Create", __FUNCTION__,__LINE__);
		return RTP_UNICAST_FAILURE;
	}
	rtpUnicastSession[index].SetDefaultMark(false);	   
	rtpUnicastSession[index].SetDefaultPayloadType(payloadtype);
	rtpUnicastSession[index].SetDefaultTimestampIncrement(90000.0/25.0/3.0);

	RTPIPv4Address desaddr(ntohl(inet_addr(ipaddr)), port);
	ret = rtpUnicastSession[index].AddDestination(desaddr);
	if (0 != ret)
	{
		printf("Error:%s-%d AddDestination", __FUNCTION__,__LINE__);
		return RTP_UNICAST_FAILURE;
	}

	*rtphandle = index;
	rtpUnicastSessionFlag[index] = RTP_UNICAST_TRUE;
	
	printf("creat rtp socket success! rtphandle:%d\n", index);
	
	return RTP_UNICAST_SUCCESS;
}

signed int rtp_unicast_destory(int rtphandle)
{
	if ((rtphandle < 0) || (rtphandle >= MAX_RTP_UNICAST_ONLINE_NUM))
	{
		return RTP_UNICAST_FAILURE;
	}
	
	if (rtpUnicastSessionFlag[rtphandle])
	{
		rtpUnicastSession[rtphandle].Destroy();
		rtpUnicastSession[rtphandle].BYEDestroy(RTPTime(1, 0), 0, 0);
		rtpUnicastSessionFlag[rtphandle] = RTP_UNICAST_FALSE;
	}
	
	return RTP_UNICAST_SUCCESS;
}

signed int rtp_unicast_sendmsg(int rtphandle, const char *data, int datalen, int payloadtype)
{
	int ret = -1;
	struct timeval m_start;
	struct timeval m_end;
	long long m_timetotl;
	
	if (NULL == data)
	{
		return RTP_UNICAST_NULL;
	}

	if ((rtphandle < 0) || (rtphandle >= MAX_RTP_UNICAST_ONLINE_NUM))
	{
		return RTP_UNICAST_FAILURE;
	}

	if ((!rtpUnicastSessionFlag[rtphandle]) || (datalen < 0))
	{
		return RTP_UNICAST_FAILURE;
	}
	
	gettimeofday(&m_start, NULL); 
	RTPTime::Wait(RTPTime(0,50));
	ret = rtpUnicastSession[rtphandle].SendPacket((void *)data, datalen, payloadtype, false, (90000/25/30));
	if (0 != ret)
	{
		printf("Error(ret=%d):%s-%d SendPacket!\n", ret, __FUNCTION__,__LINE__);
		return RTP_UNICAST_FAILURE;
	}
	gettimeofday(&m_end, NULL); 

	m_timetotl+=(m_end.tv_sec-m_start.tv_sec)*1000*1000+(m_end.tv_usec-m_start.tv_usec);

	if (m_timetotl>1000*100) 
	{
		memset(&m_start,0,sizeof(m_start));
		memset(&m_end,0,sizeof(m_end));
		m_timetotl=0;
		RTPTime::Wait(RTPTime(0,80));
	}
	
	return RTP_UNICAST_SUCCESS;
}

signed int rtp_unicast_register_recvmsg_func(int rtphandle, RTP_UNICAST_RECVMSG_FUNC recvfunc)
{
	if ((rtphandle < 0) || (rtphandle >= MAX_RTP_UNICAST_ONLINE_NUM))
	{
		return RTP_UNICAST_FAILURE;
	}

	if (!rtpUnicastSessionFlag[rtphandle])
	{
		return RTP_UNICAST_FAILURE;
	}

	rtpUnicastRecvFunc[rtphandle] = recvfunc;

	return RTP_UNICAST_SUCCESS;
}

signed int rtp_unicast_unregister_recvmsg_func(int rtphandle)
{
	if ((rtphandle < 0) || (rtphandle >= MAX_RTP_UNICAST_ONLINE_NUM))
	{
		return RTP_UNICAST_FAILURE;
	}

	if (!rtpUnicastSessionFlag[rtphandle])
	{
		return RTP_UNICAST_FAILURE;
	}

	rtpUnicastRecvFunc[rtphandle] = NULL;

	return RTP_UNICAST_SUCCESS;
}

static void* rtp_unicast_recvmsg(void *args)
{
	int rtphandle = -1;
	RTPPacket *rtppack = NULL;
	unsigned char *recvdata = NULL;
	long recvsize = 0;
	
	if (NULL == args)
	{
		return NULL;
	}

	rtphandle = *(int *)(args);

	if ((rtphandle < 0) || (rtphandle >= MAX_RTP_UNICAST_ONLINE_NUM))
	{
		return NULL;
	}

	if (!rtpUnicastSessionFlag[rtphandle])
	{
		return NULL;
	}

	while (1)
	{
		if(NULL == rtpUnicastRecvFunc[rtphandle]) 
		{
			break;
		}
		
		rtpUnicastSession[rtphandle].BeginDataAccess();

		/*get data from frist source*/
		if(rtpUnicastSession[rtphandle].GotoFirstSourceWithData())
		{
			while((rtppack = rtpUnicastSession[rtphandle].GetNextPacket()) != NULL)
			{
				recvsize = rtppack->GetPayloadLength();
				recvdata = rtppack->GetPayloadData();
				
				rtpUnicastRecvFunc[rtphandle](recvdata, recvsize);
				
				rtpUnicastSession[rtphandle].DeletePacket(rtppack);  
			}
		}

		/*get data from other source*/
		while(rtpUnicastSession[rtphandle].GotoNextSourceWithData())
		{
			while((rtppack = rtpUnicastSession[rtphandle].GetNextPacket()) != NULL)
			{
				recvsize = rtppack->GetPayloadLength();
				recvdata = rtppack->GetPayloadData();

				rtpUnicastRecvFunc[rtphandle](recvdata, recvsize);
				
				rtpUnicastSession[rtphandle].DeletePacket(rtppack);  

			}
		}

		rtpUnicastSession[rtphandle].EndDataAccess();
	}

	return NULL;
}

signed int rtp_unicast_open_recvmsg(int rtphandle)
{
	int ret = -1;
	static int pthreadParams;
		if ((rtphandle < 0) || (rtphandle >= MAX_RTP_UNICAST_ONLINE_NUM))
	{		return RTP_UNICAST_FAILURE;
	}	if (!rtpUnicastSessionFlag[rtphandle])
	{		return RTP_UNICAST_FAILURE;
	}
	pthreadParams = rtphandle;

	ret = pthread_create(&rtpUnicastRecvThread[rtphandle], NULL, rtp_unicast_recvmsg, (void *)&pthreadParams);
    if (0 != ret)    {        return RTP_UNICAST_FAILURE;
    }	return RTP_UNICAST_SUCCESS;
}

signed int rtp_unicast_close_recvmsg(int rtphandle)
{
	int ret = 0;
	
	if ((rtphandle < 0) || (rtphandle >= MAX_RTP_UNICAST_ONLINE_NUM))
	{		return RTP_UNICAST_FAILURE;
	}	if (!rtpUnicastSessionFlag[rtphandle])
	{		return RTP_UNICAST_FAILURE;
	}

	if (!pthread_join(rtpUnicastRecvThread[rtphandle], NULL))
	{
		ret = pthread_cancel(rtpUnicastRecvThread[rtphandle]);
	}

	rtp_unicast_unregister_recvmsg_func(rtphandle);

	return ret;
}
