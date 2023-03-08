// White Squid Labs Splatfest Installer !

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#include <malloc.h>
#include <sys/stat.h>

#include <iosuhax.h>
#include <iosuhax_devoptab.h>
#include <wut.h>
#include <coreinit/screen.h>
#include <coreinit/cache.h>
#include <coreinit/thread.h>
#include <coreinit/time.h>
#include <coreinit/mcp.h>
#include <coreinit/ios.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <nn/ac/ac_c.h>
#include <nn/result.h>
#include <nsysnet/_socket.h>
#include <whb/proc.h>


int EUR_Installed = 0;
int USA_Installed = 0;
int JPN_Installed = 0;
int EUR_IsUSB = 0;
int USA_IsUSB = 0;
int JPN_IsUSB = 0;
int fsaFd = -1;
static int mcp_hook_fd = -1;
int screenVerticalPlacement = 0;

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

int returnVersions() {
	
	int EUR_IsMLC = 0;
	int USA_IsMLC = 0;
	int JPN_IsMLC = 0;
	FILE *EUR_Splatfest_MLC, *EUR_Splatfest_USB, *USA_Splatfest_MLC, *USA_Splatfest_USB, *JPN_Splatfest_MLC, *JPN_Splatfest_USB;


	EUR_Splatfest_MLC = fopen("storage_mlc:/usr/boss/00050000/10176A00/user/common/data/optdat2/0000054d", "r");
	if (EUR_Splatfest_MLC != NULL) { EUR_IsMLC = 1; }
	EUR_Splatfest_USB = fopen("storage_usb:/usr/boss/00050000/10176A00/usr/common/data/optdat2/0000054d", "r");
	if (EUR_Splatfest_USB != NULL) { EUR_IsUSB = 1; }
	
	if (EUR_IsMLC == 1 || EUR_IsUSB == 1) { EUR_Installed = 1; }
	
	
	USA_Splatfest_MLC = fopen("storage_mlc:/usr/boss/00050000/10176900/user/common/data/optdat2/00000544", "r");
	if (USA_Splatfest_MLC != NULL) { USA_IsMLC = 1; }
	USA_Splatfest_USB = fopen("storage_usb:/usr/boss/00050000/10176900/user/common/data/optdat2/00000544", "r");
	if (USA_Splatfest_USB != NULL) { USA_IsUSB = 1; }

	if (USA_IsMLC == 1 || USA_IsUSB == 1) { USA_Installed = 1; }
	
	
	JPN_Splatfest_MLC = fopen("storage_mlc:/usr/boss/00050000/10162B00/user/common/data/optdat2/000005d2", "r");
	if (JPN_Splatfest_MLC != NULL) { JPN_IsMLC = 1; }
	JPN_Splatfest_USB = fopen("storage_usb:/usr/boss/00050000/10162B00/user/common/data/optdat2/000005d2", "r");
	if (JPN_Splatfest_USB != NULL) { JPN_IsUSB = 1; }

	if (JPN_IsMLC == 1 || JPN_IsUSB == 1) { JPN_Installed = 1; }

	
	if (JPN_Installed == 0 && USA_Installed == 0 && EUR_Installed == 0) { return 803; }

	
	fclose(EUR_Splatfest_MLC);
	fclose(EUR_Splatfest_USB);
	fclose(USA_Splatfest_MLC);
	fclose(USA_Splatfest_USB);
	fclose(JPN_Splatfest_MLC);
	fclose(JPN_Splatfest_USB);

	return 0;
}

void WrapItUp() {
	
	FILE *tee_target, *byaml_target, *panel_target;
	char CherryURL_Tee[] = "http://splatfest.cherrymint.live/festfiles/tee.bfres";
	char CherryURL_Panel[] = "http://splatfest.cherrymint.live/festfiles/panel.bfres";
	char CherryURL_Fest[] = "http://splatfest.cherrymint.live/festfiles/fest.byaml";
	
	curl_global_init(CURL_GLOBAL_ALL);
	
	CURL *TeeData = curl_easy_init();
	CURL *PanelData = curl_easy_init();
	CURL *FestData = curl_easy_init();
	
	curl_easy_setopt(TeeData, CURLOPT_URL, CherryURL_Tee);
	curl_easy_setopt(PanelData, CURLOPT_URL, CherryURL_Panel);
	curl_easy_setopt(FestData, CURLOPT_URL, CherryURL_Fest);
	curl_easy_setopt(TeeData, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(PanelData, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(FestData, CURLOPT_WRITEFUNCTION, write_data);

	if (EUR_Installed == 1) {
		if (EUR_IsUSB == 1) {
			tee_target = fopen("storage_usb:/usr/boss/00050000/10176A00/user/common/data/optdat2/0000054e", "wb");
			byaml_target = fopen("storage_usb:/usr/boss/00050000/10176A00/user/common/data/optdat2/0000054d", "wb");
			panel_target = fopen("storage_usb:/usr/boss/00050000/10176A00/user/common/data/optdat2/0000054c", "wb");
			
			curl_easy_setopt(TeeData, CURLOPT_WRITEDATA, tee_target);
			curl_easy_setopt(PanelData, CURLOPT_WRITEDATA, panel_target);
			curl_easy_setopt(FestData, CURLOPT_WRITEDATA, byaml_target);
			
			curl_easy_perform(TeeData);
			curl_easy_perform(PanelData);
			curl_easy_perform(FestData);

	  		fclose(tee_target);
	  		fclose(byaml_target);
	  		fclose(panel_target);
		}
		else {
			tee_target = fopen("storage_mlc:/usr/boss/00050000/10176A00/user/common/data/optdat2/0000054e", "wb");
			byaml_target = fopen("storage_mlc:/usr/boss/00050000/10176A00/user/common/data/optdat2/0000054d", "wb");
			panel_target = fopen("storage_mlc:/usr/boss/00050000/10176A00/user/common/data/optdat2/0000054c", "wb");
		
			curl_easy_setopt(TeeData, CURLOPT_WRITEDATA, tee_target);
			curl_easy_setopt(PanelData, CURLOPT_WRITEDATA, panel_target);
			curl_easy_setopt(FestData, CURLOPT_WRITEDATA, byaml_target);
			
			curl_easy_perform(TeeData);
			curl_easy_perform(PanelData);
			curl_easy_perform(FestData);

	  		fclose(tee_target);
	  		fclose(byaml_target);
	  		fclose(panel_target);
		}
	}

	OSSleepTicks(OSMillisecondsToTicks(100));

	if (USA_Installed == 1) {
		if (USA_IsUSB == 1) {
			tee_target = fopen("storage_usb:/usr/boss/00050000/10176900/user/common/data/optdat2/00000543", "wb");
			byaml_target = fopen("storage_usb:/usr/boss/00050000/10176900/user/common/data/optdat2/00000544", "wb");
			panel_target = fopen("storage_usb:/usr/boss/00050000/10176900/user/common/data/optdat2/00000545", "wb");
		
			curl_easy_setopt(TeeData, CURLOPT_WRITEDATA, tee_target);
			curl_easy_setopt(PanelData, CURLOPT_WRITEDATA, panel_target);
			curl_easy_setopt(FestData, CURLOPT_WRITEDATA, byaml_target);
			
			curl_easy_perform(TeeData);
			curl_easy_perform(PanelData);
			curl_easy_perform(FestData);
			  
			fclose(tee_target);
	  		fclose(byaml_target);
	  		fclose(panel_target);
		}
		else {
			tee_target = fopen("storage_mlc:/usr/boss/00050000/10176900/user/common/data/optdat2/00000543", "wb");
			byaml_target = fopen("storage_mlc:/usr/boss/00050000/10176900/user/common/data/optdat2/00000544", "wb");
			panel_target = fopen("storage_mlc:/usr/boss/00050000/10176900/user/common/data/optdat2/00000545", "wb");
		
			curl_easy_setopt(TeeData, CURLOPT_WRITEDATA, tee_target);
			curl_easy_setopt(PanelData, CURLOPT_WRITEDATA, panel_target);
			curl_easy_setopt(FestData, CURLOPT_WRITEDATA, byaml_target);
			
			curl_easy_perform(TeeData);
			curl_easy_perform(PanelData);
			curl_easy_perform(FestData);
			  
			fclose(tee_target);
	  		fclose(byaml_target);
	  		fclose(panel_target);
		}
	}

	OSSleepTicks(OSMillisecondsToTicks(100));
	
	if (JPN_Installed == 1) {
		if (JPN_IsUSB) {
			tee_target = fopen("storage_usb:/usr/boss/00050000/10162B00/user/common/data/optdat2/000005d3", "wb");
			byaml_target = fopen("storage_usb:/usr/boss/00050000/10162B00/user/common/data/optdat2/000005d2", "wb");
			panel_target = fopen("storage_usb:/usr/boss/00050000/10162B00/user/common/data/optdat2/000005d4", "wb");
		
			curl_easy_setopt(TeeData, CURLOPT_WRITEDATA, tee_target);
			curl_easy_setopt(PanelData, CURLOPT_WRITEDATA, panel_target);
			curl_easy_setopt(FestData, CURLOPT_WRITEDATA, byaml_target);

			curl_easy_perform(TeeData);
			curl_easy_perform(PanelData);
			curl_easy_perform(FestData);

			fclose(tee_target);
			fclose(byaml_target);
			fclose(panel_target);
		}
		else {
			tee_target = fopen("storage_mlc:/usr/boss/00050000/10162B00/user/common/data/optdat2/000005d3", "wb");
			byaml_target = fopen("storage_mlc:/usr/boss/00050000/10162B00/user/common/data/optdat2/000005d2", "wb");
			panel_target = fopen("storage_mlc:/usr/boss/00050000/10162B00/user/common/data/optdat2/000005d4", "wb");
		
			curl_easy_setopt(TeeData, CURLOPT_WRITEDATA, tee_target);
			curl_easy_setopt(PanelData, CURLOPT_WRITEDATA, panel_target);
			curl_easy_setopt(FestData, CURLOPT_WRITEDATA, byaml_target);
			
			curl_easy_perform(TeeData);
			curl_easy_perform(PanelData);
			curl_easy_perform(FestData);

			fclose(tee_target);
			fclose(byaml_target);
			fclose(panel_target);
		}
		
	}
	
	curl_easy_cleanup(TeeData);
	curl_easy_cleanup(PanelData);
	curl_easy_cleanup(FestData);
	
}

void uselessfunction() {
    // nothing here
}


int MCPHookOpen() {
    mcp_hook_fd = MCP_Open();
    if(mcp_hook_fd < 0) {
        return -1;
	}
	IOS_IoctlAsync(mcp_hook_fd, 0x62, (void*)0, 0, (void*)0, 0, uselessfunction, (void*)0);
    sleep(1);
    if(IOSUHAX_Open("/dev/mcp") < 0) {
        MCP_Close(mcp_hook_fd);
        mcp_hook_fd = -1;
        return -1;
    }
    return 0;
}

void MCPHookClose() {
    if(mcp_hook_fd < 0)
        return;
    IOSUHAX_Close();
    //wait for mcp to return
    sleep(1);
    MCP_Close(mcp_hook_fd);
    mcp_hook_fd = -1;
}


void CleanItUp() {
	
	unmount_fs("storage_mlc");
    unmount_fs("storage_usb");
    IOSUHAX_FSA_Close(fsaFd);
    if(mcp_hook_fd >= 0) {
        MCPHookClose();
	}
    else {
    IOSUHAX_Close();
	}

}


int main() {

	size_t tvBufferSize = OSScreenGetBufferSizeEx(SCREEN_TV);
	size_t drcBufferSize = OSScreenGetBufferSizeEx(SCREEN_DRC);
	
	void* tvBuffer = memalign(0x100, tvBufferSize);
	void* drcBuffer = memalign(0x100, drcBufferSize);

	WHBProcInit();
	OSScreenInit();

	OSScreenSetBufferEx(SCREEN_TV, tvBuffer);
	OSScreenSetBufferEx(SCREEN_DRC, drcBuffer);
	OSScreenEnableEx(SCREEN_TV, true);
	OSScreenEnableEx(SCREEN_DRC, true);

	while(WHBProcIsRunning()) {

	OSScreenClearBufferEx(SCREEN_TV, 0x00000000);
	OSScreenClearBufferEx(SCREEN_DRC, 0x00000000);

	OSScreenPutFontEx(SCREEN_DRC, 0, 0, "Keep an eye on the TV!");
	DCFlushRange(drcBuffer, drcBufferSize);
	OSScreenFlipBuffersEx(SCREEN_DRC);

    int res = IOSUHAX_Open(NULL);
    if(res > 0) {
		fsaFd = IOSUHAX_FSA_Open(NULL);
		mount_fs("storage_mlc", fsaFd, NULL, "/vol/storage_mlc01");
		mount_fs("storage_usb", fsaFd, NULL, "/vol/storage_usb01");
        if(fsaFd < 0) {
			printf("IOSUHAX_FSA_Open Failed...\n");
		}
	}
    else {
        printf("Failed! Returning to HBL.\n");
		return -1;
    }


	OSScreenPutFontEx(SCREEN_TV, 0, 0, "Checking what versions you have installed...");
	OSScreenFlipBuffersEx(SCREEN_TV);
	OSScreenPutFontEx(SCREEN_TV, 0, 0, "Checking what versions you have installed...");
	DCFlushRange(tvBuffer, tvBufferSize);
	OSScreenFlipBuffersEx(SCREEN_TV);
	OSSleepTicks(OSMillisecondsToTicks(1000));
	int statusOf = returnVersions();
	if (statusOf != 0) {
		screenVerticalPlacement++;
		OSScreenPutFontEx(SCREEN_TV, 0, screenVerticalPlacement, "Can't find an installed version of Splatoon.");
		OSScreenFlipBuffersEx(SCREEN_TV);
		OSScreenPutFontEx(SCREEN_TV, 0, screenVerticalPlacement, "Can't find an installed version of Splatoon.");
		DCFlushRange(tvBuffer, tvBufferSize);
		OSScreenFlipBuffersEx(SCREEN_TV);
		OSSleepTicks(OSMillisecondsToTicks(5000));
		break;
	}
	if (EUR_Installed == 1) {
		screenVerticalPlacement++;
		OSScreenPutFontEx(SCREEN_TV, 0, screenVerticalPlacement, "Found Splatoon EUR!");
		OSScreenFlipBuffersEx(SCREEN_TV);
		OSScreenPutFontEx(SCREEN_TV, 0, screenVerticalPlacement, "Found Splatoon EUR!");
		DCFlushRange(tvBuffer, tvBufferSize);
		OSScreenFlipBuffersEx(SCREEN_TV);
	}
	if (USA_Installed == 1) {
		screenVerticalPlacement++;
		OSScreenPutFontEx(SCREEN_TV, 0, screenVerticalPlacement, "Found Splatoon USA!");
		OSScreenFlipBuffersEx(SCREEN_TV);
		OSScreenPutFontEx(SCREEN_TV, 0, screenVerticalPlacement, "Found Splatoon USA!");
		DCFlushRange(tvBuffer, tvBufferSize);
		OSScreenFlipBuffersEx(SCREEN_TV);
	}
	if (JPN_Installed == 1) {
		screenVerticalPlacement++;
		OSScreenPutFontEx(SCREEN_TV, 0, screenVerticalPlacement, "Found Splatoon JPN!");
		OSScreenFlipBuffersEx(SCREEN_TV);
		OSScreenPutFontEx(SCREEN_TV, 0, screenVerticalPlacement, "Found Splatoon JPN!");
		DCFlushRange(tvBuffer, tvBufferSize);
		OSScreenFlipBuffersEx(SCREEN_TV);
	}

	OSSleepTicks(OSMillisecondsToTicks(1000));

	screenVerticalPlacement++;
	OSScreenPutFontEx(SCREEN_TV, 0, screenVerticalPlacement, "Now we just have to install it. One moment...");
	OSScreenFlipBuffersEx(SCREEN_TV);
	OSScreenPutFontEx(SCREEN_TV, 0, screenVerticalPlacement, "Now we just have to install it. One moment...");
	DCFlushRange(tvBuffer, tvBufferSize);
	OSScreenFlipBuffersEx(SCREEN_TV);
	OSSleepTicks(OSMillisecondsToTicks(1000));
	WrapItUp();

	screenVerticalPlacement++;
	OSScreenPutFontEx(SCREEN_TV, 0, screenVerticalPlacement, "Now we clean up...");
	OSScreenFlipBuffersEx(SCREEN_TV);
	OSScreenPutFontEx(SCREEN_TV, 0, screenVerticalPlacement, "Now we clean up...");
	DCFlushRange(tvBuffer, tvBufferSize);
	OSScreenFlipBuffersEx(SCREEN_TV);
	OSSleepTicks(OSMillisecondsToTicks(1000));
	CleanItUp();

	break;
	
	}


	if (tvBuffer) free(tvBuffer);
	if (drcBuffer) free(drcBuffer);

	OSScreenShutdown();
	WHBProcShutdown();

	
	return 1;
}
