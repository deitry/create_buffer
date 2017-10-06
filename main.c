/*
 * main.c
 *
 *  Created on: 5 окт. 2017 г.
 *      Author: dmsvo
 */

#include "rs232_buffer.h"

#ifdef DEBUG

/* Подключаем stdio для вывода в консоль конечного буфера для отладки и не подключаем в "релизном" варианте */
#include "stdio.h"

#endif

/* Вспомогательные функции.
 * Результат возвращаем через входные аргументы, чтобы можно было вернуть код отработки функции */
int charToDec(char* str, int* result)
{
	*result = 0;	/* инициализируем */

	while (*str != '\0')
	{
		/* цифра */
		if (*str >= 0x30 && *str <= 0x39)
		{
			*result = (*result)*10 + *str - 0x30;
		}
		else
			/* если что-то отличное от цифры*/
			return FSTATUS_WRONGIN;
		str++;
	}

	return FSTATUS_OK;
}

int charToHex(char* str, int* result)
{
	*result = 0;	/* инициализируем */

	while (*str != '\0')
	{
		/* цифра */
		if (*str >= 0x30 && *str <= 0x39)
		{
			*result = (*result)*16 + *str - 0x30;
		}
		else if (*str >= 0x41 && *str <= 0x46)
		/* A..F. Строчные буквы не принимаем */
		{
			*result = (*result)*16 + *str - 0x37;
		}
		else
		/* если что-то отличное от хекс-цифры */
			return FSTATUS_WRONGIN;
		str++;
	}

	return FSTATUS_OK;
}

/*
 * Основная функция, чтобы можно было запустить.
 * В р
 */
int main (int argc, char* argv[])
{
	/*
	 * проверка на количество аргументов - считаем, что их должно быть 4 или 5 (argc на 1 больше)
	 * (значение для статуса полагаем опциональным, потому что для передачи от ККМ
	 * он используется в сравнительно редких случаях).
	 * Проверка на корректность данных будет осуществляться в функции наполнения буфера.
	 * В реальной функции (не main) скорее всего входные поля будет фиксированными
	 */
	if (argc < INPUT_CNT + 1 || argc > INPUT_CNT + 2) return FSTATUS_WRONGIN;

	/* объявление структуры, в которой будем содержать данные */
	struct Message message;

	/* инициализируем переменные.
	 * Если функция парсинга возвращает что-то, отличное от нуля, значит со входными данными что-то не то. */
	if (charToHex(argv[1], &(message.trkNo))) 	return FSTATUS_WRONGIN;	/* номер ТРК */
	if (charToHex(argv[2], &(message.command))) return FSTATUS_WRONGIN;	/* номер команды */
	if (charToDec(argv[3], &(message.price))) 	return FSTATUS_WRONGIN; /* цена */
	if (charToDec(argv[4], &(message.volume))) 	return FSTATUS_WRONGIN; /* объём дозы */

	if (argc == INPUT_CNT + 2)
	{
		if (charToHex(argv[5], &(message.status))) 	return FSTATUS_WRONGIN;	/* значение для поля "статус", если есть */
	}
	else
	{
		message.status = 0;	/* если не указан, на всякий случай принудительно инициализируем */
	}

	/*
	 * Предполагаем, что одного буфера нам будет достаточно и для отправки нового сообщения
	 * будем использовать его же, перезаписывая данные
	 */

	uint8 message_buf[MESSAGE_LENGTH+1];

	/* Инициализация */
	for (uint8* ptr = message_buf; ptr < message_buf + MESSAGE_LENGTH; ptr++)
	{
		*ptr = '\0';
	}

	/* заполнение буфера */
	fill_buffer ((uint8*)&message_buf[0], message);

#ifdef DEBUG
	/* использование буфера - ради проверки выводим на экран */
	puts(message_buf);

#endif

	return FSTATUS_OK;
}
