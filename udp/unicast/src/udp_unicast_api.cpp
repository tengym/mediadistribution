#include "udp_unicast_api.h"

static int  udpUnicastSockfd[MAX_UDP_UNICAST_ONLINE_NUM];
static struct sockaddr_in udpUnicastSockAddr[MAX_UDP_UNICAST_ONLINE_NUM];
static bool	udpUnicastFlag[MAX_UDP_UNICAST_ONLINE_NUM];
static pthread_t udpUnicastRecvThread[MAX_UDP_UNICAST_ONLINE_NUM]; 
static UDP_UNICAST_RECVMSG_FUNC udpUnicastRecvFunc[MAX_UDP_UNICAST_ONLINE_NUM];

signed int udp_unicast_init(void)
{
	int index = 0;

	for (index = 0; index < MAX_UDP_UNICAST_ONLINE_NUM; index++)
	{
		udpUnicastFlag[index] = UDP_UNICAST_FALSE;
		udpUnicastSockfd[index] = -1;
		udpUnicastRecvFunc[index] = NULL;
	}
	
	return UDP_UNICAST_SUCCESS;
}

signed int udp_unicast_deinit(void)
{
	int index = 0;

	for (index = 0; index < MAX_UDP_UNICAST_ONLINE_NUM; index++)
	{
		if(udpUnicastFlag[index])
		{
			udp_unicast_destory(index);
		}
	}
	
	return UDP_UNICAST_SUCCESS;
}

signed int udp_unicast_create(UDP_UNICAST_TYPE type, int port, const char *ipaddr, int *udphandle)
{
	int index = 0, opt = 1;
	
	if ((NULL == ipaddr) || (NULL == udphandle))
	{
		return UDP_UNICAST_NULL;
	}
	
	for (index = 0; index < MAX_UDP_UNICAST_ONLINE_NUM; index++)
	{
		if(!udpUnicastFlag[index])
		{
			break;
		}
	}

	if ((index >= MAX_UDP_UNICAST_ONLINE_NUM) || (port <= 0))
	{
		return UDP_UNICAST_FAILURE;
	}

	udpUnicastSockfd[index] = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == udpUnicastSockfd[index])
	{
		return UDP_UNICAST_FAILURE;
	}

	udpUnicastSockAddr[index].sin_family = AF_INET;
	udpUnicastSockAddr[index].sin_port = htons(port);
	udpUnicastSockAddr[index].sin_addr.s_addr =inet_addr(ipaddr);; 

	if (UDP_UNICAST_RECEIVE == type)
	{
		setsockopt(udpUnicastSockfd[index], SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt) );
		if(bind(udpUnicastSockfd[index],(struct sockaddr *)&(udpUnicastSockAddr[index]),sizeof(struct sockaddr_in)) != 0)
		{
			close(udpUnicastSockfd[index]);
			return UDP_UNICAST_FAILURE;
		}

		struct timeval tv_out;
		tv_out.tv_sec = 3;//µÈ´ý3Ãë
		tv_out.tv_usec = 0;
		setsockopt(udpUnicastSockfd[index],SOL_SOCKET,SO_RCVTIMEO,&tv_out, sizeof(tv_out));
	}

	*udphandle = index;
	udpUnicastFlag[index] = UDP_UNICAST_TRUE;

	return UDP_UNICAST_SUCCESS;
}

signed int udp_unicast_destory(int udphandle)
{
	if ((udphandle < 0) || (udphandle >= MAX_UDP_UNICAST_ONLINE_NUM))
	{
		return UDP_UNICAST_FAILURE;
	}

	if (udpUnicastFlag[udphandle])
	{
		close(udpUnicastSockfd[udphandle]);
		udpUnicastSockfd[udphandle] = -1;
		udpUnicastFlag[udphandle] = UDP_UNICAST_FALSE;
	}

	return UDP_UNICAST_SUCCESS;
}

signed int udp_unicast_sendmsg(int udphandle, const char *data, int datalen)
{
	int length = datalen;
	int index = 0;
	int count = 0;
	
	if (NULL == data)
	{
		return UDP_UNICAST_NULL;
	}

	if ((udphandle < 0) || (udphandle >= MAX_UDP_UNICAST_ONLINE_NUM))
	{
		return UDP_UNICAST_FAILURE;
	}

	if ((!udpUnicastFlag[udphandle]) || (datalen < 0) || (-1 == udpUnicastSockfd[udphandle]))
	{
		return UDP_UNICAST_FAILURE;
	}

	if (length < UDP_UNICAST_PACKET_LEN)
	{
		sendto(udpUnicastSockfd[udphandle], data, length, 0, (struct sockaddr*)&udpUnicastSockAddr[udphandle], sizeof(struct sockaddr));
	}
	else
	{
		count = (length / UDP_UNICAST_PACKET_LEN);
		for (index = 0; index < count; index++)
		{
			sendto(udpUnicastSockfd[udphandle], (void *)&data[index * UDP_UNICAST_PACKET_LEN], UDP_UNICAST_PACKET_LEN, 0, (struct sockaddr*)&udpUnicastSockAddr[udphandle], sizeof(struct sockaddr));
		}

		if (0 != (length % UDP_UNICAST_PACKET_LEN))
		{
			sendto(udpUnicastSockfd[udphandle], (void *)&data[count * UDP_UNICAST_PACKET_LEN], (length % UDP_UNICAST_PACKET_LEN), 0, (struct sockaddr*)&udpUnicastSockAddr[udphandle], sizeof(struct sockaddr));
		}
	}

	return UDP_UNICAST_SUCCESS;
}

signed int udp_unicast_register_recvmsg_func(int udphandle, UDP_UNICAST_RECVMSG_FUNC recvfunc)
{
	if ((udphandle < 0) || (udphandle >= MAX_UDP_UNICAST_ONLINE_NUM))
	{
		return UDP_UNICAST_FAILURE;
	}

	if (!udpUnicastFlag[udphandle])
	{
		return UDP_UNICAST_FAILURE;
	}

	udpUnicastRecvFunc[udphandle] = recvfunc;

	return UDP_UNICAST_SUCCESS;
}
	
signed int udp_unicast_unregister_recvmsg_func(int udphandle)
{
	if ((udphandle < 0) || (udphandle >= MAX_UDP_UNICAST_ONLINE_NUM))
	{
		return UDP_UNICAST_FAILURE;
	}

	if (!udpUnicastFlag[udphandle])
	{
		return UDP_UNICAST_FAILURE;
	}

	udpUnicastRecvFunc[udphandle] = NULL;

	return UDP_UNICAST_SUCCESS;
}

static void* udp_unicast_recvmsg(void *args)
{
	int addr_len = 0;
	int udphandle = -1;
	unsigned char recvdata[UDP_UNICAST_PACKET_LEN];
	long recvsize = 0;
	
	if (NULL == args)
	{
		return NULL;
	}

	udphandle = *(int *)(args);

	if ((udphandle < 0) || (udphandle >= MAX_UDP_UNICAST_ONLINE_NUM))
	{
		return NULL;
	}

	if (!udpUnicastFlag[udphandle])
	{
		return NULL;
	}

	addr_len = sizeof(struct sockaddr_in);
	
	while (1)
	{
		if(NULL == udpUnicastRecvFunc[udphandle]) 
		{
			break;
		}

		recvsize = recvfrom(udpUnicastSockfd[udphandle], recvdata, sizeof(recvdata), 0, (struct sockaddr*)&udpUnicastSockAddr[udphandle], (socklen_t *)&addr_len);  
		if(-1 == recvsize)   
		{
			continue;
		}
				
		udpUnicastRecvFunc[udphandle](recvdata, recvsize);	
	}

	return NULL;
}

signed int udp_unicast_open_recvmsg(int udphandle)
{
	int ret = -1;
	static int pthreadParams;
		if ((udphandle < 0) || (udphandle >= MAX_UDP_UNICAST_ONLINE_NUM))
	{		return UDP_UNICAST_FAILURE;
	}	if (!udpUnicastFlag[udphandle])
	{		return UDP_UNICAST_FAILURE;
	}
	pthreadParams = udphandle;
	
    ret = pthread_create(&udpUnicastRecvThread[udphandle], NULL, udp_unicast_recvmsg, (void *)&pthreadParams);
    if (0 != ret)    {        return UDP_UNICAST_FAILURE;
    }	return UDP_UNICAST_SUCCESS;
}

signed int udp_unicast_close_recvmsg(int udphandle)
{
	int ret = 0;
	
	if ((udphandle < 0) || (udphandle >= MAX_UDP_UNICAST_ONLINE_NUM))
	{
		return UDP_UNICAST_FAILURE;
	}

	if (!udpUnicastFlag[udphandle])
	{
		return UDP_UNICAST_FAILURE;
	}

	if (!pthread_join(udpUnicastRecvThread[udphandle], NULL))
	{
		ret = pthread_cancel(udpUnicastRecvThread[udphandle]);
	}

	udp_unicast_unregister_recvmsg_func(udphandle);

	return ret;

}
