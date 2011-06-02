#include <stdint.h>
#include <pins_arduino.h>
#include "Wiring.h"
#include "SoftSPI.h"

uint32_t SoftSPIClass::_SCKpin = 0;
uint32_t SoftSPIClass::_SSpin = 0;
uint32_t SoftSPIClass::_MOSIpin = 0;
uint32_t SoftSPIClass::_MISOpin = 0;

volatile unsigned int * SoftSPIClass::_SSset;
volatile unsigned int * SoftSPIClass::_SCKset;
volatile unsigned int * SoftSPIClass::_MOSIset;
volatile unsigned int * SoftSPIClass::_MISOset;

volatile unsigned int * SoftSPIClass::_SSclr;
volatile unsigned int * SoftSPIClass::_SCKclr;
volatile unsigned int * SoftSPIClass::_MOSIclr;
volatile unsigned int * SoftSPIClass::_MISOclr;

volatile uint32_t * SoftSPIClass::_SSport;
volatile uint32_t * SoftSPIClass::_SCKport;
volatile uint32_t * SoftSPIClass::_MOSIport;
volatile uint32_t * SoftSPIClass::_MISOport;

static volatile uint32_t *__ports[] = {&PORTA, &PORTB, &PORTC, &PORTD, &PORTF, &PORTG};
static volatile unsigned int *__sets[] = {&PORTASET, &PORTBSET, &PORTCSET, &PORTDSET, &PORTFSET, &PORTGSET};
static volatile unsigned int *__clrs[] = {&PORTACLR, &PORTBCLR, &PORTCCLR, &PORTDCLR, &PORTFCLR, &PORTGCLR};

BYTE SoftSPIClass::transfer(BYTE _data) {
  BYTE ret = 0;
  BYTE bitnum = 128;
  BYTE val;

  for (BYTE i = 0; i < 8; ++i) {
    *((_data & bitnum) ? _MOSIset : _MOSIclr) = _MOSIpin;
    *_SCKset = _SCKpin;
    val = *_MISOport & _MISOpin;
    *_SCKclr = _SCKpin;
    ret <<= 1;
    ret |= (val == 0 ? 0 : 1);
    bitnum >>= 1;
  }
  return ret;
}

  // SPI Configuration methods

void SoftSPIClass::begin(BYTE SSpin, BYTE SCKpin, BYTE MOSIpin, BYTE MISOpin) {
  _SCKpin = SCKpin;
  _MOSIpin = MOSIpin;
  _MISOpin = MISOpin;

  uint8_t p = digitalPinToPort(SSpin) - 1;
  uint8_t b = digitalPinToBitMask(SSpin);
  _SSset = __sets[p];
  _SSclr = __clrs[p];
  _SSport = __ports[p];
  _SSpin = b;

  p = digitalPinToPort(SCKpin) - 1;
  b = digitalPinToBitMask(SCKpin);
  _SCKset = __sets[p];
  _SCKclr = __clrs[p];
  _SCKport = __ports[p];
  _SCKpin = b;

  p = digitalPinToPort(MOSIpin) - 1;
  b = digitalPinToBitMask(MOSIpin);
  _MOSIset = __sets[p];
  _MOSIclr = __clrs[p];
  _MOSIport = __ports[p];
  _MOSIpin = b;

  p = digitalPinToPort(MISOpin) - 1;
  b = digitalPinToBitMask(MISOpin);
  _MISOset = __sets[p];
  _MISOclr = __clrs[p];
  _MISOport = __ports[p];
  _MISOpin = b;

  pinMode(SSpin, OUTPUT);
  pinMode(SCKpin, OUTPUT);
  pinMode(MISOpin, INPUT);
  pinMode(MOSIpin, OUTPUT);
  digitalWrite(SSpin, HIGH);
  digitalWrite(SCKpin, LOW);
}

void SoftSPIClass::end() {
}

void SoftSPIClass::setBitOrder(BYTE) {
}
void SoftSPIClass::setDataMode(WORD) {
}
void SoftSPIClass::setClockDivider(BYTE) {
}
