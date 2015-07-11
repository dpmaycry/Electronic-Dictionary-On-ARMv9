#ifndef __IIC_8LED_H__
#define __IIC_8LED_H__

void __irq iic_int_8led(void) ;
void iic_init_8led(void);
void iic_write_8led(UINT32T unSlaveAddr, UINT32T unAddr, UINT8T ucData);
void iic_read_8led(UINT32T unSlaveAddr, UINT32T unAddr, UINT8T *pData);

#endif
