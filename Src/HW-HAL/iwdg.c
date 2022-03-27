#include "iwdg.h"
#include "hw.h"
IWDG_HandleTypeDef hiwdg;


/*******************************************************
*Function Name 	:MX_IWDG_Init
*Description  	:���Ź���ʼ��
*Input					:
*Output					:
*���Ź����ʱ����㣺t=(256*(3749+1))/32k=30s
*******************************************************/



void MX_IWDG_Init(void)
{

    hiwdg.Instance = IWDG;
    hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
    hiwdg.Init.Window = IWDG_WINDOW_DISABLE;
    hiwdg.Init.Reload = (3749);//30s
    if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
    {
        Error_Handler();
    }

}
