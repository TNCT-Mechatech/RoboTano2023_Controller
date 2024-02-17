#include "Control.hpp"
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
SerialBridge serial(dev,1024);

AnalogIn X(A6);         //平行移動用のジョイスティックの左右
AnalogIn Y(A5);         //平行移動用のジョイスティックの上下
AnalogIn Turn(A4);      //旋回用のジョイスティックの左右
AnalogIn Speed(A3);     //ローラーの速度変更用の可変抵抗
DigitalIn Roller(D12);  //ローラー用のオンオフスイッチ
DigitalIn Shoot(D11);   //射出ボタン
DigitalIn Arm_up(D10);  //双方向スイッチの上方向
DigitalIn Arm_down(D9); //双方向スイッチの下方向
DigitalIn Hand(D6);     //エアシリンダー

// Message
Control msg;

int main() {
  serial.add_frame(0, &msg);

  while (true) {
    msg.data.joystick_x = static_cast<int8_t>((X.read() - 0.5) * 200); //x方向のジョイスティックの値を1byteで送信できるように変換かつ値の範囲を-100~100までにする
    msg.data.joystick_y = static_cast<int8_t>((Y.read() - 0.5) * 200);//Y方向のジョイスティックの値を1byteで送信できるように変換かつ値の範囲をを-100~100までにする
    msg.data.joystick_turn = static_cast<int8_t>((Turn.read() - 0.5) * 200);//旋回用のジョイスティックの値を1byteで送信できるように変換かつ値の範囲を-100~100までにする
    msg.data.moter_speed = static_cast<int8_t>((Speed.read() - 0.5) * 200);//ローラーの回転速度の値を1byteで送信できるように変換かつ値の範囲を-100~100までにする
    msg.data.roller_status = (Roller == 1) ? true : false;
    msg.data.shoot_bottom = (Shoot == 1) ? true : false;
    msg.data.arm_up = (Arm_up == 1) ? true : false;
    msg.data.arm_down = (Arm_down == 1) ? true : false;
    msg.data.hand_status = (Hand == 1) ? true : false;
    serial.write(0);
    printf("抵抗値:%d  " ,msg.data.moter_speed);
    printf("スイッチ:%s\n",msg.data.roller_status ? "true" : "false");
    wait_us(70 * 1000);
  }
}