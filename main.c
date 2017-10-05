/*
 * main.c
 *
 *  Created on: 5 окт. 2017 г.
 *      Author: dmsvo
 */

#include "rs232_buffer.h"

#ifdef DEBUG

#include "stdio.h"

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

	/*uint8* message_buf = (uint8*) malloc(23 * sizeof(uint8));*/
	uint8 message_buf[MESSAGE_LENGTH+1];

	/* Инициализация */
	for (uint8* ptr = message_buf; ptr < message_buf + MESSAGE_LENGTH; ptr++)
	{
		*ptr = 'A';
	}

	/* заполнение буфера */
	fill_buffer ((uint8*)&message_buf[0], message);

	/* использование буфера - ради проверки выводим на экран */
	puts(message_buf);

	/* освобождение памяти */
	/*free(message_buf);*/

	return FSTATUS_OK;
}

#endif
