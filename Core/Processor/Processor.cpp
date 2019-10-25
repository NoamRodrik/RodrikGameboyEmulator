/**
 * @file		Processor.cpp
 * @author		Noam Rodrik
 * @description Main logic of processor
 */
#include <Core/Processor/Processor.h>

namespace Core
{
Processor& Processor::GetInstance()
{
	if (Processor::m_instance == nullptr)
	{
		Processor::m_instance = gsl::not_null<Processor*>(new Processor{});
	}

	return *gsl::not_null<Processor*>(Processor::m_instance);
}
} // Core