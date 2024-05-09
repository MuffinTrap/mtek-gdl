#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <gccore.h>


// Stuff for video and framebuffer
GXRModeObj	*videoMode;	// Video mode registers
void		*xfb=NULL;	// External framebuffer


// Declaration of the video init routine
void initVideo();


// Main function
int main(int argc, char *argv[]) {
	
	// Initialize video for console display
	initVideo();
	
	// Initialize console
	console_init(xfb, 20, 20, videoMode->fbWidth, videoMode->xfbHeight, videoMode->fbWidth*2);
	
	// Print Hello World message
	printf("\nHello world!\n");
	
	// Wait until reset is pressed
	while(SYS_ResetButtonDown() == false) {
		VIDEO_WaitVSync();
	}
	
}

// Simple video init routine for displaying a text console
void initVideo() {
	
	// Initialize video subsystem
	VIDEO_Init();
	
	// Get preferred video mode according to the console's configuration
	videoMode = VIDEO_GetPreferredMode(NULL);
	
	// Create external framebuffer and clear it
	// (only one framebuffer since we're only going to display a text console)
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(videoMode));
	VIDEO_ClearFrameBuffer(videoMode, xfb, 0x00000000);
	
	// Configure video system, set framebuffer and clear display mask
	VIDEO_Configure(videoMode);
	VIDEO_SetNextFramebuffer(xfb);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	
}
