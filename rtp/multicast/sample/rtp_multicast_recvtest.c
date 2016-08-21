#include "rtp_multicast_api.h"

static const char *help="usage: recv  ipaddr local_port netdevice \n"
						"TIPS : make sure the local_port you input is even number";

int rtphandle = 1;
int s32Ret = RTP_MULTICAST_FAILURE; 

void print( unsigned  char *data, int datalen)
{
	printf("%s\n",data);
}

int main(int argc,char *argv[])
{
	if(argc < 4 || argc > 4)
	{
		printf("%s\n",help);
		return RTP_MULTICAST_FALSE;
	}
	
	const char *ipaddr = argv[1];
	const char *netdevice = argv[3];

	s32Ret = rtp_multicast_init();
	if(s32Ret != RTP_MULTICAST_SUCCESS)
	{
		printf("rtp_multicast_init fail\n");
		return RTP_MULTICAST_FAILURE;
	}
			
	rtp_multicast_addroute(ipaddr,netdevice); 
	
	s32Ret = rtp_multicast_create(RTP_MULTICAST_RECEIVE, 33, atoi(argv[2]), ipaddr, &rtphandle);
	if(s32Ret != RTP_MULTICAST_SUCCESS)
	{
		printf("rtp_multicast_create fail \n");
		goto RTP_CREATE_FAIL;
	}
	
	s32Ret = rtp_multicast_register_recvmsg_func(rtphandle,print);	
	if(s32Ret != RTP_MULTICAST_SUCCESS)
	{
		printf("rtp_multicast_register_recvmsg_func fail \n");
		goto REGISTER_RECVMSG_FAIL;
	}

	s32Ret = rtp_multicast_open_recvmsg(rtphandle);
	if(s32Ret != RTP_MULTICAST_SUCCESS)
	{
		printf("rtp_multicast_open_recvmsg fail \n");
		goto OPEN_RECVMSG_FAIL;
	}
	
	while(1);
	
	return RTP_MULTICAST_SUCCESS;
	
RTP_CREATE_FAIL:
	rtp_multicast_deinit();
	return RTP_MULTICAST_FAILURE;

REGISTER_RECVMSG_FAIL:
	rtp_multicast_deinit();
	rtp_multicast_destory(rtphandle);
	return RTP_MULTICAST_FAILURE;

OPEN_RECVMSG_FAIL:
	rtp_multicast_deinit();
	rtp_multicast_destory(rtphandle);
	return RTP_MULTICAST_FAILURE;
}