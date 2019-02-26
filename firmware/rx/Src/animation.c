#include "animation.h"
#include "shared.h"

TIM_HandleTypeDef* frame_timer;
TIM_HandleTypeDef* pwm_timer;
uint32_t frame_count, ani_start, curr_frame;
uint8_t current_animation;

const static uint8_t breathing_lookup[51] = {0, 0, 0, 0, 1, 3, 6, 9, 13, 18, 25, 32, 41, 52, 64, 78, 94, 111, 129, 148, 168, 187, 206, 222, 236, 246, 252, 253, 250, 243, 231, 217, 200, 181, 161, 142, 123, 105, 88, 73, 60, 48, 38, 30, 22, 16, 12, 8, 5, 2, 1};
const static uint8_t doubleflash_lookup[16] = {255, 255, 255, 255, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0};
void animation_init(TIM_HandleTypeDef* frame_tim, TIM_HandleTypeDef* pwm_tim)
{
  frame_timer = frame_tim;
  pwm_timer = pwm_tim;

  HAL_TIM_Base_Start(pwm_timer);
  HAL_TIM_PWM_Init(pwm_timer);
  HAL_TIM_PWM_Start(pwm_timer, TIM_CHANNEL_1);

  HAL_TIM_Base_Start_IT(frame_timer);
  current_animation = ANIMATION_TYPE_CONST_OFF;
}

void animation_update(void)
{
  frame_count++;
  curr_frame = frame_count - ani_start;

  if(current_animation == ANIMATION_TYPE_CONST_ON)
  {
      pwm_timer->Instance->CCR1 = 255;
  }
  else if(current_animation == ANIMATION_TYPE_BREATHING)
  {
    pwm_timer->Instance->CCR1 = breathing_lookup[curr_frame % 51];
  }
  else if(current_animation == ANIMATION_TYPE_DOUBLEFLASH && curr_frame < 16)
  {
    pwm_timer->Instance->CCR1 = doubleflash_lookup[curr_frame];
  }
  else
  {
    pwm_timer->Instance->CCR1 = 0;
  }
}

void start_animation(uint8_t ani_type)
{
  current_animation = ani_type;
  ani_start = frame_count;
}


