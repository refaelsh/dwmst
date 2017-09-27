#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <X11/Xlib.h>
#include <alsa/asoundlib.h>
#include <unistd.h>

#define  MAXSTR     1024
#define  MAXVOLUME  65535

static void XSetRoot(const char *  name)
{
	Display *  display ;
	display = XOpenDisplay(0x0) ;

	if (display == NULL)
	{
		//This should not happen, but, you know...
		fprintf(stderr, "Cannot open display!\n");
		exit(1);
	}

	XStoreName(display, DefaultRootWindow(display), name) ;
	XSync(display, 0) ;

	XCloseDisplay(display) ;
}

static void GetCPUUsage(long double *  result)
{
	long double  a[4] ;
	long double  b[4] ;

	FILE *  fp ;

	fp = fopen("/proc/stat", "r");
	fscanf(fp, "%*s %Lf %Lf %Lf %Lf", &a[0], &a[1], &a[2], &a[3]);
	fclose(fp);
	sleep(1);

	fp = fopen("/proc/stat", "r");
	fscanf(fp, "%*s %Lf %Lf %Lf %Lf", &b[0], &b[1], &b[2], &b[3]);
	fclose(fp);

	*result = ((b[0] + b[1] + b[2]) - (a[0] + a[1] + a[2]))
				/ ((b[0] + b[1] + b[2] + b[3]) - (a[0] + a[1] + a[2] + a[3])) ;
}

static void GetTime(char *  result)
{
	time_t       curtime  ;
    struct tm *  loc_time ;
    char		 temp[99] ;

    //Initialize result to empty string.
    result[0] = '\0' ;

    //Getting current time of system
    curtime = time (NULL) ;

    // Converting current time to local time
    loc_time = localtime (&curtime);

    sprintf(temp, "%02d", loc_time->tm_hour) ;
    strcat(result, temp);
    strcat(result, ":");

    sprintf(temp, "%02d", loc_time->tm_min) ;
    strcat(result, temp);
    strcat(result, ":");

    sprintf(temp, "%02d", loc_time->tm_sec) ;
    strcat(result, temp);
}

static void GetVolume(int *  result)
{
	snd_mixer_t *           handle ;
    snd_mixer_selem_id_t *  sid    ;

    const char *  card       = "default" ;
    const char *  selem_name = "Master"  ; //Relevant control.
    int   x             ;
    long  i             ;
    long  currentVolume ;
    long  currentVdB    ;

    //Set up ALSA access.
    snd_mixer_open(&handle, 0) ;
    snd_mixer_attach(handle, card) ;
    snd_mixer_selem_register(handle, NULL, NULL) ;
    snd_mixer_load(handle) ;

    //I don't know what those do...
    snd_mixer_selem_id_alloca(&sid) ;
    snd_mixer_selem_id_set_index(sid, 0) ;
    snd_mixer_selem_id_set_name(sid, selem_name) ;
    snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid) ;

    //Get the volume.
    snd_mixer_selem_get_playback_volume (elem, SND_MIXER_SCHN_FRONT_LEFT, &currentVolume) ;

    *result = (double)currentVolume / (double)MAXVOLUME * 100 ;
}

int main(void)
{
	char         status[MAXSTR]        ;
	char         time  [MAXSTR]        ;
	long double  cpuUsage = 0          ;
	int          temp = 0              ;
	char         currentVolume[MAXSTR] ;
	char         tempString[MAXSTR]    ;

	GetVolume(&temp) ;
	snprintf(currentVolume, MAXSTR, "%d", temp) ;

	for (;;)
	{
		GetCPUUsage(&cpuUsage) ;
		GetTime(time) ;

		snprintf(status, MAXSTR, currentVolume) ;
		strcat(status, "|") ;
		snprintf(tempString, MAXSTR, "%.2Lf%%", cpuUsage * 100) ;
		strcat(status, tempString) ;
		strcat(status, "|") ;
		strcat(status, time) ;

		XSetRoot(status) ;
	}

	return (0) ;
}
