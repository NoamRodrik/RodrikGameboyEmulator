/**
 * @file		IME.h
 * @author		Noam Rodrik
 * @description LR35902 main ime definition header.
 */
#ifndef __LR35902_IME_H__
#define __LR35902_IME_H__

#include <Core/API/StaticInstance.h>

namespace Core
{
class IME : StaticInstance<IME>
{
public:
	static inline void EnableInterrupts() { IME::GetInstance().m_enabled = true; }
	static inline void DisableInterrupts() { IME::GetInstance().m_enabled = false; }
	static inline bool IsEnabled() { return IME::GetInstance().m_enabled; }

private:
	// By default, the IME is disabled.
	bool m_enabled{false};
};
}

#endif // __LR35902_IME_H__