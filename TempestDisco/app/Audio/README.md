# Tempest Audio Board Audio Driver

This directory contains the low level audio driver for the Tempest sound board.  The actual
waveform generation code is in the platform independent [TempestIO](https://github.com/notjulie/Tempest/tree/master/TempestIO) directory; this is the
hardware-specific portion of the driver.  The modules in this directory are:

* *AudioDriver*
    * Does buffer management of the double buffers

* *AudioLEDs*
   * Handles updating the diagnostic LEDs on the board
    
* *DiscoWaveStreamer*
   * Class derived from the WaveStreamer waveform generator class... watches for when AudioDriver
     reports a buffer is ready to be filled, and fills it.  Also responsible for polling the
     waveform generator base class.

* *TempestDiscoAudioIO*
   * Class that implements the standard interfaces that our serial port listener expects...
     basically just glues the sound & control panel implementation to the serial port.

* *stm32f4_discovery_audio_codec*
   * Sample implementation from the STMicro examples... basically controls double-buffered
     wave output using the I2S codec.
    
