/*
 * rs232_buffer.h
 *
 *  Created on: 5 окт. 2017 г.
 *      Author: dmsvo
 */

#ifndef RS232_BUFFER_H_
#define RS232_BUFFER_H_

#define DEBUG

#define HI(word) ((signed char*)&word)[1]
#define LO(word) ((signed char*)&word)[0]

#define FSTATUS_OK 		0	/* обозначаем успешную отработку функции */
#define FSTATUS_WRONGIN	1	/* некорректные входные параметры */

#define INPUT_CNT 		4 	/* количество входных параметров для функции */
#define MESSAGE_LENGTH 	23 	/* длина сообщения */

typedef  char uint8;	/* в рамках большого проекта этот тип скорее всего будет уже определён */


/*
 * Тип, описывающий содержимое сообщения
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
