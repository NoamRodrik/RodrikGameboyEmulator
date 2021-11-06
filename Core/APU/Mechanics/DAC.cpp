/**
 * @file   DAC.cpp
 * @author Noam Rodrik
 * @description The implementation for the DAC (Digital-Analog-Converter)
 */
#include <Core/APU/Mechanics/DAC.h>
#include <Core/APU/Definitions.h>
#include <portaudio.h>

using namespace API;

namespace Core
{
DAC::DAC()
{
#if !defined(_DEBUG) || _DEBUG 0
    const auto wave_callback = [](const void* input, void* output, unsigned long frames,
                           const PaStreamCallbackTimeInfo* time_info, PaStreamCallbackFlags status_flags, void* user_data)
    {
        DAC::GetInstance().Populate(reinterpret_cast<float*>(output));
        return 0;
    };

    Pa_Initialize();
    SANITY(Pa_OpenDefaultStream(&this->_stream,
                                0,
                                API::OUTPUT_TERMINALS_AMOUNT,
                                paFloat32,
                                API::SAMPLE_RATE,
                                API::BUFFER_FRAMES,
                                wave_callback,
                                nullptr) == PaErrorCode::paNoError, "Failed opening default stream for PortAudio");
#endif
}

void DAC::FeedSamples(const SampleData& samples)
{
#if !defined(_DEBUG) || _DEBUG 0
    for (std::size_t current_frame = 0; current_frame < API::BUFFER_FRAMES; ++current_frame)
    {
        Message("TODO! Make WaveController control this");
        const float_t LEFT_CHANNEL_ACCUMULATED =
            static_cast<float_t>(samples.SoundChannel1Left[current_frame] +
                samples.SoundChannel2Left[current_frame] +
                samples.SoundChannel3Left[current_frame] +
                samples.SoundChannelNoiseLeft[current_frame]) / 4;
        const float_t RIGHT_CHANNEL_ACCUMULATED =
            static_cast<float_t>(samples.SoundChannel1Right[current_frame] +
                samples.SoundChannel2Right[current_frame] +
                samples.SoundChannel3Right[current_frame] +
                samples.SoundChannelNoiseRight[current_frame]) / 4;
    
        this->_sample_buffer_per_terminal[static_cast<std::size_t>(OutputTerminal::SO1)].Push((RIGHT_CHANNEL_ACCUMULATED - 8) / 8);
        this->_sample_buffer_per_terminal[static_cast<std::size_t>(OutputTerminal::SO2)].Push((LEFT_CHANNEL_ACCUMULATED - 8) / 8);
    }
    
    // Wait with the start of the DAC until we filled the buffer a bit
    if (!Pa_IsStreamActive(this->_stream) &&
        this->_sample_buffer_per_terminal[static_cast<std::size_t>(OutputTerminal::SO1)].GetCapacity() >= API::BUFFER_FRAMES * 2)
    {
        SANITY(Pa_StartStream(this->_stream) == PaErrorCode::paNoError, "Failed starting stream");
    }
#endif
}

void DAC::Populate(gsl::not_null<float*> buffer)    
{
#if !defined(_DEBUG) || _DEBUG 0
    for (std::size_t current_frame = 0; current_frame < API::BUFFER_FRAMES * API::OUTPUT_TERMINALS_AMOUNT; current_frame += API::OUTPUT_TERMINALS_AMOUNT)
    {
        buffer.get()[current_frame] = this->_sample_buffer_per_terminal[static_cast<std::size_t>(OutputTerminal::SO2)].Pop();
        buffer.get()[current_frame + 1] = this->_sample_buffer_per_terminal[static_cast<std::size_t>(OutputTerminal::SO1)].Pop();
    }
#endif
}
} // Core