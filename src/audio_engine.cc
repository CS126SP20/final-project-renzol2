// Copyright (c) 2020 [Your Name]. All rights reserved.

#include <covidaudio/audio_engine.h>

namespace covidaudio {

void AudioEngine::SetupParams() {}

void AudioEngine::MakeNote(const cinder::vec2& pos) {}

float AudioEngine::QuantizePitch(const cinder::vec2& pos) { return 0; }

void AudioEngine::HandleInstrumentsSelected() {}

void AudioEngine::HandleGeneratorSelected() {}

void AudioEngine::HandleEffectSelected() {}

void AudioEngine::HandleInstrumentSpecificNote(const cinder::vec2& pos) {}

void AudioEngine::PrintAudioGraph() {}

}  // namespace covidaudio
