#include "timer.h"

void timer::nextmode() {
  if (mode == model[modellen - 1])
    mode = model[0];
  else
    ++mode;
}
