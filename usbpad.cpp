#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// need to include /usr/include/linux/joystick.h
#include <linux/joystick.h>

#define MAX_AXIS 16
#define MAX_BUTTON 16

struct padData {
  unsigned char axisCount;
  unsigned char buttonCount;
  int fd;
  int version;
  char devName[80];
  int aPos[MAX_AXIS];
  int bPos[MAX_BUTTON];
  bool changed;
  js_event ev;
};

padData pad;


int main (void)
{

  pad.fd = open("/dev/input/js0", O_RDONLY);
  if (pad.fd > 0)
    {
      printf ("js0 opened OK\n");

      // Get pad info ...
      ioctl(pad.fd, JSIOCGAXES, &pad.axisCount);
      ioctl(pad.fd, JSIOCGBUTTONS, &pad.buttonCount);
      ioctl(pad.fd, JSIOCGVERSION, &pad.version);
      ioctl(pad.fd, JSIOCGNAME(80), &pad.devName);
      fcntl(pad.fd, F_SETFL, O_NONBLOCK);

      printf ("axis : %d\n", pad.axisCount);
      printf ("buttons : %d\n", pad.buttonCount);
      printf ("version : %d\n", pad.version);
      printf ("name : %s\n", pad.devName);

      // set default values
      pad.changed = false;
      for (int i=0;i<pad.axisCount;i++) pad.aPos[i]=0;
      for (int i=0;i<pad.buttonCount;i++) pad.bPos[i]=0;

      while (1)
	{
	  int result = read(pad.fd, &pad.ev, sizeof(pad.ev));
	  if (result > 0)
	    {
	      switch (pad.ev.type)
		{
		case JS_EVENT_INIT:
		case JS_EVENT_INIT | JS_EVENT_AXIS:
		case JS_EVENT_INIT | JS_EVENT_BUTTON:
		  break;
		case JS_EVENT_AXIS:
		  pad.aPos[pad.ev.number] = pad.ev.value;
		  pad.changed = true;
		  break;
		case JS_EVENT_BUTTON:
		  pad.bPos[pad.ev.number] = pad.ev.value;
		  pad.changed = true;
		  break;
		default:
		  printf ("Other event ? %d\nnumber=%d\nvalue=%d\n",
			  pad.ev.type,pad.ev.number, pad.ev.value);
		  break;
		}
	    } else usleep(1);


	  if (pad.changed)
	    {
	      printf ("----------------------------------------------\n");
	      printf ("axis : %d\n", pad.axisCount);
	      printf ("buttons : %d\n", pad.buttonCount);
	      printf ("version : %d\n", pad.version);
	      printf ("name : %s\n", pad.devName);
	      printf ("----------------------------------------------\n");
	      printf ("last ev time : %d\n", pad.ev.time);

	      for (int i=0;i<pad.axisCount;i++) printf (" Axis %2d |",i);
	      printf ("\n");
	      for (int i=0;i<pad.axisCount;i++) printf (" %7d |",pad.aPos[i]);
	      printf ("\n\n");
	      for (int i=0;i<pad.buttonCount;i++) printf (" Btn.%2d |",i);
	      printf ("\n");
	      for (int i=0;i<pad.buttonCount;i++) printf (" %2d |",pad.bPos[i]);
	      printf ("\n");
	      pad.changed = false;
	    }
	}
      close(pad.fd);
    } else
    {
      printf ("failed to open /dev/input/js0\n");
    }
  return 0;
}
