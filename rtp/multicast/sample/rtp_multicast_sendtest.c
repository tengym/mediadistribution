#include "rtp_multicast_api.h"

static const char *help="usage: send  ipaddr local_port netdevice \n"
						"TIPS : make sure the local_port you input is even number";

int rtphandle = 1;
const char *data = "1234567890";
int s32Ret = RTP_MULTICAST_FAILURE; 

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
	
	s32Ret = rtp_multicast_create(RTP_MULTICAST_SEND, 33, atoi(argv[2]), ipaddr, &rtphandle);
	if(s32Ret != RTP_MULTICAST_SUCCESS)
	{
		printf("rtp multicast create fail\n");
		goto RTP_MULICAST_CREATE_FAIL;
	}
	
	while(1)
	{
		s32Ret = rtp_multicast_sendmsg(rtphandle, data, strlen(data)-1, 33);
		if(s32Ret != RTP_MULTICAST_SUCCESS)
		{
			printf("rtp_multicast_sendmsg fail \n");
			goto RTP_MULICAST_SENDMSG_FAIL;
		}

	}

	return RTP_MULTICAST_SUCCESS;
	
RTP_MULICAST_CREATE_FAIL:
	rtp_multicast_deinit();
	return RTP_MULTICAST_FAILURE;

RTP_MULICAST_SENDMSG_FAIL:
	rtp_multicast_deinit();
	rtp_multicast_destory(rtphandle);
	return RTP_MULTICAST_FAILURE;
}