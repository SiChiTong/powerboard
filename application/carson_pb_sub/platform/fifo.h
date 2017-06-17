/***************************Copyright BestFu 2014-05-14*************************
��	����	Fifo.h
˵	����	ͨ�ö��д���ͷ�ļ�
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2013-09-03
��  ��:     ����
*******************************************************************************/

#ifndef __FIFO_H
#define __FIFO_H
#include "Common.h"

#include "can_protocol.h"

typedef struct
{
    CAN_ID_UNION    id;
    CAN_DATA_UNION  data;
}can_pkg_t;

typedef struct{
    can_pkg_t   *data;
    uint32_t   size;
    uint32_t   front;
    uint32_t   rear;
}can_fifo_t;



#define ERRR(conf, ret)      do              \
                            {                \
                                if (conf)    \
                                {            \
                                    ret;     \
                                }            \
                            } while(0) 
                            
//#define MIN(a, b)           ((a) > (b) ? (b) : (a))

                              
#define TRUE    1
#define FALSE   0

uint8_t FifoInit(can_fifo_t *head, can_pkg_t *buf, uint32_t len);
void FifoRst(can_fifo_t *head);
uint8_t IsFifoEmpty(can_fifo_t *head);
static uint8_t IsFifoFull(can_fifo_t *head);
static uint32_t FifoSurplusSize(can_fifo_t *head);
uint32_t FifoValidSize(can_fifo_t *head);
uint8_t FifoPuts(can_fifo_t *head, uint8_t *data, uint32_t len);
uint8_t FifoGets(can_fifo_t *head, uint8_t *data, uint32_t len);
uint8_t FifoPutc(can_fifo_t *head, can_pkg_t data);
uint8_t FifoGetc(can_fifo_t *head, can_pkg_t *data);
							
#endif //queue.h end
/**************************Copyright BestFu 2014-05-14*************************/
