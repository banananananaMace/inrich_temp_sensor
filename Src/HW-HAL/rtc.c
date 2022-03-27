#include "rtc.h"
RTC_HandleTypeDef RTCHandle;
void RTC_Config(void)
{
    RTCHandle.Instance = RTC;
    RTCHandle.Init.HourFormat = RTC_HOURFORMAT_24;
    RTCHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
    RTCHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
    RTCHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
    RTCHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    RTCHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
    if(HAL_RTC_Init(&RTCHandle) != HAL_OK)
    {
        Error_Handler();
    }
}
void Handle_WakeUpSource(void)
{
    if(READ_REG(RTC->BKP31R) == 0)//�ո��ϵ�����
    {
        Send_Frame_Type = 1;//���ϵ緢������
        HAL_PWR_EnableBkUpAccess();//ʹ�ܺ�������ʣ�
        HAL_RTCEx_BKUPWrite(&RTCHandle,RTC_BKP_DR0,0);
        HAL_RTCEx_BKUPWrite(&RTCHandle,RTC_BKP_DR5,0);
    }
    else if (READ_REG(RTC->BKP31R) == 1)//�ӵ͹���ģʽ�л���
    {
        WRITE_REG( RTC->BKP31R, 0x0 ); //RTC->BKP31R���ֵ������¼�Ƿ����͹���
        HAL_PWR_EnableBkUpAccess();
        Message_Cycle_Count_down = HAL_RTCEx_BKUPRead(&RTCHandle,RTC_BKP_DR1);
        if(Message_Cycle_Count_down==0)
        {
            WakeUpNumber = HAL_RTCEx_BKUPRead(&RTCHandle,RTC_BKP_DR0);
            WakeUpNumber++;
            HAL_RTCEx_BKUPWrite(&RTCHandle,RTC_BKP_DR0,WakeUpNumber);
            //�����ޱ���
            if(Message_cycle>30*60*1000)//MESSAGE����   30���ӣ�����ֵ����
            {
                Message_cycle=(30*60*1000);
            }

            if (WakeUpNumber%(Message_cycle/WakeUp_TimeBasis) == 0 )//ÿ���ӷ���һ��Message
            {
                Send_Frame_Type = 1;
            }
            if (WakeUpNumber%((Ctrl_cycle*Message_cycle)/WakeUp_TimeBasis) == 0 ) //����ӷ�һ��req
            {
                Send_Frame_Type = 2;
                HAL_RTCEx_BKUPWrite(&RTCHandle,RTC_BKP_DR0,0);
            }

        }
        else
        {
            Message_Cycle_Count_down--;
            HAL_RTCEx_BKUPWrite(&RTCHandle,RTC_BKP_DR1,Message_Cycle_Count_down);//��¼ƫ��ʱ�����������
        }
    }
}



