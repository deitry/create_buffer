/*
 * main.c
 *
 *  Created on: 5 ���. 2017 �.
 *      Author: dmsvo
 */

#include "rs232_buffer.h"

#ifdef DEBUG

/* ���������� stdio ��� ������ � ������� ��������� ������ ��� ������� � �� ���������� � "��������" �������� */
#include "stdio.h"

#endif

/* ��������������� �������.
 * ��������� ���������� ����� ������� ���������, ����� ����� ���� ������� ��� ��������� ������� */
int charToDec(char* str, int* result)
{
	*result = 0;	/* �������������� */

	while (*str != '\0')
	{
		/* ����� */
		if (*str >= 0x30 && *str <= 0x39)
		{
			*result = (*result)*10 + *str - 0x30;
		}
		else
			/* ���� ���-�� �������� �� �����*/
			return FSTATUS_WRONGIN;
		str++;
	}

	return FSTATUS_OK;
}

int charToHex(char* str, int* result)
{
	*result = 0;	/* �������������� */

	while (*str != '\0')
	{
		/* ����� */
		if (*str >= 0x30 && *str <= 0x39)
		{
			*result = (*result)*16 + *str - 0x30;
		}
		else if (*str >= 0x41 && *str <= 0x46)
		/* A..F. �������� ����� �� ��������� */
		{
			*result = (*result)*16 + *str - 0x37;
		}
		else
		/* ���� ���-�� �������� �� ����-����� */
			return FSTATUS_WRONGIN;
		str++;
	}

	return FSTATUS_OK;
}

/*
 * �������� �������, ����� ����� ���� ���������.
 * � �
 */
int main (int argc, char* argv[])
{
	/*
	 * �������� �� ���������� ���������� - �������, ��� �� ������ ���� 4 ��� 5 (argc �� 1 ������)
	 * (�������� ��� ������� �������� ������������, ������ ��� ��� �������� �� ���
	 * �� ������������ � ������������ ������ �������).
	 * �������� �� ������������ ������ ����� �������������� � ������� ���������� ������.
	 * � �������� ������� (�� main) ������ ����� ������� ���� ����� ��������������
	 */
	if (argc < INPUT_CNT + 1 || argc > INPUT_CNT + 2) return FSTATUS_WRONGIN;

	/* ���������� ���������, � ������� ����� ��������� ������ */
	struct Message message;

	/* �������������� ����������.
	 * ���� ������� �������� ���������� ���-��, �������� �� ����, ������ �� �������� ������� ���-�� �� ��. */
	if (charToHex(argv[1], &(message.trkNo))) 	return FSTATUS_WRONGIN;	/* ����� ��� */
	if (charToHex(argv[2], &(message.command))) return FSTATUS_WRONGIN;	/* ����� ������� */
	if (charToDec(argv[3], &(message.price))) 	return FSTATUS_WRONGIN; /* ���� */
	if (charToDec(argv[4], &(message.volume))) 	return FSTATUS_WRONGIN; /* ����� ���� */

	if (argc == INPUT_CNT + 2)
	{
		if (charToHex(argv[5], &(message.status))) 	return FSTATUS_WRONGIN;	/* �������� ��� ���� "������", ���� ���� */
	}
	else
	{
		message.status = 0;	/* ���� �� ������, �� ������ ������ ������������� �������������� */
	}

	/*
	 * ������������, ��� ������ ������ ��� ����� ���������� � ��� �������� ������ ���������
	 * ����� ������������ ��� ��, ������������� ������
	 */

	uint8 message_buf[MESSAGE_LENGTH+1];

	/* ������������� */
	for (uint8* ptr = message_buf; ptr < message_buf + MESSAGE_LENGTH; ptr++)
	{
		*ptr = '\0';
	}

	/* ���������� ������ */
	fill_buffer ((uint8*)&message_buf[0], message);

#ifdef DEBUG
	/* ������������� ������ - ���� �������� ������� �� ����� */
	puts(message_buf);

#endif

	return FSTATUS_OK;
}
