#include "Control.hpp"
#include "mbed.h"
#include <MbedHardwareSerial.hpp>
#include <SerialBridge.hpp>
#include <cmath>
#include <cstdio>

#define SERIAL_TX D5
#define SERIAL_RX D4

SerialDev *dev =
    new MbedHardwareSerial(new BufferedSerial(SERIAL_TX, SERIAL_RX, 9600));
SerialBridge serial(dev);

AnalogIn X(A6);//平行移動用のジョイスティックの左右
AnalogIn Y(A5);//平行移動用のジョイスティックの上下
AnalogIn Turn(A4);//旋回用のジョイスティックの左右
AnalogIn Speed(A3);//ローラーの速度変更用の可変抵抗
AnalogIn Roller(D12);//ローラー用のオンオフスイッチ
DigitalIn Shoot(D11);//射出ボタン
DigitalIn Arm_up(D10);//双方向スイッチの上方向
DigitalIn Arm_down(D9);//双方向スイッチの下方向
DigitalIn Hand(D6);//エアシリンダー


// Message
Control msg;

int main() {
  serial.add_frame(0, &msg);

  while (true) {
    msg.data.joystick_x = X;
    msg.data.joystick_y = Y;
    msg.data.joystick_turn = Turn;
    msg.data.moter_speed = Speed;
    msg.data.roller_status = Roller;
    msg.data.shoot_bottom = Shoot;
    msg.data.arm_up = Arm_up;
    msg.data.arm_down = Arm_down;
    msg.data.hand_status  = Hand;
    serial.write(0);
  }
}