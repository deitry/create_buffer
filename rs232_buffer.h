/*
 * rs232_buffer.h
 *
 *  Created on: 5 окт. 2017 г.
 *      Author: dmsvo
 */

#ifndef RS232_BUFFER_H_
#define RS232_BUFFER_H_

#define DEBUG	/* в отладочном режиме подключаем stdio, чтобы можно было вывести результат в консоль */
	/* Вообще говоря, такой ключ указывается в настройках проекта, сюда вынесен для наглядности. */

/* Коды отработки функций */
#define FSTATUS_OK 			0	/* обозначаем успешную отработку функции */
#define FSTATUS_WRONGIN		1	/* ошибка - некорректные входные параметры (не числа) */
#define FSTATUS_BOUND		2	/* ошибка - параметры удалось прочитать, но они "не вписываются" в заданные границы */
#define FSTATUS_WRONGSPC	3	/* ошибка - параметры не удовлетворяют специальным условиям */

#define INPUT_CNT 			4 	/* базовое количество входных параметров для функции */
#define MESSAGE_LENGTH 		23 	/* длина сообщения */

/* ASCII-коды */
#define NUM0	0x30
#define NUM9	0x39
#define NUMA	0x41
#define NUMF	0x46
#define DIFA	NUMA-0xA	/* если к этому числу прибавить 0xA, получится код буквы A */


typedef  char uint8;	/* в рамках большого проекта этот тип скорее всего будет уже определён */


/*
 * Тип, описывающий содержимое сообщения
 */
struct Message
{
	int trkNo;		/* размер поля int принят для унификации. В реальном приложении имеет смысл оставить байт */
	int command;	/* размер поля int принят для унификации. В реальном приложении имеет смысл оставить байт */
	int price;
	int volume;
	int status;
};


int fill_buffer (uint8* buf, struct Message msg);



#endif /* RS232_BUFFER_H_ */
