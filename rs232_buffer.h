/*
 * rs232_buffer.h
 *
 *  Created on: 5 ���. 2017 �.
 *      Author: dmsvo
 */

#ifndef RS232_BUFFER_H_
#define RS232_BUFFER_H_

#define DEBUG	/* � ���������� ������ ���������� stdio, ����� ����� ���� ������� ��������� � ������� */
	/* ������ ������, ����� ���� ����������� � ���������� �������, ���� ������� ��� �����������. */

/* ���� ��������� ������� */
#define FSTATUS_OK 			0	/* ���������� �������� ��������� ������� */
#define FSTATUS_WRONGCNT	1	/* ������ - ������������ ���������� ������� ���������� */
#define FSTATUS_WRONGIN		2	/* ������ - ������������ ������� ��������� (�� �����) */
#define FSTATUS_BOUND		3	/* ������ - ��������� ������� ���������, �� ��� "�� �����������" � �������� ������� */
#define FSTATUS_WRONGSPC	4	/* ������ - ��������� �� ������������� ����������� �������� */
#define FSTATUS_NOTDIGIT	0xFF	/* ������ - � ������� ��������� �����->ascii �������� �� ����� */

#define INPUT_CNT 			4 	/* ������� ���������� ������� ���������� ��� ������� */
#define MESSAGE_LENGTH 		23 	/* ����� ��������� � ������ */
#define PRC_LEN				6	/* ����� � ������ ���� ���� */
#define VOL_LEN				6	/* ����� � ������ ���� ������ */
#define STS_LEN				4	/* ����� � ������ ���� ������� */

/* ������������ �������� ��� �����. ����������� - ���� */
#define MAX_TRKNO		0x7F	/* ������������ �������� ��� ������ ������� */
#define MAX_COM			0xF		/* ������������ */
#define MAX_PRC			999999	/* ������������ �������� ��� ���� */
#define MAX_VOL			999999	/* ������������ �������� ��� ������ */

/* ASCII-���� */
#define NUM0	0x30
#define NUM9	0x39
#define NUMA	0x41
#define NUMF	0x46
#define DIFA	(NUMA-0xA)	/* ���� � ����� ����� ��������� 0xA, ��������� ��� ����� A */


typedef unsigned char uint8;	/* � ������ �������� ������� ���� ��� ������ ����� ����� ��� �������� */


/*
 * ���, ����������� ���������� ���������
 */
struct Message
{
	unsigned int trkNo;		/* ������ ���� int ������ ��� ����������. � �������� ���������� ����� ����� �������� ���� */
	unsigned int command;	/* ������ ���� int ������ ��� ����������. � �������� ���������� ����� ����� �������� ���� */
	unsigned int price;
	unsigned int volume;
	unsigned int status;
};


int fill_buffer (uint8* buf, struct Message msg);
/* ���� ��������� ������������ ���-�� ���, ���� ����� ������� ��������������� ������� �� ��������� ����� � ASCII � ������� */

#endif /* RS232_BUFFER_H_ */
