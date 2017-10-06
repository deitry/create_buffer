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
#define FSTATUS_WRONGIN		1	/* ������ - ������������ ������� ��������� (�� �����) */
#define FSTATUS_BOUND		2	/* ������ - ��������� ������� ���������, �� ��� "�� �����������" � �������� ������� */
#define FSTATUS_WRONGSPC	3	/* ������ - ��������� �� ������������� ����������� �������� */

#define INPUT_CNT 			4 	/* ������� ���������� ������� ���������� ��� ������� */
#define MESSAGE_LENGTH 		23 	/* ����� ��������� */

/* ASCII-���� */
#define NUM0	0x30
#define NUM9	0x39
#define NUMA	0x41
#define NUMF	0x46
#define DIFA	NUMA-0xA	/* ���� � ����� ����� ��������� 0xA, ��������� ��� ����� A */


typedef  char uint8;	/* � ������ �������� ������� ���� ��� ������ ����� ����� ��� �������� */


/*
 * ���, ����������� ���������� ���������
 */
struct Message
{
	int trkNo;		/* ������ ���� int ������ ��� ����������. � �������� ���������� ����� ����� �������� ���� */
	int command;	/* ������ ���� int ������ ��� ����������. � �������� ���������� ����� ����� �������� ���� */
	int price;
	int volume;
	int status;
};


int fill_buffer (uint8* buf, struct Message msg);



#endif /* RS232_BUFFER_H_ */
