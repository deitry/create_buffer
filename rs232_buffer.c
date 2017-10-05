/*
 * rs232_buffer.c
 *
 *  Created on: 5 окт. 2017 г.
 *      Author: dmsvo
 */

#include "rs232_buffer.h"

/* Вспомогательная функция по переводу 16-цифры в ASCII */
char hexToChar(char hex)
{
	if ((hex >= 0) && (hex <= 9))
		return 0x30 + hex;

	if ((hex >=10) && (hex <= 15))
		return 0x37 + hex; /* с 41го символа начинается A, B, ... , F; 0x41 - 0xA = 0x37*/

	return 0;	/* если нам почему-то не подошло. Возможно, имеет смысл в таком случае возвращать -1 */
}

/*
 * Функция принимает необходимые числа и составляет буфер по правилам протокола Искра
 * формат сообщения - список полей
 * наименование - размер
 * SOH - 1 байт - маркер начала
 * + TRK_No - 2 байта - номер ТРК
 * + Command - 1 байт - код команды управления
 * STX - 1 байт - маркер начала поля данных
 * + Price 6 - байт - цена топлива, коп
 * + Volume - 6 байт - доза налива
 * Status - 4 байта - код состояния/ошибки
 * ETX - 1 байт - маркер окончания поля данных
 * CRC - 1 байт - контрольная сумма
 * всего - 23 байта
 */
int fill_buffer (uint8* buf, struct Message msg)
{
	/*
	 * Проверка корректности параметров
	 */
	if (msg.trkNo > 127) return FSTATUS_WRONGIN;
	if (msg.price > 999999 || msg.price < 0) return FSTATUS_WRONGIN;
	if (msg.volume > 999999 || msg.price < 0) return FSTATUS_WRONGIN;

	/*
	 * Запись параметров в буфер
	 */
	uint8* ptr = buf;
	/* маркер начала сообщения */
	*ptr = 0x1; ptr++;

	/* номер ТРК */
	*ptr = hexToChar(msg.trkNo / 16);
	int crc = *ptr; ptr++;
	*ptr = hexToChar(msg.trkNo % 16);
	crc ^= *ptr; ptr++;

	/* код команды управления */
	*ptr = hexToChar(msg.command);
	crc ^= *ptr; ptr++;

	/* маркер начала поля данных */
	*ptr = 0x2;
	crc ^= *ptr; ptr++;

	/* цена топлива */
	int tmp = msg.price;
	for (int i = 0; i < 6; i++)
	{
		ptr[5-i] = hexToChar(tmp%10); /* переводим число в ascii в обратном порядке для какой-никакой оптимизации */
		crc ^= ptr[5-i];
		tmp = tmp / 10;
	}
	ptr += 6;

	/* объём дозы */
	tmp = msg.volume;
	for (int i = 0; i < 6; i++)
	{
		ptr[5-i] = hexToChar(tmp%10); /* переводим число в ascii в обратном порядке для какой-никакой оптимизации */
		crc ^= ptr[5-i];
		tmp = tmp / 10;
	}
	ptr += 6;

	/* поле статуса. Может использоваться при передачах от ККМ к ТРК */
	tmp = msg.status;
	for (int i = 0; i < 4; i++)
	{
		ptr[3-i] = hexToChar(tmp%16);
		crc ^= ptr[3-i];
		tmp = tmp / 16;
	}
	ptr += 4;	/* 4 байта для кода статуса от ТРК */

	/* маркер окончания поля данных */
	*ptr = 0x3;
	crc ^= *ptr; ptr++;

	/* проверка контрольной суммы */
	*ptr = crc;

	return FSTATUS_OK;
}

#ifndef DEBUG

int main (int argc, char* argv[])
{
	/*
	 * Чтобы не писать парсер, предполагаем, что все числа даны уже в понятном виде
	 */

	/* проверка на количество параметров.
	 * Количество параметров в main больше на единицу, чем нас интересует */

	/*if (argc != INPUT_CNT + 1)
	{
		return FSTATUS_WRONGIN;
	}*/

	struct Message message;
	message.trkNo = 5;
	message.command = 1;	/* задание дозы налива для ТРК и установка готовности ТРК к пуску */
	message.price = 4620;	/* АИ-98 на 2017.10.05, в копейках */
	message.volume = 3000;	/* объём дозы в мл */
	message.status = 0xFAFA; /* для проверки */

	/* выделяем память под буфер, в предположении, что одного буфера
	 * нам будет достаточно и для отправки нового сообщения
	 * будем использовать его же, перезаписывая данные
	 */
	uint8 message_buf[MESSAGE_LENGTH+1];

	/* Инициализация */
	for (uint8* ptr = message_buf; ptr < message_buf + MESSAGE_LENGTH; ptr++)
	{
		*ptr = 'A';
	}

	/* заполнение буфера */
	fill_buffer ((uint8*)&message_buf[0], message);

	/* использование буфера */

	/* освобождение памяти */

	return FSTATUS_OK;
}

#endif

