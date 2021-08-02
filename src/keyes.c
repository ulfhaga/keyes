#include <wiringPi.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lirc/lirc_client.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/reboot.h>


int runprogram2(const char *filename, char *const argv[], char *const envp[]);

__pid_t killprogram(__pid_t pid);

void flipLED(int led);

//The WiringPi pin numbers used by our LEDs
#define LED1 24                 // wPi:24 (GPIO:19  phypin:35)
#define LED2 23                 // wPi:23 (GPIO:13  phypin:34)
//#define LED2 5
//#define LED3 6

#define ON 1
#define OFF 0

int main(int argc, char *argv[])
{
  __pid_t pid;
  __pid_t pidMotion;
  
  puts("Start");
  const char *filename = "/usr/bin/mpv";
  const char *fileNameMotion = "/home/pi/bin/motionmail";

  char *const p1[] =
    { "/usr/bin/mpv", "http://sverigesradio.se/topsy/direkt/132-hi-mp3.m3u",
    NULL
  };
  char *const p2[] =
    { "/usr/bin/mpv", "http://sverigesradio.se/topsy/direkt/2562-hi-mp3.m3u",
    NULL
  };
  char *const p3[] =
    { "/usr/bin/mpv", "http://sverigesradio.se/topsy/direkt/164-hi-mp3.m3u",
    NULL
  };
  char *const p3_din_gata[] =
    { "/usr/bin/mpv", "http://sverigesradio.se/topsy/direkt/1607-hi-mp3.m3u",
    NULL
  };

 /*
    char *const p3_star[] =
    { "/usr/bin/mpv", "http://sverigesradio.se/topsy/direkt/1607-hi-mp3.m3u",
    NULL
  };
   */

  char *const nrk_mp3[] =
    { "/usr/bin/mpv", "http://lyd.nrk.no/nrk_radio_mp3_mp3_h.m3u",
    NULL
  };

  char *const play_list[] =
    { "/usr/bin/mpv -playlist", "/mnt/PIHDD/music/ulf.m3u",
    NULL
  };

  char *const envp[] =
    { "HOME=/home/pi", "PATH=/bin:/usr/bin", "DISPLAY=arcer:0", NULL };
    
  pid = -1;
  pidMotion = -1; 

  struct lirc_config *config;

  //Timer for our buttons
  int buttonTimer = millis();

  char *code;
  short last_key = 0;
  //Initiate WiringPi and set WiringPi pins 4, 5 & 6 (GPIO 23, 24 & 25) to output. These are the pins the LEDs are connected to.
  if (wiringPiSetup() == -1)
    exit(1);

  pinMode(LED1, OUTPUT);
  // pinMode(LED1, INPUT);
  pinMode(LED2, OUTPUT);
  //pinMode(LED3, OUTPUT);

  //Initiate LIRC. Exit on failure
  if (lirc_init("lirc", 1) == -1)
    exit(EXIT_FAILURE);

  digitalWrite(LED2, OFF); 

  // Motion larm on
  //pidMotion = runprogram2(fileNameMotion, NULL, NULL);
  
  digitalWrite(LED2, ON);

  //Read the default LIRC config at /etc/lirc/lircd.conf  This is the config for your remote.
  if (lirc_readconfig(NULL, &config, NULL) == 0)
  {
    //Do stuff while LIRC socket is open  0=open  -1=closed.
    while (lirc_nextcode(&code) == 0)
    {
      //If code = NULL, meaning nothing was returned from LIRC socket,
      //then skip lines below and start while loop again.
      if (code != NULL)
      {
        //Make sure there is a 400ms gap before detecting button presses.
        if (millis() - buttonTimer > 400)
        {
          //Check to see if the string "KEY_1" appears anywhere within the string 'code'.

          printf("Code:%s", code);


          if (strstr(code, "KEY_0"))
          {
            printf("MATCH on KEY_0\n");
            //flipLED(LED1);
            digitalWrite(LED1, OFF);
            buttonTimer = millis();
            if (pid > 0)
            {
              pid = killprogram(pid);
            }
            last_key = 0;

          } else if (strstr(code, "KEY_1"))
          {
            printf("MATCH on KEY_1\n");
            buttonTimer = millis();
            printf("pid: %i\n", pid);
            if (last_key != 1)
            {
              digitalWrite(LED1, ON);
              //flipLED(LED1);
              if (pid > 0)
              {
                pid = killprogram(pid);
              }
              if (pid < 0)
              {
                pid = runprogram2(filename, p1, envp);

              }
              last_key = 1;
            }
          }

          else if (strstr(code, "KEY_2"))
          {
            printf("MATCH on KEY_2\n");
            printf("pid: %i\n", pid);
            buttonTimer = millis();
            if (last_key != 2)
            {
              last_key = 2;
              //flipLED(LED1);
              digitalWrite(LED1, ON);
              if (pid > 0)
              {
                pid = killprogram(pid);

              }
              if (pid < 0)
              {
                pid = runprogram2(filename, p2, envp);

              }

            }
          } else if (strstr(code, "KEY_3"))
          {
            printf("MATCH on KEY_3\n");
            printf("pid: %i\n", pid);
            buttonTimer = millis();
            if (last_key != 3)
            {
              last_key = 3;
              //  flipLED(LED1);
              digitalWrite(LED1, ON);
              if (pid > 0)
              {
                pid = killprogram(pid);

              }
              if (pid < 0)
              {
                pid = runprogram2(filename, p3, envp);

              }

            }
          }

          else if (strstr(code, "KEY_4"))
          {
            printf("MATCH on KEY_4\n");
            printf("pid: %i\n", pid);
            buttonTimer = millis();
            if (last_key != 4)
            {
              last_key = 4;
              //flipLED(LED1);
              digitalWrite(LED1, ON);
              if (pid > 0)
              {
                pid = killprogram(pid);

              }
              if (pid < 0)
              {
                pid = runprogram2(filename, p3_din_gata, envp);

              }

            }
          } else if (strstr(code, "KEY_5"))
          {
            printf("MATCH on KEY_5\n");
            printf("pid: %i\n", pid);
            buttonTimer = millis();
            if (last_key != 5)
            {
              last_key = 5;
              digitalWrite(LED1, ON);
              //  flipLED(LED1);
              if (pid > 0)
              {
                pid = killprogram(pid);

              }
              if (pid < 0)
              {
                pid = runprogram2(filename, nrk_mp3, envp);

              }

            }
          }
           else if (strstr(code, "KEY_8"))
          {
            printf("MATCH on KEY_8\n");
            printf("pidMotion: %i\n", pidMotion);
            buttonTimer = millis();
            if (last_key != 8)
            {
              last_key = 8;
            
              if (pidMotion > 0)
              {
                  digitalWrite(LED2, ON);
                  delay(100);                  // mS
                  digitalWrite(LED2, OFF);
                  delay(100); 
                  digitalWrite(LED2, ON);
                  delay(100); 
                  pidMotion = killprogram(pidMotion);
                  digitalWrite(LED2, OFF);
                          printf("Motion dectction stop");
              }
              else
              {
                  digitalWrite(LED2, ON);
                  delay(1000);                  // mS
                  digitalWrite(LED2, OFF);
                  delay(1000); 
                  digitalWrite(LED2, ON);
                  delay(1000); 
                  digitalWrite(LED2, OFF);
                  pidMotion = runprogram2(fileNameMotion, NULL, NULL);
                  printf("Motion dectction run");
              }
            }
          }
          else if (strstr(code, "KEY_9"))
          {
            printf("MATCH on KEY_9\n");
            printf("pid: %i\n", pid);
            buttonTimer = millis();
            if (last_key != 9)
            {
              last_key = 9;
              //flipLED(LED1);
              digitalWrite(LED1, ON);
              if (pid > 0)
              {
                pid = killprogram(pid);
              }
              if (pid < 0)
              {
                pid = runprogram2(filename, play_list, envp);
              }

            }
          }

          else if (strstr(code, "KEY_BACK"))
          {
            printf("MATCH on KEY_BACK\n");
            printf("pid: %i\n", pid);
            buttonTimer = millis();
            if (last_key != 11)
            {
              last_key = 11;
              digitalWrite(LED2, ON);
              //printf("\a");
              //flipLED(LED1);
              //GPIO.cleanup();  
              sync();
              setuid(0);
              system("sudo shutdown -P now");
            }
          }
        }
      }
      //Need to free up code before the next loop
      free(code);
    }
    //Frees the data structures associated with config.
    lirc_freeconfig(config);
  }
  //lirc_deinit() closes the connection to lircd and does some internal clean-up stuff.
  lirc_deinit();
  exit(EXIT_SUCCESS);
}

void flipLED(int led)
{
  //If LED is on, turn it off. Otherwise it is off, so thefore we need to turn it on.
  if (digitalRead(led) == ON)
    digitalWrite(led, OFF);
  else
    digitalWrite(led, ON);

}


int runprogram2(const char *filename, char *const argv[], char *const envp[])
{
  pid_t my_pid = -2;
  if (0 == (my_pid = fork()))
  {
    int status;
    if (argv == NULL)
    {
        fprintf(stdout, "Start program %s\n", filename);
    }
    else
    {
        fprintf(stdout, "Start program %s %s\n", filename , argv[1]);
    }
    fprintf(stdout, "pid: %i\n", my_pid);
    status = execve(filename, argv, envp);
    if (status == -1)
    {
      perror("child process execve failed [%m]");
      return -1;
    }
  }
  return my_pid;
}


__pid_t killprogram(__pid_t pid)
{
  fprintf(stdout, "Kill program pid: %i\n", pid);
  //kill(child_pid,SIGKILL);
  kill(pid, SIGTERM);
  return -1;
}




