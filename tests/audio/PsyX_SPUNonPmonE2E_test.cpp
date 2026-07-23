#include "audio/PsyX_SPUAL_ext.h"
#include "PsyX/PsyX_audio.h"
#include "psx/libspu.h"

#include <SDL.h>
#include <cstdio>

#define CHECK(condition) \
	do { \
		if (!(condition)) { \
			std::fprintf(stderr, "check failed at line %d: %s\n", \
				__LINE__, #condition); \
			SpuQuit(); \
			return 1; \
		} \
	} while (0)

#ifdef _WIN32
extern "C" int SDL_main(int, char**)
#else
int main()
#endif
{
	SDL_setenv("SDL_AUDIODRIVER", "dummy", 1);
	PsyX_SPUAL_ConfigureOutput(
		PSYX_AUDIO_BACKEND_SDL,
		PSYX_AUDIO_MODE_SHARED,
		44100,
		0);
	CHECK(PsyX_SPUAL_ConfigureRenderer(1, 0, 0, 0) == 1);

	SpuInit();
	PsyXAudioStatus status{};
	PsyX_AudioGetStatus(&status);
	CHECK(status.state == PSYX_AUDIO_STATE_RUNNING);
	CHECK(status.active_backend == PSYX_AUDIO_BACKEND_SDL);
	CHECK(status.active_rate == 44100);

	CHECK(SpuSetNoiseClock(-1) == 0);
	CHECK(SpuSetNoiseClock(17) == 17);
	CHECK(SpuSetNoiseClock(0x40) == 0x3F);

	const u_int noiseA = SPU_VOICECH(2);
	const u_int noiseB = SPU_VOICECH(5);
	CHECK(SpuSetNoiseVoice(SPU_ON, noiseA) == noiseA);
	CHECK(SpuSetNoiseVoice(SPU_ON, noiseB) == (noiseA | noiseB));
	CHECK(SpuSetNoiseVoice(SPU_OFF, noiseA) == noiseB);
	CHECK(SpuSetNoiseVoice(SPU_OFF, noiseB) == 0);

	const u_int pmonA = SPU_VOICECH(3);
	const u_int pmonB = SPU_VOICECH(7);
	CHECK(SpuSetPitchLFOVoice(SPU_ON, pmonA) == pmonA);
	CHECK(SpuSetPitchLFOVoice(SPU_ON, pmonB) == (pmonA | pmonB));
	CHECK(SpuSetPitchLFOVoice(SPU_OFF, pmonA) == pmonB);
	CHECK(SpuSetPitchLFOVoice(SPU_OFF, pmonB) == 0);

	SpuQuit();
	return 0;
}

#undef CHECK
