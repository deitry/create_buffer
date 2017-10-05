/*
 * rs232_buffer.c
 *
 *  Created on: 5 ���. 2017 �.
 *      Author: dmsvo
 */

#include "rs232_buffer.h"

/* ��������������� ������� �� �������� 16-����� � ASCII */
char hexToChar(char hex)
{
	if ((hex >= 0) && (hex <= 9))
		return 0x30 + hex;

	if ((hex >=10) && (hex <= 15))
		return 0x37 + hex; /* � 41�� ������� ���������� A, B, ... , F; 0x41 - 0xA = 0x37*/

	return 0;	/* ���� ��� ������-�� �� �������. ��������, ����� ����� � ����� ������ ���������� -1 */
}

/*
 * ������� ��������� ����������� ����� � ���������� ����� �� �������� ��������� �����
 * ������ ��������� - ������ �����
 * ������������ - ������
 * SOH - 1 ���� - ������ ������
 * + TRK_No - 2 ����� - ����� ���
 * + Command - 1 ���� - ��� ������� ����������
 * STX - 1 ���� - ������ ������ ���� ������
 * + Price 6 - ���� - ���� �������, ���
 * + Volume - 6 ���� - ���� ������
 * Status - 4 ����� - ��� ���������/������
 * ETX - 1 ���� - ������ ��������� ���� ������
 * CRC - 1 ���� - ����������� �����
 * ����� - 23 �����
 */
int fill_buffer (uint8* buf, struct Message msg)
{
	/*
	 * �������� ������������ ����������
	 */
	if (msg.trkNo > 127) return FSTATUS_WRONGIN;
	if (msg.price > 999999 || msg.price < 0) return FSTATUS_WRONGIN;
	if (msg.volume > 999999 || msg.price < 0) return FSTATUS_WRONGIN;

	/*
	 * ������ ���������� � �����
	 */
	uint8* ptr = buf;
	/* ������ ������ ��������� */
	*ptr = 0x1; ptr++;

	/* ����� ��� */
	*ptr = hexToChar(msg.trkNo / 16);
	int crc = *ptr; ptr++;
	*ptr = hexToChar(msg.trkNo % 16);
	crc ^= *ptr; ptr++;

	/* ��� ������� ���������� */
	*ptr = hexToChar(msg.command);
	crc ^= *ptr; ptr++;

	/* ������ ������ ���� ������ */
	*ptr = 0x2;
	crc ^= *ptr; ptr++;

	/* ���� ������� */
	int tmp = msg.price;
	for (int i = 0; i < 6; i++)
	{
		ptr[5-i] = hexToChar(tmp%10); /* ��������� ����� � ascii � �������� ������� ��� �����-������� ����������� */
		crc ^= ptr[5-i];
		tmp = tmp / 10;
	}
	ptr += 6;

	/* ����� ���� */
	tmp = msg.volume;
	for (int i = 0; i < 6; i++)
	{
		ptr[5-i] = hexToChar(tmp%10); /* ��������� ����� � ascii � �������� ������� ��� �����-������� ����������� */
		crc ^= ptr[5-i];
		tmp = tmp / 10;
	}
	ptr += 6;

	/* ���� �������. ����� �������������� ��� ��������� �� ��� � ��� */
	tmp = msg.status;
	for (int i = 0; i < 4; i++)
	{
		ptr[3-i] = hexToChar(tmp%16);
		crc ^= ptr[3-i];
		tmp = tmp / 16;
	}
	ptr += 4;	/* 4 ����� ��� ���� ������� �� ��� */

	/* ������ ��������� ���� ������ */
	*ptr = 0x3;
	crc ^= *ptr; ptr++;

	/* �������� ����������� ����� */
	*ptr = crc;

	return FSTATUS_OK;
}

#ifndef DEBUG

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
	uint8 message_buf[MESSAGE_LENGTH+1];

	/* ������������� */
	for (uint8* ptr = message_buf; ptr < message_buf + MESSAGE_LENGTH; ptr++)
	{
		*ptr = 'A';
	}

	/* ���������� ������ */
	fill_buffer ((uint8*)&message_buf[0], message);

	/* ������������� ������ */

	/* ������������ ������ */

	return FSTATUS_OK;
}

#endif

