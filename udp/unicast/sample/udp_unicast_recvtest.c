#include "udp_unicast_api.h"

static const  char *help = "Usage: recv ipaddr localport \n";

void print( unsigned  char *data, int datalen)
{
	printf("%s\n",data);
}

int udphandle = 1;
int s32Ret = UDP_UNICAST_FAILURE;

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

	s32Ret = udp_unicast_create(UDP_UNICAST_RECEIVE,atoi(argv[2]),ipaddr,&udphandle);
	if(s32Ret != UDP_UNICAST_SUCCESS)
	{
		printf("udp unicast create fail\n");
		goto UDP_UNICAST_CREATE_FAIL;
	}

	s32Ret = udp_unicast_register_recvmsg_func(udphandle,print);	
	if(s32Ret != UDP_UNICAST_SUCCESS)
	{
		printf("udp_unicast_register_recvmsg_func fail \n");
		goto UDP_UNICAST_REGISTER_RECVMSG_FAIL;
	}
	
	s32Ret = udp_unicast_open_recvmsg(udphandle);
	if(s32Ret != UDP_UNICAST_SUCCESS)
	{
		printf("udp_unicast_open_recvmsg fail \n");
		goto UDP_UNICAST_OPEN_RECVMSG_FAIL;
	}
	
	while(1);

	return UDP_UNICAST_SUCCESS;

UDP_UNICAST_CREATE_FAIL:
	udp_unicast_deinit();
	return UDP_UNICAST_FAILURE;

UDP_UNICAST_REGISTER_RECVMSG_FAIL:
	udp_unicast_deinit();
	udp_unicast_destory(udphandle);
	return UDP_UNICAST_FAILURE;

UDP_UNICAST_OPEN_RECVMSG_FAIL:
	udp_unicast_deinit();
	udp_unicast_destory(udphandle);
	return UDP_UNICAST_FAILURE;
}