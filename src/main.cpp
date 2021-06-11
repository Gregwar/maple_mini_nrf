#include "nRF24L01P_PRX.h"
#include "nRF24L01P_PTX.h"
#include "shell.h"
#include <USBSerial.h>
#include <mbed.h>

DigitalOut led(PB_1);
DigitalOut usbDisconnect(PB_9);

#define SPI1_MOSI PB_15
#define SPI1_MISO PB_14
#define SPI1_SCK PB_13
#define SPI1_CS1 PA_15
#define SPI1_CS2 PA_14
#define CE1 PC_14
#define CE2 PB_0
#define INT PB_7

nRF24L01P Device1(SPI1_MOSI, SPI1_MISO, SPI1_SCK, SPI1_CS1);
nRF24L01P Device2(SPI1_MOSI, SPI1_MISO, SPI1_SCK, SPI1_CS2);

nRF24L01P_PTX PTX(Device1, CE1, INT);
nRF24L01P_PRX PRX(Device2, CE2, INT);

SHELL_COMMAND(test, "") {
  char c = 'a';
  shell_println("Transmit");
  int r = PTX.TransmitPacket(&c, 1);
  shell_println(r);

  shell_println(Device1.num_lost_packets());
  shell_println(Device2.num_lost_packets());

  if (PRX.IsPacketReady()) {
    char d;
    int r = PRX.ReadPacket(&d);
    shell_print("Read: ");
    shell_print(r);
    shell_print(" ");
    shell_println(d);
  } else {
    shell_println("Got nothing");
  }

  shell_println((int)Device2.debug_read());

  wait_us(1000);
}

SHELL_COMMAND(dbg, "") {}

int main() {
  shell_init_usb();
  usbDisconnect = 0;

  PTX.Initialize();
  PTX.SetChannel(0);
  PTX.SetDataRate(2000);
  PTX.PowerUp();

  PRX.Initialize();
  PRX.SetChannel(0);
  PRX.SetDataRate(2000);
  PRX.SetPayloadSize(1);
  PRX.PowerUp();
  PRX.StartReceive();

  while (1) {
    ThisThread::sleep_for(100);
  }
}
