extern int Delay;
extern bool AutoPK;
extern bool Flash;
extern bool CastMissles;
extern HANDLE TimerHandle;
//	DWORD WINAPI TimerTickThread(LPVOID P);
		void TimerTick();
extern bool Test;

void TimerTick();
#define KEYDOWN(vk_code)				((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)