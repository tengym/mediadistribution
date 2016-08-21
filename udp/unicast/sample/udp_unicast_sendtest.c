#include "udp_unicast_api.h"

static const char *help = "Usage : send ipaddr localport\n";

int udphandle = 1;
int s32Ret = UDP_UNICAST_FAILURE;
const char *data = "1234567890";

int main(int argc ,char *argv[])
{
	if(argc < 3 || argc > 3)
	{
		printf("%s",help);
		return UDP_UNICAST_FAILURE;
	}

	const char *ipaddr = argv[1];
	
	s32Ret = udp_unicast_init();
	if(s32Ret != UDP_UNICAST_SUCCESS)
	{
		printf("udp_unicast_init fail \n");
		return UDP_UNICAST_FAILURE;
	}
	
	s32Ret = udp_unicast_create(UDP_UNICAST_SEND,atoi(argv[2]),ipaddr,&udphandle);
	if(s32Ret != UDP_UNICAST_SUCCESS)
	{
		goto UDP_UNICAST_CREATE_FAIL;
	}
	
	while(1)
	{
		s32Ret = udp_unicast_sendmsg(udphandle,data,strlen(data)-1);
		if(s32Ret != UDP_UNICAST_SUCCESS)
		{
			printf("send msg fail \n");
			goto UDP_UNICAST_SENDMSG_FAIL;
		}
	}

	return UDP_UNICAST_SUCCESS;

UDP_UNICAST_CREATE_FAIL:
	udp_unicast_deinit();
	return UDP_UNICAST_FAILURE;

UDP_UNICAST_SENDMSG_FAIL:
	udp_unicast_deinit();
	udp_unicast_destory(udphandle);
	return UDP_UNICAST_FAILURE;
}