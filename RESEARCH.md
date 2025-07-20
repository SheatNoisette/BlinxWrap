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

## Miles Sound System 6.1c

No public documentation available.


Imported Miles Sound System functions from Bink, resolved with GetProcAddress.

French Version 1.0.1:
- _AIL_minimum_sample_buffer_size@12
- _AIL_allocate_sample_handle@4
- _AIL_init_sample@4
- _AIL_stop_sample@4
- _AIL_resume_sample@4
- _AIL_set_sample_type@12
- _AIL_set_sample_playback_rate@8
- _AIL_set_sample_user_data@12
- _AIL_set_sample_volume@8
- _AIL_set_sample_pan@8
- _AIL_serve@0
- _AIL_sample_buffer_ready@4
- _AIL_load_sample_buffer@16
- _AIL_end_sample@4
- _AIL_release_sample_handle@4
- _AIL_sample_status@4
- _AIL_sample_buffer_info@20
- _AIL_mem_alloc_lock@4
- _AIL_mem_free_lock@4
- _AIL_get_preference@4
- _AIL_set_sample_user_data@12
- _AIL_sample_user_data@8
- _AIL_register_EOB_callback@8
- _AIL_sample_buffer_info@20
- _AIL_digital_latency@4

Random Found Online Bink (Require MSS 7):
- _AIL_sample_buffer_info@24
- _AIL_allocate_sample_handle@4
- _AIL_digital_latency@4
- _AIL_init_sample@8
- _AIL_load_sample_buffer@16
- _AIL_mem_alloc_lock_info@12
- _AIL_mem_free_lock@4
- _AIL_minimum_sample_buffer_size@12
- _AIL_register_EOB_callback@8
- _AIL_release_sample_handle@4
- _AIL_resume_sample@4
- _AIL_sample_buffer_available@4
- _AIL_sample_user_data@8
- _AIL_set_sample_buffer_count@8
- _AIL_set_sample_channel_levels@20
- _AIL_set_sample_playback_rate@8
- _AIL_set_sample_user_data@12
- _AIL_set_sample_volume_pan@12
- _AIL_stop_sample@4

### Approximate functions calls used in Bink 

```c
int AIL_minimum_sample_buffer_size(Sample *s, int unknown_1, int unknown_2); 
Sample *__stdcall AIL_allocate_sample_handle(void *unknown_1);
void __stdcall AIL_init_sample(Sample *s);
void __stdcall AIL_stop_sample(Sample *s);
void __stdcall AIL_resume_sample(Sample *s);
void __stdcall AIL_set_sample_type(Sample *s, int unknown_1, int unknown_2);
void __stdcall AIL_set_sample_playback_rate(Sample *s, int rate);
void __stdcall AIL_set_sample_user_data(Sample *s, unsigned int index, int value);
void __stdcall AIL_set_sample_volume(Sample *s, int volume); // May be between 0 and 2^16-1
void __stdcall AIL_set_sample_pan(Sample *s, int pan); // May be between 0 and 2^16-1
void __stdcall AIL_serve();
int __stdcall AIL_sample_buffer_ready(Sample *s);
void __stdcall AIL_load_sample_buffer(Sample *s, int unknown_1, void* buffer, unsigned int length);
void __stdcall AIL_end_sample(Sample *s);
void __stdcall AIL_release_sample_handle(Sample *s);
int __stdcall AIL_sample_status(Sample *s);
int __stdcall AIL_sample_buffer_info(Sample *s, int unknown_1, int unknown_2, int unknown_3, int unknown_4);
void __stdcall *AIL_mem_alloc_lock(unsigned int size);
void __stdcall AIL_mem_free_lock(void *ptr);
int __stdcall AIL_get_preference(int data); // Data ?
void __stdcall AIL_set_sample_user_data(Sample *s, int index, int value);
int __stdcall AIL_sample_user_data(Sample *s, int index);
void *__stdcall AIL_register_EOB_callback(Sample *s, void *callback);
int __stdcall AIL_digital_latency(void *driver); // May be some sort of driver
```
