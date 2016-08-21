#include "udp_multicast_api.h"

static const char *help="usage: recv  ipaddr local_port netdevice \n";

int udphandle = 1;
int s32Ret = UDP_MULTICAST_FAILURE; 

void print( unsigned  char *data, int datalen)
{
	printf("%s\n",data);
}

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
	
	s32Ret = udp_multicast_create(UDP_MULTICAST_RECEIVE, atoi(argv[2]), ipaddr, &udphandle);
	if(s32Ret != UDP_MULTICAST_SUCCESS)
	{
		printf("udp_multicast_create fail \n");
		goto UDP_CREATE_FAIL;
	}
	
	s32Ret = udp_multicast_register_recvmsg_func(udphandle,print);	
	if(s32Ret != UDP_MULTICAST_SUCCESS)
	{
		printf("udp_multicast_register_recvmsg_func fail \n");
		goto REGISTER_RECVMSG_FAIL;
	}

	s32Ret = udp_multicast_open_recvmsg(udphandle);
	if(s32Ret != UDP_MULTICAST_SUCCESS)
	{
		printf("udp_multicast_open_recvmsg fail \n");
		goto OPEN_RECVMSG_FAIL;
	}
	
	while(1);
	
	return UDP_MULTICAST_SUCCESS;
	
UDP_CREATE_FAIL:
	udp_multicast_deinit();
	return UDP_MULTICAST_FAILURE;

REGISTER_RECVMSG_FAIL:
	udp_multicast_deinit();
	udp_multicast_destory(udphandle);
	return UDP_MULTICAST_FAILURE;

OPEN_RECVMSG_FAIL:
	udp_multicast_deinit();
	udp_multicast_destory(udphandle);
	return UDP_MULTICAST_FAILURE;
}