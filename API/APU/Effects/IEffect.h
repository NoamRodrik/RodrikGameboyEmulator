/**
 * @file   IEffect.h
 * @author Noam Rodrik
 * @description Audio effect interface
 */
#ifndef __API_IEFFECT_H__
#define __API_IEFFECT_H__

namespace API
{
class [[nodiscard]] IEffect
{
public:
	virtual ~IEffect() = default;

public:
	[[nodiscard]] virtual bool Play() = 0;
};
} // API

#endif // __API_IEFFECT_H__