#include "udp_multicast_api.h"

static int  udpMulticastSockfd[MAX_UDP_MULTICAST_ONLINE_NUM];
static struct sockaddr_in udpMulticastSockAddr[MAX_UDP_MULTICAST_ONLINE_NUM];
static struct ip_mreq udpMulticastMreq[MAX_UDP_MULTICAST_ONLINE_NUM];  
static bool	udpMulticastFlag[MAX_UDP_MULTICAST_ONLINE_NUM];
static pthread_t udpMulticastRecvThread[MAX_UDP_MULTICAST_ONLINE_NUM]; 
static UDP_MULTICAST_RECVMSG_FUNC udpMulticastRecvFunc[MAX_UDP_MULTICAST_ONLINE_NUM];

signed int udp_multicast_init(void)
{
	int index = 0;

	for (index = 0; index < MAX_UDP_MULTICAST_ONLINE_NUM; index++)
	{
		udpMulticastFlag[index] = UDP_MULTICAST_FALSE;
		udpMulticastSockfd[index] = -1;
		udpMulticastRecvFunc[index] = NULL;
	}
	
	return UDP_MULTICAST_SUCCESS;
}

signed int udp_multicast_deinit(void)
{
	int index = 0;

	for (index = 0; index < MAX_UDP_MULTICAST_ONLINE_NUM; index++)
	{
		if(udpMulticastFlag[index])
		{
			udp_multicast_destory(index);
		}
	}
	
	return UDP_MULTICAST_SUCCESS;
}

signed int udp_multicast_create(UDP_MULTICAST_TYPE type, int port, const char *ipaddr, int *udphandle)
{
	int index = 0, opt = 1;
	
	if ((NULL == ipaddr) || (NULL == udphandle))
	{
		return UDP_MULTICAST_NULL;
	}
	
	for (index = 0; index < MAX_UDP_MULTICAST_ONLINE_NUM; index++)
	{
		if(!udpMulticastFlag[index])
		{
			break;
		}
	}

	if ((index >= MAX_UDP_MULTICAST_ONLINE_NUM) || (port <= 0))
	{
		return UDP_MULTICAST_FAILURE;
	}

	udpMulticastSockfd[index] = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == udpMulticastSockfd[index])
	{
		return UDP_MULTICAST_FAILURE;
	}

	udpMulticastSockAddr[index].sin_family = AF_INET;
	udpMulticastSockAddr[index].sin_port = htons(port);
	udpMulticastSockAddr[index].sin_addr.s_addr =inet_addr(ipaddr);; 

    struct timeval tv_out;
    tv_out.tv_sec = 0;
    tv_out.tv_usec = 200;
    setsockopt(udpMulticastSockfd[index],SOL_SOCKET,SO_RCVTIMEO,&tv_out, sizeof(tv_out));

    setsockopt(udpMulticastSockfd[index],SOL_SOCKET,SO_SNDTIMEO,&tv_out, sizeof(tv_out));

	if (UDP_MULTICAST_RECEIVE == type)
	{
		setsockopt(udpMulticastSockfd[index], SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt) );
		if(bind(udpMulticastSockfd[index],(struct sockaddr *)&(udpMulticastSockAddr[index]),sizeof(struct sockaddr_in)) != 0)
		{
			close(udpMulticastSockfd[index]);
			return UDP_MULTICAST_FAILURE;
		}

		udpMulticastMreq[index].imr_multiaddr.s_addr = inet_addr(ipaddr); 
		udpMulticastMreq[index].imr_interface.s_addr = htonl(INADDR_ANY);
		if(setsockopt(udpMulticastSockfd[index], IPPROTO_IP, IP_ADD_MEMBERSHIP, &udpMulticastMreq[index], sizeof(udpMulticastMreq[index])) < 0)
		{
			close(udpMulticastSockfd[index]);
			return UDP_MULTICAST_FAILURE;
		}
	}

	*udphandle = index;
	udpMulticastFlag[index] = UDP_MULTICAST_TRUE;

	return UDP_MULTICAST_SUCCESS;
}

signed int udp_multicast_destory(int udphandle)
{
	if ((udphandle < 0) || (udphandle >= MAX_UDP_MULTICAST_ONLINE_NUM))
	{
		return UDP_MULTICAST_FAILURE;
	}

	if (udpMulticastFlag[udphandle])
	{
		setsockopt(udpMulticastSockfd[udphandle], IPPROTO_IP, IP_DROP_MEMBERSHIP, &udpMulticastMreq[udphandle], sizeof(udpMulticastMreq[udphandle]));
		
		close(udpMulticastSockfd[udphandle]);
		udpMulticastSockfd[udphandle] = -1;
		udpMulticastFlag[udphandle] = UDP_MULTICAST_FALSE;
	}

	return UDP_MULTICAST_SUCCESS;
}

signed int udp_multicast_addroute(const char *ipaddr, const char *netdevice)
{
	char cmd[256];

	if ((NULL == ipaddr) || (NULL == netdevice))
	{
		return UDP_MULTICAST_NULL;
	}

	sprintf(cmd,"route add %s dev %s", ipaddr, netdevice);
	if (0 != system(cmd))
	{
		return UDP_MULTICAST_FAILURE;
	}
	
	return UDP_MULTICAST_SUCCESS;
}

signed int udp_multicast_sendmsg(int udphandle, const char *data, int datalen)
{
	int length = datalen;
	int index = 0;
	int count = 0;
	
	if (NULL == data)
	{
		return UDP_MULTICAST_NULL;
	}

	if ((udphandle < 0) || (udphandle >= MAX_UDP_MULTICAST_ONLINE_NUM))
	{
		return UDP_MULTICAST_FAILURE;
	}

	if ((!udpMulticastFlag[udphandle]) || (datalen < 0) || (-1 == udpMulticastSockfd[udphandle]))
	{
		return UDP_MULTICAST_FAILURE;
	}

	if (length <= UDP_MULTICAST_PACKET_LEN)
	{
		sendto(udpMulticastSockfd[udphandle], data, length, 0, (struct sockaddr*)&udpMulticastSockAddr[udphandle], sizeof(struct sockaddr));
	}
	else
	{
		count = (length / UDP_MULTICAST_PACKET_LEN);
		for (index = 0; index < count; index++)
		{
			sendto(udpMulticastSockfd[udphandle], (void *)&data[index * UDP_MULTICAST_PACKET_LEN], UDP_MULTICAST_PACKET_LEN, 0, (struct sockaddr*)&udpMulticastSockAddr[udphandle], sizeof(struct sockaddr));
		}

		if (0 != (length % UDP_MULTICAST_PACKET_LEN))
		{
			sendto(udpMulticastSockfd[udphandle], (void *)&data[count * UDP_MULTICAST_PACKET_LEN], (length % UDP_MULTICAST_PACKET_LEN), 0, (struct sockaddr*)&udpMulticastSockAddr[udphandle], sizeof(struct sockaddr));
		}
	}

	return UDP_MULTICAST_SUCCESS;
}

signed int udp_multicast_register_recvmsg_func(int udphandle, UDP_MULTICAST_RECVMSG_FUNC recvfunc)
{
	if ((udphandle < 0) || (udphandle >= MAX_UDP_MULTICAST_ONLINE_NUM))
	{
		return UDP_MULTICAST_FAILURE;
	}

	if (!udpMulticastFlag[udphandle])
	{
		return UDP_MULTICAST_FAILURE;
	}

	udpMulticastRecvFunc[udphandle] = recvfunc;

	return UDP_MULTICAST_SUCCESS;
}
	
signed int udp_multicast_unregister_recvmsg_func(int udphandle)
{
	if ((udphandle < 0) || (udphandle >= MAX_UDP_MULTICAST_ONLINE_NUM))
	{
		return UDP_MULTICAST_FAILURE;
	}

	if (!udpMulticastFlag[udphandle])
	{
		return UDP_MULTICAST_FAILURE;
	}

	udpMulticastRecvFunc[udphandle] = NULL;

	return UDP_MULTICAST_SUCCESS;
}

static void* udp_multicast_recvmsg(void *args)
{
	int addr_len = 0;
	int udphandle = -1;
	unsigned char recvdata[UDP_MULTICAST_PACKET_LEN];
	long recvsize = 0;
	
	if (NULL == args)
	{
		return NULL;
	}

	udphandle = *(int *)(args);

	if ((udphandle < 0) || (udphandle >= MAX_UDP_MULTICAST_ONLINE_NUM))
	{
		return NULL;
	}

	if (!udpMulticastFlag[udphandle])
	{
		return NULL;
	}

	addr_len = sizeof(struct sockaddr_in);
	
	while (1)
	{
		if(NULL == udpMulticastRecvFunc[udphandle]) 
		{
			break;
		}

		recvsize = recvfrom(udpMulticastSockfd[udphandle], recvdata, sizeof(recvdata), 0, (struct sockaddr*)&udpMulticastSockAddr[udphandle], (socklen_t *)&addr_len);  
		if(-1 == recvsize)   
		{
			continue;
		}
				
		udpMulticastRecvFunc[udphandle](recvdata, recvsize);	
	}

	return NULL;
}

signed int udp_multicast_open_recvmsg(int udphandle)
{
	int ret = -1;
	static int pthreadParams;

	if ((udphandle < 0) || (udphandle >= MAX_UDP_MULTICAST_ONLINE_NUM))
	{
		return UDP_MULTICAST_FAILURE;
	}

	if (!udpMulticastFlag[udphandle])
	{
		return UDP_MULTICAST_FAILURE;
	}

	pthreadParams = udphandle;

    ret = pthread_create(&udpMulticastRecvThread[udphandle], NULL, udp_multicast_recvmsg, (void *)&pthreadParams);
    if (0 != ret)
    {
        return UDP_MULTICAST_FAILURE;
    }

	return UDP_MULTICAST_SUCCESS;
}

signed int udp_multicast_close_recvmsg(int udphandle)
{
	int ret = 0;
	
	if ((udphandle < 0) || (udphandle >= MAX_UDP_MULTICAST_ONLINE_NUM))
	{
		return UDP_MULTICAST_FAILURE;
	}

	if (!udpMulticastFlag[udphandle])
	{
		return UDP_MULTICAST_FAILURE;
	}

	if (!pthread_join(udpMulticastRecvThread[udphandle], NULL))
	{
		ret = pthread_cancel(udpMulticastRecvThread[udphandle]);
	}

	udp_multicast_unregister_recvmsg_func(udphandle);

	return ret;

}
