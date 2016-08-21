#include "rtp_unicast_api.h"

static const char *help = "Usage : send ipaddr localport\n"
					"TIPS  : make sure the localport you input is even number !!!\n";

int rtphandle = 1;
int s32Ret = RTP_UNICAST_FAILURE;
const char *data = "1234567890";

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
	
	while(1)
	{
		s32Ret = rtp_unicast_sendmsg(rtphandle, data, strlen(data)-1, 33);
		if(s32Ret != RTP_UNICAST_SUCCESS)
		{
			printf("send msg fail \n");
			goto RTP_UNICAST_SENDMSG_FAIL;
		}
	}

	return RTP_UNICAST_SUCCESS;

RTP_UNICAST_CREATE_FAIL:
	rtp_unicast_deinit();

RTP_UNICAST_SENDMSG_FAIL:
	rtp_unicast_deinit();
	rtp_unicast_destory(rtphandle);
	
	return RTP_UNICAST_FAILURE;
}