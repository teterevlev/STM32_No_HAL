#include <stm32f411xe.h>
extern "C" void TIM1_UP_TIM10_IRQHandler(void){
	extern uint32_t events;
	events ++;
  if (TIM1->SR & TIM_SR_UIF)
  {
    TIM1->SR &= ~TIM_SR_UIF;


  }
}
