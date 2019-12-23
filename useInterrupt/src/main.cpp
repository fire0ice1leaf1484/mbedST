#include "stdlib.h"
#include "communication.h"
#include "mbed.h"
#include "pinOutYamaShoEdition.h"
#include "interruptYamaShoEdition.h"

int main()
{
  initAll();
  attachInterrupt();
  while (1)
  {
    for (int i = 0; i < 10; i++)
    {
      pc.printf("%d\t", data[i]);
    }
    myled = !myled;
    wait(0.05);
  }
}