/**
 * @file		IME.h
 * @author		Noam Rodrik
 * @description LR35902 main ime definition header.
 */
#ifndef __LR35902_IME_H__
#define __LR35902_IME_H__

#include <API/StaticInstance.h>

namespace Core
{
class IME : API::StaticInstance<IME>
{
public:
	static inline void EnableInterrupts() { IME::GetInstance().m_enabled = true; }
	static inline void DisableInterrupts() { IME::GetInstance().m_enabled = false; }
	static inline bool IsEnabled() { return IME::GetInstance().m_enabled; }

	static inline void Schedule() { IME::GetInstance().m_schedule = true; IME::GetInstance().m_cycles_wait = 1; }
	static inline void EnableInterruptsIfScheduled()
	{
		if (IME::GetInstance().m_schedule)
		{
			if (IME::GetInstance().m_cycles_wait > 0)
			{
				IME::GetInstance().m_cycles_wait -= 1;
			}
			else
			{
				IME::GetInstance().m_schedule = false;
				IME::EnableInterrupts();
			}
		}
	}

private:
	// By default, the IME is disabled.
	bool   m_enabled{false};
	bool   m_schedule{false};
	size_t m_cycles_wait{0};
};
}

#endif // __LR35902_IME_H__