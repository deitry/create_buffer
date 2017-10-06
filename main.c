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
		if (*str >= NUM0 && *str <= NUM9)
		{
			*result = (*result)*10 + *str - NUM0;
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
		if (*str >= NUM0 && *str <= NUM9)
		{
			*result = (*result)*16 + *str - NUM0;
		}
		else if (*str >= NUMA && *str <= NUMF)
		/* A..F. Строчные буквы не принимаем */
		{
			*result = (*result)*16 + *str - DIFA;
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
 * В рабочем проекте её объявление скорее всего будет с фиксированными полями
 */
int main (int argc, char* argv[])
{
	/*
	 * проверка на количество аргументов - считаем, что их должно быть 4 или 5 (argc на 1 больше)
	 * (значение для статуса полагаем опциональным, потому что для передачи от ККМ
	 * он используется в сравнительно редких случаях).
	 * Проверка на корректность данных будет осуществляться в функции наполнения буфера.
	 * В реальной функции (не main) скорее всего входные поля будет фиксированными и такая проверка не понадобится
	 */
	if (argc < INPUT_CNT + 1 || argc > INPUT_CNT + 2) return FSTATUS_WRONGCNT;

	/* объявление структуры, в которой будем содержать данные */
	struct Message message;
	int status;	// для запоминания статуса выполнения функции

	/* инициализируем переменные.
	 * Если функция парсинга возвращает что-то, отличное от нуля, значит со входными данными что-то не то. */
	if ((status = charToHex(argv[1], &(message.trkNo))) 	!= FSTATUS_OK) 	return status;	/* номер ТРК */
	if ((status = charToHex(argv[2], &(message.command))) 	!= FSTATUS_OK) 	return status;	/* номер команды */
	if ((status = charToDec(argv[3], &(message.price))) 	!= FSTATUS_OK) 	return status; /* цена */
	if ((status = charToDec(argv[4], &(message.volume))) 	!= FSTATUS_OK) 	return status; /* объём дозы */

	if (argc == INPUT_CNT + 2)
	{
		if ((status = charToHex(argv[5], &(message.status))) != FSTATUS_OK) return status;	/* значение для поля "статус", если есть */
	}
	else
	{
		message.status = 0;	/* если не указан, на всякий случай принудительно инициализируем */
	}

	/*
	 * Предполагаем, что одного буфера нам будет достаточно и для отправки нового сообщения
	 * будем использовать его же, перезаписывая данные.
	 * Без применения динамического выделения памяти, массив под буфер должен быть объявлен вне функции, в которой формируется
	 * - или в качестве глобальной переменной, или, как здесь, в той функции, которая предполагает его использование
	 */
	uint8 message_buf[MESSAGE_LENGTH+1]; /* в самый последний байт на всякий случай пишем 0, чтобы выводилось нормально. */
		/* В рабочих условиях можно будет последний байт исключить */

	/* Инициализация */
	for (uint8* ptr = message_buf; ptr < message_buf + MESSAGE_LENGTH + 1; ptr++)
	{
		*ptr = '\0';
	}

	/* заполнение буфера */
	status = fill_buffer (&message_buf[0], message);
	if (status != FSTATUS_OK)
		return status;

#ifdef DEBUG
	/* использование буфера - ради проверки выводим на экран */
	puts(message_buf);

#endif

	return FSTATUS_OK;
}
