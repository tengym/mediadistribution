#include "udp_multicast_api.h"

static const char *help="usage: send  ipaddr local_port netdevice \n";

int udphandle = 1;
const char *data = "1234567890";
int s32Ret = UDP_MULTICAST_FAILURE; 

int main(int argc,char *argv[])
{
	if(argc < 4 || argc > 4)
	{
		printf("%s\n",help);
		return UDP_MULTICAST_FALSE;
	}
	
	const char *ipaddr = argv[1];
	const char *netdevice = argv[3];

	s32Ret = udp_multicast_init();
	if(s32Ret != UDP_MULTICAST_SUCCESS)
	{
		printf("udp_multicast_init fail\n");
		return UDP_MULTICAST_FAILURE;
	}
			
	udp_multicast_addroute(ipaddr,netdevice); 

	s32Ret = udp_multicast_create(UDP_MULTICAST_SEND, atoi(argv[2]), ipaddr, &udphandle);
	if(s32Ret != UDP_MULTICAST_SUCCESS)
	{
		goto UDP_MULTICAST_CREATE_FAIL;
	}
	
	while(1)
	{
		s32Ret = udp_multicast_sendmsg(udphandle,data,strlen(data)-1);
		if(s32Ret != UDP_MULTICAST_SUCCESS)
		{
			printf("udp_multicast_sendmsg fail \n");
			goto UDP_MULTICAST_SEND_MSG_FAIL;
		}
	}

	return UDP_MULTICAST_SUCCESS;

UDP_MULTICAST_CREATE_FAIL:
	udp_multicast_deinit();
	return UDP_MULTICAST_FAILURE;

UDP_MULTICAST_SEND_MSG_FAIL:
	udp_multicast_deinit();
	udp_multicast_destory(udphandle);
	return UDP_MULTICAST_FAILURE;
}