
#include "fifo.h"


static uint32_t FifoSurplusSize(can_fifo_t *head); //����ʣ��ռ��ж�
static uint8_t IsFifoFull(can_fifo_t *head);         //�����ж�

/*******************************************************************************
�� �� ����  fifo_Init
����˵����  ���г�ʼ��
��	  ����  *head:  ����ͷ
            *data:  ���ݴ洢�׵�ַ
            len:    ���ݴ洢���򳤶�
�� �� ֵ��  ��ʼ����� TRUE/FALSE
*******************************************************************************/
uint8_t FifoInit(can_fifo_t *head, can_pkg_t *buf, uint32_t len)
{
    ERRR(head == NULL, return FALSE);
    head->data = buf;
    head->size = len;
    head->front = head->rear = 0;

    return TRUE;
}

/*******************************************************************************
�� �� ����  fifo_Clr
����˵����  �������
��	  ����  *head:  ����ͷ
�� �� ֵ��  ��
*******************************************************************************/
void FifoRst(can_fifo_t *head)
{
    ERRR(head == NULL, return);
    head->front = 0;
	head->rear = 0;
}
/*******************************************************************************
�� �� ����  fifo_empty
����˵����  �ж϶����Ƿ�Ϊ��
��	  ����  *head:  ����ͷ
�� �� ֵ��  TRUE(����Ϊ��)/FALSE
*******************************************************************************/
uint8_t IsFifoEmpty(can_fifo_t *head)
{    
    return ((head->front == head->rear) ? TRUE : FALSE);
}

/*******************************************************************************
�� �� ����  fifo_full
����˵����  �ж϶����Ƿ�����
��	  ����  *head:  ����ͷ
�� �� ֵ��  TRUE(��������)/FALSE
*******************************************************************************/
static uint8_t IsFifoFull(can_fifo_t *head)
{
    
    return ((head->front == ((head->rear+1)%head->size)) ? TRUE : FALSE);
}

/*******************************************************************************
�� �� ����  fifo_surplusSize
����˵����  �ж϶���ʣ��ռ��С 
��	  ����  *head:  ����ͷ
�� �� ֵ��  ʣ��ռ��С(�ַ�Ϊ��λ)
*******************************************************************************/
static uint32_t FifoSurplusSize(can_fifo_t *head)
{

    return ((head->front > head->rear)
            ? (head->front - head->rear - 1)
            : (head->size + head->front - head->rear - 1));
}

/*******************************************************************************
�� �� ����  fifo_validSize
����˵����  ��ѯ������Ч�ռ��С
��	  ����  *head:  ����ͷ
�� �� ֵ��  ʣ��ռ��С(�ַ�Ϊ��λ)
*******************************************************************************/
uint32_t FifoValidSize(can_fifo_t *head)
{

	return ((head->rear < head->front)
			? (head->rear + head->size - head->front)
			: (head->rear - head->front));
}

/*******************************************************************************
�� �� ����  kfifo_puts
����˵����  ���
��	  ����  *head:  ����ͷ
            *data:  �������
            len:    ���ݳ���
�� �� ֵ��  ��ӽ�� TRUE/FALSE
*******************************************************************************/
uint8_t FifoPuts(can_fifo_t *head, uint8_t *data, uint32_t len)
{  
	uint32_t size;

    ERRR(head == NULL, return FALSE);
    ERRR(len > FifoSurplusSize(head), return FALSE); //�ж϶������Ƿ��д洢�ռ�
    
	size = MIN(len, head->size - head->rear);
	memcpy(head->data + head->rear, data, size);
	memcpy(head->data, data + size, len - size);

	head->rear = (head->rear + len)%head->size;

    return TRUE;   
}

/*******************************************************************************
�� �� ����  kfifo_gets
����˵����  ����
��	  ����  *head:  ����ͷ
            *data:  ��������
            len:    �������ݳ���
�� �� ֵ��  ���ӽ�� TRUE/FALSE
*******************************************************************************/
uint8_t FifoGets(can_fifo_t *head, uint8_t *data, uint32_t len)
{
	uint32_t size;

    ERRR(head == NULL, return FALSE);
    ERRR(IsFifoEmpty(head) == TRUE, return FALSE); //����Ϊ��
    ERRR(len > FifoValidSize(head), return FALSE); //�洢����С��Ҫ��ȡ����

	size = MIN(len, head->size - head->front);
	memcpy(data, head->data + head->front, size);
	memcpy(data+size, head->data, len - size);

	head->front = (head->front + len)%head->size;

    return TRUE;   
}

/*******************************************************************************
�� �� ����  fifo_putc
����˵����  ���һ���ַ�
��	  ����  *head:  ����ͷ
            data:   Ҫ��ӵ�����
�� �� ֵ��  ��ӽ�� TRUE/FALSE
*******************************************************************************/
uint8_t FifoPutc(can_fifo_t *head, can_pkg_t data)
{
    ERRR(head == NULL, return FALSE);
    ERRR(IsFifoFull(head) == TRUE, return FALSE); //�ж϶������Ƿ��д洢�ռ�

    
    //head->data[head->rear] = data;
    memcpy(&head->data[head->rear], &data, sizeof(can_pkg_t));

    head->rear = (++head->rear)%head->size;

    return TRUE;   
}

/*******************************************************************************
�� �� ����  kfifo_getc
����˵����  ����һ���ַ�
��	  ����  *head:  ����ͷ
            data:  ��������
�� �� ֵ��  ���ӽ�� TRUE/FALSE
*******************************************************************************/
uint8_t FifoGetc(can_fifo_t *head, can_pkg_t *data)
{
    ERRR(head == NULL, return FALSE);
    ERRR(IsFifoEmpty(head) == TRUE, return FALSE); //����Ϊ��

    //*data = head->data[head->front];
    memcpy(data, &head->data[head->front], sizeof(can_pkg_t));
    head->front = (++head->front)%head->size;

    return TRUE;   
}



