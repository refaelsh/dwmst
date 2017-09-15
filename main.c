#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <unistd.h>

#define MAXSTR  1024

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

int main(void)
{
	char           status[MAXSTR] ;
	long double *  cpuUsage       ;

	for (;;)
	{
		//printf("The current CPU utilization is : %Lf\n", loadavg);
		GetCPUUsage(cpuUsage) ;
		snprintf(status, MAXSTR, "%.2Lf%%", (*cpuUsage * 100)) ;
		XSetRoot(status) ;
	}

	return (0) ;
}
