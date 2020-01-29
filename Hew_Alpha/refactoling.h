#pragma once
#include "main.h"
#include <mmsystem.h>
#include <xaudio2.h>
#include <deque>
#include "directx.h"
#pragma region library ...
#pragma comment(lib,"winmm.lib")
#pragma endregion
#pragma region macro ...
constexpr auto SOUND_MAX = 10;
constexpr auto BUFFERQUEUE_MAX = 4;
constexpr auto BUFFERQUEUE_ALLOC = (BUFFERQUEUE_MAX + 1);
#pragma endregion
//=============================================================//
//	XAUDIO CLASS												//
//=============================================================//
class XAUDIO2 :public SINGLETON<XAUDIO2> {

	struct VOICE_DATA {
		IXAudio2SourceVoice*	lpSourceVoice = nullptr;

		HMMIO			hmmio = nullptr;
		HANDLE			hEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		WAVEFORMATEX	wfx = {};
		uint8_t*	buffAudio = nullptr;
		uint8_t*	startAudio = nullptr;
		DWORD32		bufflength = 0;
		uint32_t	audioBytes = 0;
		uint32_t	callcount = 0;
		uint32_t	loopStart = 0;
		uint32_t	loopLength = 0;
	};
	class  SOUNDCALLBACK :public IXAudio2VoiceCallback {
	public:
		HANDLE hEvent;

		void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 BytesRequired) {}
		void STDMETHODCALLTYPE OnVoiceProcessingPassEnd(void) {}
		void STDMETHODCALLTYPE OnStreamEnd(void) { SetEvent(hEvent); }
		void STDMETHODCALLTYPE OnBufferStart(void *pBufferContext) { SetEvent(hEvent); }
		void STDMETHODCALLTYPE OnBufferEnd(void *pBufferContext) {}
		void STDMETHODCALLTYPE OnLoopEnd(void *pBufferContext) {}
		void STDMETHODCALLTYPE OnVoiceError(void *pBufferContext, HRESULT Error) {}

		SOUNDCALLBACK() {};
		~SOUNDCALLBACK() {};
	};

	IXAudio2*				lpXAudio = nullptr;
	IXAudio2MasteringVoice*	lpMastaringVoice = nullptr;
	IXAudio2SubmixVoice*	lpSubmixVoice = nullptr;
	XAUDIO2_BUFFER			buffinfo = {};
	XAUDIO2_VOICE_STATE		state = {};
	SOUNDCALLBACK			callback;

	using	sound_list = std::vector<VOICE_DATA>;
	sound_list				snd_dat;
	sound_list::iterator	snd_itr = snd_dat.begin();

	HRESULT Initialize() {
		CoInitializeEx(nullptr, COINIT_MULTITHREADED);

		if (FAILED(XAudio2Create(&lpXAudio, 0, XAUDIO2_DEFAULT_PROCESSOR))) {
			return E_FAIL;
		}
		if (FAILED(lpXAudio->CreateMasteringVoice(
			&lpMastaringVoice,
			XAUDIO2_DEFAULT_CHANNELS,
			XAUDIO2_DEFAULT_SAMPLERATE,
			0, 0, nullptr))) {

			return E_FAIL;
		}
		return S_OK;
	}
	HRESULT Finalize() {
		if (snd_dat.size() > 0) {
			for (snd_itr = snd_dat.begin(); snd_itr != snd_dat.end(); ++snd_itr) {
				Release();
			}
			snd_dat.clear();
		}
		if (lpSubmixVoice != nullptr) {
			lpSubmixVoice->DestroyVoice();
			lpSubmixVoice = nullptr;
		}
		if (lpMastaringVoice != nullptr) {
			lpMastaringVoice->DestroyVoice();
			lpMastaringVoice = nullptr;
		}
		if (callback.hEvent != nullptr) {
			callback.hEvent = nullptr;
		}
		if (lpXAudio != nullptr) {
			lpXAudio->Release();
			lpXAudio = nullptr;
		}

		CoUninitialize();
		return S_OK;
	}

	HRESULT ReadWave(LPCTSTR lpFilename) {
		MMCKINFO		mmckinfo;
		PCMWAVEFORMAT	pcmwf;
		MMRESULT		mmret;
		MMIOINFO		mmioinfo;
		MMCKINFO		riffchunkinfo;
		MMCKINFO		datachunkinfo;

		memset(&mmioinfo, 0x00, sizeof(mmioinfo));

		snd_itr->hmmio = mmioOpen(const_cast<LPSTR>(lpFilename),
			&mmioinfo,
			MMIO_READ);

		if (snd_itr->hmmio == nullptr) {
			return E_FAIL;
		}

		memset(&riffchunkinfo, 0x00, sizeof(riffchunkinfo));
		riffchunkinfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');

		mmret = mmioDescend(snd_itr->hmmio, &riffchunkinfo, nullptr, MMIO_FINDRIFF);
		if (mmret != MMSYSERR_NOERROR) {
			return E_FAIL;
		}

		memset(&mmckinfo, 0x00, sizeof(mmckinfo));
		mmckinfo.ckid = mmioFOURCC('f', 'm', 't', ' ');

		mmret = mmioDescend(snd_itr->hmmio, &mmckinfo, &riffchunkinfo, MMIO_FINDCHUNK);
		if (mmret != MMSYSERR_NOERROR) {
			return E_FAIL;
		}

		if (mmckinfo.cksize >= sizeof(WAVEFORMATEX)) {
			mmioRead(snd_itr->hmmio, (HPSTR)&snd_itr->wfx, sizeof(snd_itr->wfx));
		}
		else {
			mmioRead(snd_itr->hmmio, (HPSTR)&pcmwf, sizeof(pcmwf));

			memset(&snd_itr->wfx, 0x00, sizeof(snd_itr->wfx));
			memcpy(&snd_itr->wfx, &pcmwf, sizeof(pcmwf));
		}

		mmioAscend(snd_itr->hmmio, &mmckinfo, 0);

		memset(&datachunkinfo, 0x00, sizeof(datachunkinfo));
		datachunkinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');

		mmret = mmioDescend(snd_itr->hmmio, &datachunkinfo, &riffchunkinfo, MMIO_FINDCHUNK);
		if (mmret != MMSYSERR_NOERROR) {
			return E_FAIL;
		}
		return S_OK;
	}
	HRESULT CreateBuff(LPCTSTR lpFilename) {
		snd_dat.emplace_back();
		snd_itr = --snd_dat.end();

		ReadWave(lpFilename);

		if (FAILED(lpXAudio->CreateSourceVoice(
			&snd_itr->lpSourceVoice,
			&snd_itr->wfx,
			XAUDIO2_VOICE_NOPITCH | XAUDIO2_VOICE_MUSIC,
			XAUDIO2_DEFAULT_FREQ_RATIO,
			&callback, nullptr, nullptr))) {
			return E_FAIL;
		}

		snd_itr->bufflength = snd_itr->wfx.nAvgBytesPerSec * 4;
		snd_itr->buffAudio = new unsigned char[snd_itr->bufflength * BUFFERQUEUE_ALLOC];
		snd_itr->startAudio = snd_itr->buffAudio;
		snd_itr->callcount = ++snd_itr->callcount % BUFFERQUEUE_ALLOC;
		snd_itr->audioBytes = mmioRead(snd_itr->hmmio, (LPSTR)snd_itr->startAudio, snd_itr->bufflength);

		if (snd_itr->audioBytes <= 0) {
			return E_FAIL;
		}

		memset(&buffinfo, 0x00, sizeof(buffinfo));

		buffinfo.pAudioData = snd_itr->startAudio;
		buffinfo.Flags = ((uint32_t)snd_itr->audioBytes >= snd_itr->bufflength) ? 0 : XAUDIO2_END_OF_STREAM;
		buffinfo.AudioBytes = snd_itr->audioBytes;
		buffinfo.PlayBegin = 0;
		buffinfo.PlayLength = snd_itr->audioBytes / snd_itr->wfx.nBlockAlign;

		if (snd_itr->loopLength > 0) {
			buffinfo.LoopBegin = snd_itr->loopStart;
			buffinfo.LoopLength = snd_itr->loopLength;
			buffinfo.LoopCount = XAUDIO2_LOOP_INFINITE;
		}

		if (FAILED(snd_itr->lpSourceVoice->SubmitSourceBuffer(&buffinfo, nullptr))) {
			return E_FAIL;
		}

		snd_itr->lpSourceVoice->Start(0, XAUDIO2_COMMIT_NOW);
		return S_OK;
	}
	HRESULT Release() {
		if (snd_itr->hmmio != nullptr) {
			mmioClose(snd_itr->hmmio, 0);
			snd_itr->hmmio = nullptr;
		}
		if (snd_itr->buffAudio != nullptr) {
			delete[] snd_itr->buffAudio;
			snd_itr->buffAudio = nullptr;
		}
		if (snd_itr->lpSourceVoice != nullptr) {
			snd_itr->lpSourceVoice->DestroyVoice();
			snd_itr->lpSourceVoice = nullptr;
		}
		if (snd_itr->hEvent != nullptr) {
			CloseHandle(snd_itr->hEvent);
			snd_itr->hEvent = nullptr;
		}

		return S_OK;
	}
	HRESULT ReadBuff() {

		for (snd_itr = snd_dat.begin(); snd_itr != snd_dat.end(); snd_itr++) {

			snd_itr->lpSourceVoice->GetState(&state);
			callback.hEvent = snd_itr->hEvent;

			if (state.BuffersQueued == 0) {
				return E_FAIL;
			}

			if (state.BuffersQueued < BUFFERQUEUE_MAX && snd_itr->hmmio != nullptr) {

				snd_itr->startAudio = snd_itr->buffAudio + snd_itr->bufflength * snd_itr->callcount;
				snd_itr->callcount = ++snd_itr->callcount % BUFFERQUEUE_ALLOC;
				snd_itr->audioBytes = mmioRead(snd_itr->hmmio, (HPSTR)snd_itr->startAudio, snd_itr->bufflength);

				if (snd_itr->audioBytes <= 0) {
					return E_FAIL;
				}

				buffinfo.Flags = ((uint32_t)snd_itr->audioBytes >= snd_itr->bufflength) ? 0 : XAUDIO2_END_OF_STREAM;
				buffinfo.AudioBytes = snd_itr->audioBytes;
				buffinfo.pAudioData = snd_itr->startAudio;
				buffinfo.PlayBegin = 0;
				buffinfo.PlayLength = snd_itr->audioBytes / snd_itr->wfx.nBlockAlign;

				snd_itr->lpSourceVoice->SubmitSourceBuffer(&buffinfo, nullptr);
				++state.BuffersQueued;

				if (buffinfo.Flags & XAUDIO2_END_OF_STREAM) {
					Release();
					snd_dat.erase(snd_itr);
					snd_dat.shrink_to_fit();
					break;
				}
			}
		}
		return S_OK;
	}

public:
	XAUDIO2() {
		Initialize();
	}
	~XAUDIO2() {
		Finalize();
	}

	HRESULT Update() {

		if (lpXAudio != nullptr) {
			ReadBuff();
		}
		return S_OK;
	}
	HRESULT Load(LPCSTR lpFilename) {
		CreateBuff(lpFilename);
		return S_OK;
	}
};