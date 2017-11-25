//
//  iTempestSoundIO.cpp
//  iTempest
//
//  Created by Randy Rasmussen on 11/18/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

//#include <AVFoundation/AVAudioSession.h>
#include "iosException.h"
#include "iTempestSoundIO.h"

const int BUFFER_SIZE = 2048;

iTempestSoundIO::iTempestSoundIO(void)
{
    //[AVAudioSession.sharedInstance setPreferredIOBufferDuration:NSInterval(0.01) error:nullptr];

    // initialize our audio queue
    AudioStreamBasicDescription info;
    info.mBitsPerChannel = 16;
    info.mBytesPerFrame = 4;
    info.mChannelsPerFrame = 2;
    info.mFormatID = kAudioFormatLinearPCM;
    info.mFormatFlags =
        kAudioFormatFlagsNativeEndian |
        kAudioFormatFlagIsSignedInteger |
        kLinearPCMFormatFlagIsPacked;
    info.mBytesPerPacket = 4;
    info.mFramesPerPacket = 1;
    info.mReserved = 0;
    info.mSampleRate = 44100;

    OSStatus result = AudioQueueNewOutput(
            &info,
            &AudioOutputCallback,
            this,
            nullptr,
            nullptr,
            0,
            &audioQueue
            );
    if (result != noErr)
        throw iosException("AudioQueueNewOutput", result);
    
    // enqueue our buffers
    EnqueueBuffer(nullptr);
    EnqueueBuffer(nullptr);

    // start the audio queue
    AudioQueueStart(audioQueue, nullptr);

    // start our wave streamer that calls us periodically with new sound
    // to shove into the buffers
    waveStreamer = new Cpp11WaveStreamer(BUFFER_SIZE * 2, this);
}

iTempestSoundIO::~iTempestSoundIO(void)
{
    // stop our wave streamer so that it won't call us while we are
    // disposing things
    if (waveStreamer != nullptr)
    {
        delete waveStreamer;
        waveStreamer = nullptr;
    }
    
    // stop the queue
    AudioQueueStop(audioQueue, true);
    
    // dispose the buffers
    for (int i=0; i<bufferList.size(); ++i)
        AudioQueueFreeBuffer(audioQueue, bufferList[i]);
    
    // dispose the audio queue
    AudioQueueDispose(audioQueue, true);
}

void iTempestSoundIO::SetPlayer1ButtonState(bool state)
{
    if (state)
        buttons |= ONE_PLAYER_BUTTON;
    else
        buttons &= ~ONE_PLAYER_BUTTON;
}

void iTempestSoundIO::SetFireButtonState(bool state)
{
    if (state)
        buttons |= FIRE_BUTTON;
    else
        buttons &= ~FIRE_BUTTON;
}

void iTempestSoundIO::SetZapButtonState(bool state)
{
    if (state)
        buttons |= ZAPPER_BUTTON;
    else
        buttons &= ~ZAPPER_BUTTON;
}

uint8_t iTempestSoundIO::GetEncoder(void)
{
    return encoder;
}

void iTempestSoundIO::MoveSpinner(int offset)
{
    encoder += offset;
}

void iTempestSoundIO::SetSoundChannelState(int channel, SoundChannelState state)
{
    waveStreamer->SetChannelState(channel, state);
}

void iTempestSoundIO::SetTime(uint64_t clockCycles)
{
    int elapsed = (int)(clockCycles - currentCPUTime);
    waveStreamer->Delay(elapsed);
    currentCPUTime = clockCycles;
}


void iTempestSoundIO::AudioOutputCallback(void *inUserData, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer)
{
    iTempestSoundIO *pThis = (iTempestSoundIO *)inUserData;
    
    std::lock_guard<std::mutex> lock(pThis->bufferQueueMutex);
    pThis->bufferQueue.push_back(inBuffer);
}


void iTempestSoundIO::FillNextBuffer(WaveSoundSource *streamer)
{
    // pull a buffer out of the queue
    AudioQueueBufferRef buffer = nullptr;
    {
        std::lock_guard<std::mutex> lock(bufferQueueMutex);
        if (bufferQueue.size() == 0)
            return;
        buffer = bufferQueue[0];
        bufferQueue.erase(bufferQueue.begin());
    }

    // tempest generates mono, and we generate stereo so just fill half way
    int16_t *data = (int16_t *)buffer->mAudioData;
    int sampleCount = buffer->mAudioDataBytesCapacity/4;
    streamer->FillBuffer(data, sampleCount);
    
    // and turn them into stereo
    for (int i=sampleCount-1; i>=0; --i)
        data[i * 2 + 1] = data[i*2] = data[i];

    // enqueue it
    buffer->mAudioDataByteSize = sampleCount * 4;
    EnqueueBuffer(buffer);
}

void iTempestSoundIO::EnqueueBuffer(AudioQueueBufferRef buffer)
{
    OSStatus status;
    
    // if the buffer parameter is null this is a request to create a
    // new buffer and enqueue it
    if (buffer == nullptr)
    {
        status = AudioQueueAllocateBuffer(
            audioQueue,
            1000 * 16,
            &buffer);
        if (status == noErr)
            bufferList.push_back(buffer);
    }
    
    buffer->mAudioDataByteSize = buffer->mAudioDataBytesCapacity;
    status = AudioQueueEnqueueBuffer(
                   audioQueue,
                   buffer,
                   0,
                   0
                   );
}

