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
		if (*str >= NUM0 && *str <= NUM9)
		{
			*result = (*result)*10 + *str - NUM0;
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
		if (*str >= NUM0 && *str <= NUM9)
		{
			*result = (*result)*16 + *str - NUM0;
		}
		else if (*str >= NUMA && *str <= NUMF)
		/* A..F. �������� ����� �� ��������� */
		{
			*result = (*result)*16 + *str - DIFA;
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
 * � ������� ������� � ���������� ������ ����� ����� � �������������� ������
 */
int main (int argc, char* argv[])
{
	/*
	 * �������� �� ���������� ���������� - �������, ��� �� ������ ���� 4 ��� 5 (argc �� 1 ������)
	 * (�������� ��� ������� �������� ������������, ������ ��� ��� �������� �� ���
	 * �� ������������ � ������������ ������ �������).
	 * �������� �� ������������ ������ ����� �������������� � ������� ���������� ������.
	 * � �������� ������� (�� main) ������ ����� ������� ���� ����� �������������� � ����� �������� �� �����������
	 */
	if (argc < INPUT_CNT + 1 || argc > INPUT_CNT + 2) return FSTATUS_WRONGCNT;

	/* ���������� ���������, � ������� ����� ��������� ������ */
	struct Message message;
	int status;	// ��� ����������� ������� ���������� �������

	/* �������������� ����������.
	 * ���� ������� �������� ���������� ���-��, �������� �� ����, ������ �� �������� ������� ���-�� �� ��. */
	if ((status = charToHex(argv[1], &(message.trkNo))) 	!= FSTATUS_OK) 	return status;	/* ����� ��� */
	if ((status = charToHex(argv[2], &(message.command))) 	!= FSTATUS_OK) 	return status;	/* ����� ������� */
	if ((status = charToDec(argv[3], &(message.price))) 	!= FSTATUS_OK) 	return status; /* ���� */
	if ((status = charToDec(argv[4], &(message.volume))) 	!= FSTATUS_OK) 	return status; /* ����� ���� */

	if (argc == INPUT_CNT + 2)
	{
		if ((status = charToHex(argv[5], &(message.status))) != FSTATUS_OK) return status;	/* �������� ��� ���� "������", ���� ���� */
	}
	else
	{
		message.status = 0;	/* ���� �� ������, �� ������ ������ ������������� �������������� */
	}

	/*
	 * ������������, ��� ������ ������ ��� ����� ���������� � ��� �������� ������ ���������
	 * ����� ������������ ��� ��, ������������� ������.
	 * ��� ���������� ������������� ��������� ������, ������ ��� ����� ������ ���� �������� ��� �������, � ������� �����������
	 * - ��� � �������� ���������� ����������, ���, ��� �����, � ��� �������, ������� ������������ ��� �������������
	 */
	uint8 message_buf[MESSAGE_LENGTH+1]; /* � ����� ��������� ���� �� ������ ������ ����� 0, ����� ���������� ���������. */
		/* � ������� �������� ����� ����� ��������� ���� ��������� */

	/* ������������� */
	for (uint8* ptr = message_buf; ptr < message_buf + MESSAGE_LENGTH + 1; ptr++)
	{
		*ptr = '\0';
	}

	/* ���������� ������ */
	status = fill_buffer (&message_buf[0], message);
	if (status != FSTATUS_OK)
		return status;

#ifdef DEBUG
	/* ������������� ������ - ���� �������� ������� �� ����� */
	puts(message_buf);

#endif

	return FSTATUS_OK;
}
