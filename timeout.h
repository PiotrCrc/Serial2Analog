boolean timeout(unsigned long *timer, int ms_delay) { //timer without delay
  if ((unsigned long)(millis() - (*timer)) > ms_delay) {
    *timer = (millis());
    return HIGH;
  } else {
    return LOW;
  }
}
