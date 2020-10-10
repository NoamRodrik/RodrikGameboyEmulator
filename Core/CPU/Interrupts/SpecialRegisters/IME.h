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
class [[nodiscard]] IME : API::StaticInstance<IME>
{
public:
	static inline void EnableInterrupts() { IME::GetInstance()._enabled = true; }
	static inline void DisableInterrupts() { IME::GetInstance()._enabled = false; }
	static inline bool IsEnabled() { return IME::GetInstance()._enabled; }

	static inline void Schedule() { IME::GetInstance()._schedule = true; IME::GetInstance()._cycles_wait = 1; }
	static inline void EnableInterruptsIfScheduled()
	{
		if (IME::GetInstance()._schedule)
		{
			if (IME::GetInstance()._cycles_wait > 0)
			{
				IME::GetInstance()._cycles_wait -= 1;
			}
			else
			{
				IME::GetInstance()._schedule = false;
				IME::EnableInterrupts();
			}
		}
	}

private:
	// By default, the IME is disabled.
	bool   _enabled{false};
	bool   _schedule{false};
	size_t _cycles_wait{0};
};
}

#endif // __LR35902_IME_H__