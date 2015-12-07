
#include <oal/oal_thread.h>

extern "C"
{
	#include <stm32f4xx.h>

	// sleep ms
	void HAL_Delay(__IO uint32_t Delay)
	{
		OAL::Thread::Sleep((unsigned long)Delay*10);
	}

    uint32_t HAL_GetTick(void)
    {
    	return OAL::Thread::GetTicks();
    }

}
