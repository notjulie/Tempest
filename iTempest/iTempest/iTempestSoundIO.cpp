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
    :
        waveStreamer(BUFFER_SIZE * 2, this)
{
    //[AVAudioSession.sharedInstance setPreferredIOBufferDuration:NSInterval(0.01) error:nullptr];
    
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
    EnqueueBuffer(nullptr);
    EnqueueBuffer(nullptr);
    
    AudioQueueStart(audioQueue, nullptr);
}


void iTempestSoundIO::SetPlayer1ButtonState(bool state)
{
    if (state)
        buttons |= ONE_PLAYER_BUTTON;
    else
        buttons &= ~ONE_PLAYER_BUTTON;
}


void iTempestSoundIO::AudioOutputCallback(void *inUserData, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer)
{
    iTempestSoundIO *pThis = (iTempestSoundIO *)inUserData;
    
    std::lock_guard<std::mutex> lock(pThis->mutex);
    pThis->bufferQueue.push_back(inBuffer);
}


void iTempestSoundIO::FillNextBuffer(WaveSoundSource *streamer)
{
    // pull a buffer out of the queue
    AudioQueueBufferRef buffer = nullptr;
    {
        std::lock_guard<std::mutex> lock(mutex);
        if (bufferQueue.size() == 0)
            return;
        buffer = bufferQueue[0];
        bufferQueue.erase(bufferQueue.begin());
    }

    // enqueue it
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
    }
    
    // square wave for now
    int16_t *data = (int16_t *)buffer->mAudioData;
    for (int i=0; i<buffer->mAudioDataBytesCapacity/2; ++i)
    {
        if (i & 32)
            data[i] = 0x8000;
        else
            data[i] = 0x7FFF;
    }
    buffer->mAudioDataByteSize = buffer->mAudioDataBytesCapacity;
    
    status = AudioQueueEnqueueBuffer(
                   audioQueue,
                   buffer,
                   0,
                   0
                   );
}

