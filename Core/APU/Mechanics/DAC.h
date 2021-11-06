/**
 * @file		DAC.h
 * @author		Noam Rodrik
 * @description Definition of the DAC header.
 */
#ifndef __CORE_DAC_H__
#define __CORE_DAC_H__

#include <API/StaticInstance.h>
#include <API/Definitions.h>
#include <API/RingBuffer.h>
#include <portaudio.h>

namespace Core
{
class [[nodiscard]] DAC : public API::StaticInstance<DAC>
{
public:
	struct [[nodiscard]] SampleData
    {
        uint8_t SoundChannel1Left[API::BUFFER_FRAMES]{};
        uint8_t SoundChannel1Right[API::BUFFER_FRAMES]{};
        uint8_t SoundChannel2Left[API::BUFFER_FRAMES]{};
        uint8_t SoundChannel2Right[API::BUFFER_FRAMES]{};
        uint8_t SoundChannel3Left[API::BUFFER_FRAMES]{};
        uint8_t SoundChannel3Right[API::BUFFER_FRAMES]{};
        uint8_t SoundChannelNoiseLeft[API::BUFFER_FRAMES]{};
        uint8_t SoundChannelNoiseRight[API::BUFFER_FRAMES]{};
    };

public:
	DAC();

public:
    void FeedSamples(const SampleData& samples);

private:
    void Populate(gsl::not_null<float*> buffer);

private:
	std::array<API::RingBuffer<float_t, 1024 * 4>, API::OUTPUT_TERMINALS_AMOUNT> _sample_buffer_per_terminal{};
	PaStream*														             _stream{nullptr};
};
} // Core

#endif // __CORE_DAC_H__