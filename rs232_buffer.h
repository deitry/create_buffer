/*
 * rs232_buffer.h
 *
 *  Created on: 5 ���. 2017 �.
 *      Author: dmsvo
 */

#ifndef RS232_BUFFER_H_
#define RS232_BUFFER_H_

#define DEBUG

#define HI(word) ((signed char*)&word)[1]
#define LO(word) ((signed char*)&word)[0]

#define FSTATUS_OK 		0	/* ���������� �������� ��������� ������� */
#define FSTATUS_WRONGIN	1	/* ������������ ������� ��������� */

#define INPUT_CNT 		4 	/* ���������� ������� ���������� ��� ������� */
#define MESSAGE_LENGTH 	23 	/* ����� ��������� */

typedef  char uint8;	/* � ������ �������� ������� ���� ��� ������ ����� ����� ��� �������� */


/*
 * ���, ����������� ���������� ���������
 */
struct Message
{
	char trkNo;
	char command;
	int price;
	int volume;
	int status;
};


int fill_buffer (uint8* buf, struct Message msg);

#endif /* RS232_BUFFER_H_ */
