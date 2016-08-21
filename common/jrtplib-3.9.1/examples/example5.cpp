//JRTPLIB receiver on PC
//wangsu820@163.com  2008-7-1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#ifndef WIN32
#include <netinet/in.h>
#include <arpa/inet.h>
#else
#include <winsock2.h>
#endif // WIN32
#include "rtpsession.h"
#include "rtpsessionparams.h"
#include "rtpudpv4transmitter.h"
#include "rtpipv4address.h"
#include "rtperrors.h"
#include "rtppacket.h"
//链接jrtplib.lib
#ifdef WIN32
#pragma comment( lib, "jrtplib.lib" )
#pragma comment( lib, "jthread.lib" )
#pragma comment( lib, "ws2_32.lib" )
#endif
#define DEBUG

using namespace jrtplib;
//增加了这个,就不用在cout前加std::了
using namespace std;

void checkerror(int errorcode)
{
	if(errorcode < 0)
	{
		cout << "ERROR:" << RTPGetErrorString(errorcode) << endl;
		exit(-1);
	}
}
long getfilelen(FILE *fp)
{
	long cur_pos;
	long len;

	if(NULL == fp)
	{
		printf("FILE fp is NULL\n");
		exit(-1);
	}
	cur_pos = ftell(fp);
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	fseek(fp, cur_pos, SEEK_SET);
	return len;
}
int main(void)
{
#ifdef WIN32
	WSADATA dat;
	WSAStartup(MAKEWORD(2, 2), &dat);
#endif // WIN32
	RTPSession rtpsess;
	RTPSessionParams sessionparams;
	RTPUDPv4TransmissionParams transparams;
	unsigned short localport = 10000;
	string strpath = "test.ts";
	FILE *fp;
	unsigned char *recvdata;
//用于统计每次接收到数据的大小
	long recvsize = 0;
//用于实际写入到文件的数据的大小
	long writesize;
	long totalsize = 0;
	int status, timeout;
	bool flag = -1;

	cout << ">>>>>>Receive Program<<<<<<" << endl;
#ifndef RTP_SUPPORT_THREAD
	cout << "NOT support thread,you can use Poll()" << endl;
#endif
	cout << "Please input your receive port:" << endl;
	cin >> localport;
	cout << "Please input file name to be saved" << endl;
	cin >> strpath;
	char *filepath = new char[strpath.length() + 1];
	strcpy(filepath, strpath.c_str());
#ifdef DEBUG
	printf("\nthe file path you save: %s\n", filepath);
#endif
	fp = fopen(filepath, "w+");
#ifdef DEBUG
	printf("fopen %s\n", filepath);
#endif

//创建会话
	sessionparams.SetOwnTimestampUnit(1.0 / 8000.0);
	transparams.SetPortbase(localport);
	status = rtpsess.Create(sessionparams, &transparams);
	int i = 0;
	
	while(status != 0)
	{
		i++;
		transparams.SetPortbase(localport + i);
		status = rtpsess.Create(sessionparams, &transparams);
	}
	RTPIPv4Address addr1(ntohl(inet_addr("192.168.8.20")), localport);
	status |= rtpsess.AddDestination(addr1);
	status |= rtpsess.SendPacket((void *)"1234567890", 10, 0, false, 10);
	printf("status =%d \n", status);
	cout << "localport:"
		<< transparams.GetPortbase()
		<< endl;
#ifdef DEBUG
	cout << "check if Create Session error..." << endl;
#endif
	checkerror(status);
//接收数据
#ifdef DEBUG
	std::cout << "waiting for the data incoming ..." << std::endl;
#endif


	while(1)
	{
#ifndef RTP_SUPPORT_THREAD
		status = rtpsess.Poll();
		checkerror(status);
#endif
		rtpsess.BeginDataAccess();
//	RTPTime::Wait(RTPTime(1,0));
		int ret=0;
		ret=rtpsess.GotoFirstSourceWithData();
		if(ret)
		{
			printf("GotoFirstSourceWithData\n");
			do
			{
				RTPPacket *rtppack;
				printf("GotoNextSourceWithData\n");
				while((rtppack = rtpsess.GetNextPacket()) != NULL)
				{
					recvsize = rtppack->GetPayloadLength();
					totalsize += recvsize;
					printf("recv ... data size: %ldKB(%ldB)   total size: %ldKB(%ldB)\n",
						   (recvsize / 1024), recvsize,
						   (totalsize / 1024), totalsize);
					recvdata = rtppack->GetPayloadData();
					writesize = fwrite(recvdata, 1, recvsize, fp);
					if(writesize != recvsize)
					{
						printf("try to write [%ldKB(%ldB)/%ldKB(%ldB)] data into file....ERROR\n",
							   (writesize / 1024), writesize,
							   (recvsize / 1024), recvdata);
					}
					rtpsess.DeletePacket(rtppack);
				}
			}
			while(rtpsess.GotoNextSourceWithData());
		}
		rtpsess.EndDataAccess();
		
	} //while(1)
#ifdef DEBUG
	cout << "Goodbye RTP Session" << endl;
#endif
	rtpsess.BYEDestroy(RTPTime(10, 0), 0, 0);
//清除totalsize,重新统计文件大小
	totalsize = 0;
	totalsize = getfilelen(fp);
	printf("%s total size is %ldKB(%ldB)\n", filepath, (totalsize / 1024), totalsize);
#ifdef DEBUG
	cout << "do some ending job...END" << endl;
#endif
	fclose(fp);
	delete filepath;
#ifdef WIN32
	WSACleanup();
#endif // WIN32
	return 0;
}
