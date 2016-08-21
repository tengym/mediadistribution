#include "rtp_unicast_api.h"

static const char *help = "Usage: recv ipaddr localport \n"
					"TIPS : make sure the localport you input is even number\n";

int rtphandle = 1;
int s32Ret = RTP_UNICAST_FAILURE;

void print( unsigned  char *data, int datalen)
{
	printf("%s\n",data);
}

int main(int argc ,char *argv[])
{
	if(argc < 3 || argc > 3)
	{
		printf("%s",help);
		return RTP_UNICAST_FAILURE;
	}

	const char *ipaddr = argv[1];
	
	s32Ret = rtp_unicast_init();
	if(s32Ret != RTP_UNICAST_SUCCESS)
	{
		printf("rtp_unicast_init fail \n");
		return RTP_UNICAST_FAILURE;
	}

	s32Ret = rtp_unicast_create(atoi(argv[2]), 33, ipaddr,&rtphandle);
	if(s32Ret != RTP_UNICAST_SUCCESS)
	{
		goto RTP_UNICAST_CREATE_FAIL;
	}

	s32Ret = rtp_unicast_register_recvmsg_func(rtphandle,print);	
	if(s32Ret != RTP_UNICAST_SUCCESS)
	{
		printf("rtp_unicast_register_recvmsg_func fail \n");
		goto RTP_UNICAST_REGISTER_RECVMSG_FAIL;
	}
	
	s32Ret = rtp_unicast_open_recvmsg(rtphandle);
	if(s32Ret != RTP_UNICAST_SUCCESS)
	{
		printf("rtp_unicast_open_recvmsg fail \n");
		goto RTP_UNICAST_OPEN_RECVMSG_FAIL;
	}
	
	while(1);
	
	return RTP_UNICAST_SUCCESS;

RTP_UNICAST_CREATE_FAIL:
	rtp_unicast_deinit();
	return RTP_UNICAST_FAILURE;

RTP_UNICAST_REGISTER_RECVMSG_FAIL:
	rtp_unicast_deinit();
	rtp_unicast_destory(rtphandle);
	return RTP_UNICAST_FAILURE;

RTP_UNICAST_OPEN_RECVMSG_FAIL:
	rtp_unicast_deinit();
	rtp_unicast_destory(rtphandle);
	return RTP_UNICAST_FAILURE;
}