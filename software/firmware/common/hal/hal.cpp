
#include <oal/oal_thread.h>

extern "C"
{
	#include <stm32f4xx.h>

	void HAL_Delay(__IO uint32_t Delay)
	{
		OAL::Thread::Sleep((unsigned long)Delay);
	}

    uint32_t HAL_GetTick(void)
    {
    	return OAL::Thread::GetTicks();
    }

}
