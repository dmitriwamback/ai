#ifdef MI_APPLE_IMPLEMENTATION
    #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    #include <OpenAL/OpenAL.h>
#else
    #include <AL/al.h>
#endif
#include "sndfile.h"
#include "audiosource.h"

namespace Mi { namespace Audio {

    class AudioPlayer {
    private:
        std::vector<AudioSource> sources;

    public:

        ALCcontext* context;
        ALCdevice*  device;
        const char* ext_name;

        static AudioPlayer Create() {

            AudioPlayer a_player = AudioPlayer();

            a_player.device        = alcOpenDevice(nullptr);
            a_player.context       = alcCreateContext(a_player.device, nullptr);

            if (alcIsExtensionPresent(a_player.device, "ALC_ENUMERATE_ALL_EXT"))
                a_player.ext_name = alcGetString(a_player.device, ALC_ALL_DEVICES_SPECIFIER);

            if (!a_player.ext_name || alcGetError(a_player.device) != AL_NO_ERROR)
                a_player.ext_name = alcGetString(a_player.device, ALC_DEVICE_SPECIFIER);

            return a_player;
        }

        void AddSource(AudioSource source) {
            sources.push_back(source);
        }
    };
}}