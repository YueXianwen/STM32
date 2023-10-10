//
// Created by 36348 on 2023/9/14.
//

#ifndef TEST_ADC_H
#define TEST_ADC_H

void Adc_Init();
u16 Get_Adc(u8 ch);
u16 Get_Adc_Average(u8 ch, u8 times);

#endif //TEST_ADC_H
