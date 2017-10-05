/*
 * main.c
 *
 *  Created on: 5 ���. 2017 �.
 *      Author: dmsvo
 */

#include "rs232_buffer.h"

#ifdef DEBUG

#include "stdio.h"

int main (int argc, char* argv[])
{
	/*
	 * ����� �� ������ ������, ������������, ��� ��� ����� ���� ��� � �������� ����
	 */

	/* �������� �� ���������� ����������.
	 * ���������� ���������� � main ������ �� �������, ��� ��� ���������� */

	/*if (argc != INPUT_CNT + 1)
	{
		return FSTATUS_WRONGIN;
	}*/

	struct Message message;
	message.trkNo = 5;
	message.command = 1;	/* ������� ���� ������ ��� ��� � ��������� ���������� ��� � ����� */
	message.price = 4620;	/* ��-98 �� 2017.10.05, � �������� */
	message.volume = 3000;	/* ����� ���� � �� */
	message.status = 0xFAFA; /* ��� �������� */

	/* �������� ������ ��� �����, � �������������, ��� ������ ������
	 * ��� ����� ���������� � ��� �������� ������ ���������
	 * ����� ������������ ��� ��, ������������� ������
	 */

	/*uint8* message_buf = (uint8*) malloc(23 * sizeof(uint8));*/
	uint8 message_buf[MESSAGE_LENGTH+1];

	/* ������������� */
	for (uint8* ptr = message_buf; ptr < message_buf + MESSAGE_LENGTH; ptr++)
	{
		*ptr = 'A';
	}

	/* ���������� ������ */
	fill_buffer ((uint8*)&message_buf[0], message);

	/* ������������� ������ - ���� �������� ������� �� ����� */
	puts(message_buf);

	/* ������������ ������ */
	/*free(message_buf);*/

	return FSTATUS_OK;
}

#endif
