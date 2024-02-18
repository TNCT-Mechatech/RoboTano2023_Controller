#include "Control.hpp"
#include "PinNamesTypes.h"
#include "mbed.h"
#include <MbedHardwareSerial.hpp>
#include <SerialBridge.hpp>
#include <cmath>
#include <cstdint>
#include <cstdio>

#define SERIAL_TX D5
#define SERIAL_RX D4

SerialDev *dev =
    new MbedHardwareSerial(new BufferedSerial(SERIAL_TX, SERIAL_RX, 115200));
SerialBridge serial(dev, 1024);

AnalogIn X(A6);     //平行移動用のジョイスティックの左右
AnalogIn Y(A5);     //平行移動用のジョイスティックの上下
AnalogIn Turn(A4);  //旋回用のジョイスティックの左右
AnalogIn Speed(A3); //ローラーの速度変更用の可変抵抗
DigitalIn Roller(D12, PullDown);  //ローラー用のオンオフスイッチ
DigitalIn Shoot(D11, PullDown);   //射出ボタン
DigitalIn Arm_up(D10, PullDown);  //双方向スイッチの上方向
DigitalIn Arm_down(D9, PullDown); //双方向スイッチの下方向
DigitalIn Hand(D6, PullDown);     //エアシリンダー

DigitalOut Led1(A0);
DigitalOut Led2(A1);
DigitalOut Led3(A2);

// Message
Control msg;

int main() {
  serial.add_frame(0, &msg);

  while (true) {

    msg.data.joystick_x = static_cast<int8_t>(
        (X.read() - 0.5) *
        200); // x方向のジョイスティックの値を1byteで送信できるように変換かつ値の範囲を-100~100までにする
    msg.data.joystick_y = static_cast<int8_t>(
        (Y.read() - 0.5) *
        200); // Y方向のジョイスティックの値を1byteで送信できるように変換かつ値の範囲をを-100~100までにする
    Led1 = !Led1;
    msg.data.joystick_turn = static_cast<int8_t>(
        (Turn.read() - 0.5) *
        200); //旋回用のジョイスティックの値を1byteで送信できるように変換かつ値の範囲を-100~100までにする
    msg.data.moter_speed = static_cast<int8_t>(
        (Speed.read() - 0.5) *
        200); //ローラーの回転速度の値を1byteで送信できるように変換かつ値の範囲を-100~100までにする
    Led2 = !Led2;
    msg.data.roller_status = (Roller == 1) ? true : false;
    msg.data.shoot_bottom = (Shoot == 1) ? true : false;
    msg.data.arm_up = (Arm_up == 1) ? true : false;
    msg.data.arm_down = (Arm_down == 1) ? true : false;
    msg.data.hand_status = (Hand == 1) ? true : false;
    Led3 = !Led3;

    //  send
    serial.write(0);

    wait_us(100 * 1000);
  }
}