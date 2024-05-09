#include <mgdl-wii.h>
#include <wiiuse/wpad.h>

int main(int argc, char *argv[]) {

	gdl::InitSystem(gdl::ModeMPAL, gdl::Aspect16x9, gdl::HiRes);
	WPAD_Init();
	WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
	gdl::ConsoleMode();

	printf("Hello world!\n");
	while(true)
	{
		WPAD_ScanPads();
		if (WPAD_ButtonsDown(WPAD_CHAN_0) & WPAD_BUTTON_HOME){
			break;
		}
		VIDEO_WaitVSync();
	}
	VIDEO_SetBlack(TRUE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	VIDEO_WaitVSync();
	exit(0);
}