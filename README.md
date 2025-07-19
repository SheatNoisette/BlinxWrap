# BlinxWrap
An Experimental Bink Wrapper for ZanZarah: The Hidden Portal, Steam Version.

**THIS WRAPPER TRIES TO IMPLEMENT THE SUBSET OF BINK USED BY ZanZarah**

This library is **PARTIALLY** ABI compatible with Bink Codec and implements the
following functions:
```
BinkOpen()
BinkClose()
BinkWait()
BinkDoFrame()
BinkNextFrame()
BinkSetFrameRate()
BinkCopyToBuffer()
BinkDDSurfaceType()
BinkSetSoundSystem()
BinkOpenMiles()
BinkSetVolume()
```

## Milestones

> STATUS: Can play Bink Videos / other containers without sound !

- [X] Be ABI compatible to be loaded by the game without any issue
- [X] Be able to write to the DirectDraw7 surface provided by the game
- [X] Display a frame from a new video format (Such as h264/mp4)
    - [X] Integrate FFMPEG
    - [X] Load a file and detect the codec used
    - [X] Update the Bink Structure
    - [X] Copy the first frame to the Direct Draw buffer
- [X] Play an unsynced video from a new container
- [X] Play a proprely sync video from a new container
- [ ] Load options from a config file
- [ ] Hook into Miles Sound System 6.1c to play audio
    - [ ] Reverse and Document MSS Sample format
    - [ ] Play a basic sin wave from MSS
    - [ ] Basic unsync audio playback
    - [ ] Play audio proprely from MSS
- [ ] Patch the game in memory to be able to resize Direct Draw surface

The following milestones would be optional, when all of the primary milestones
are met, the project would be technically considered complete:
- [ ] Be able to detect proprely detect any DirectDraw surface
- [ ] Implement alternative audio backend (DirectSound, Xaudio)
- [ ] 64 bits support for newer games
- [ ] Handle newer or older Miles Sound System, such as <= 5 / 7 / 8

## Building

To build BlinxWrap, you need:
- Visual Studio 2022
- Windows 10 / 11 SDK
- Precompiled FFMPEG library https://github.com/System233/ffmpeg-msvc-prebuilt

## License

Bink Codec(c) and Miles Sound System(c) are trademarks from RAD Game Tools/Epic
Game Tools, all rights reserved.

BlinxWrap is licensed under the Mozilla Public License 2.0. See `LICENSE` for
more details.
