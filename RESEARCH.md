# Bink Wrapper

Experimental Bink replacement for Zanzarah: The Hidden Portal game (2001).

## ZanZarah
This research is focusing on the Steam Version of the Zanzarah game.
`zanthp.exe - SHA1: 45c81e3245ec98e776872b8cabbb7129cd17fe96`.

The game is written in C++.

The following functions are imported from the Bink Codec (Mangled):
```
_BinkClose@4
_BinkCopyToBuffer@28
_BinkDDSurfaceType@4
_BinkDoFrame@4
_BinkNextFrame@4
_BinkOpen@8
_BinkOpenMiles@4
_BinkSetFrameRate@8
_BinkSetSoundSystem@8
_BinkSetVolume@12
_BinkWait@4
```

The video size is hardcoded to 640x480, the framerate is handled by Bink.

### BinkVideoPlayback

`BinkVideoPlayback` is the name of the class given to the part handling the Bink Video.
This is not the official name of the class, more a deduction.

Possible class members:
```
struct BinkVideoPlayback __packed
{
    struct BinkVideoPlayback* `this`;
    struct BinkHandle* bink_handle;
    int32_t width;
    int32_t height;
    int32_t total_frames;
    char* video_name;
    char* field_18;
    int32_t field_1c;
    int32_t field_20;
    __padding char _24[0x1c];
    __padding char _40[0x14];
    struct LPDIRECTDRAWSURFACE* directdraw_surface;
    int32_t* field_58;
    int32_t current_frame;
    bool is_finished;
    __padding char _61[0x1f];
    __padding char _80[0x40];
    __padding char _c0[0x40];
    __padding char _100[0x24];
    float field_124;
    int32_t field_128;
    int32_t field_12c;
    int32_t field_130;
    int32_t field_134;
    __padding char _138[8];
    __padding char _140[4];
    int32_t field_144;
    __padding char _148[8];
    char field_150;
    __padding char _151[3];
    float field_154;
};

struct BinkHandle __packed
{
    uint32_t width;
    uint32_t height;
    uint32_t frames;
};

```

### Open a Video (0x00564bbe)

There's a reconstructed pseudocode of how the game loads a video:
```c++

Binkhandle *BinkVideoPlayback::open_video(char *video_name) {
  if (this->bink_handle == nullptr)
    return nullptr;

  this->

}

```

### Play Video (0x00564d35)

There's a reconstructed pseudocode of how the game plays a video:
```c+++

```
