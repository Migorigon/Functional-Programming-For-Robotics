/***********************************************************************************
 * Program: Control for MiniBot 5001
 * Programmer: William Bryant
 * INFO: 
 *  left wheel power: pins 9 and 10, right wheel power: pins 5 and 6
 *  left wheel encoder: pins 2 and 7, right wheel encoder: 3 and 8 
 *  IR sensor: pins A1 - 1080 for model GP2Y0A21Y  
 *  Included libraries for encoders and sharp IR snesor
 **********************************************************************************/

/***********************---includes & defines---***********************************/
#include <Encoder.h>
#include <SharpIR.h>

#define IR                A1
#define MODEL             1080

#define RADIUS            16.0
#define REV               358.0
#define SPEED             50.0
#define OFFSET            5.0

#define L_MOTOR           0
#define R_MOTOR           1 

#define L_ENCODER_1       2
#define L_ENCODER_2       7
#define R_ENCODER_1       3
#define R_ENCODER_2       8

#define L_MOTOR_PIN_1     10
#define L_MOTOR_PIN_2     9
#define R_MOTOR_PIN_1     6
#define R_MOTOR_PIN_2     5
/**********************************************************************************/

Encoder left_wheel_ticks(L_ENCODER_1,L_ENCODER_2);
Encoder right_wheel_ticks(R_ENCODER_1,R_ENCODER_2);

/*************************---global variables---***********************************/
long old_position = -999;
long position_left = -999;
long position_right = -999;
long circumference = 2*PI*RADIUS;
long _distance = 0;
long left_distance = 0;
long right_distance = 0;
/**********************************************************************************/

void setup() 
{
  pinMode(R_MOTOR_PIN_1, OUTPUT);
  pinMode(R_MOTOR_PIN_2, OUTPUT);
  pinMode(L_MOTOR_PIN_1, OUTPUT);
  pinMode(L_MOTOR_PIN_2, OUTPUT);
  Serial.begin(4800);
}

void loop() 
{
  make_a_square();
  delay(5000);
  
}

/*************************---function defines---***********************************/

/*=================Monitor Distance=================*/
long read_L_motor_distance()
{
  long new_position = left_wheel_ticks.read();
  if (new_position != old_position)
  {
    old_position = new_position;
    long mm = (new_position*circumference)/REV;
    return mm;
  }  
}

long read_R_motor_distance()
{
  long new_position = right_wheel_ticks.read();
  if (new_position != old_position)
  {
    old_position = new_position;
    long mm = ((new_position*circumference)/REV)*-1;
    return mm;
  }  
}

long read_both_motors_distance()
{
  long new_left, new_right; 
  new_left = left_wheel_ticks.read(); 
  new_right = right_wheel_ticks.read(); 
  position_left = new_left;
  position_right = new_right;
  long lmm = ((new_left*circumference)/REV);
  long rmm = ((new_right*circumference)/REV)*-1;
  long _length = ((lmm + rmm)/2);
  //int a = _length;
  //Serial.print("Left = ");
  //Serial.print(lmm);
  //Serial.print(", Right = ");
  // Serial.print(rmm);
  //Serial.print(", Total Length = ");
  //Serial.print(_length);
  //Serial.println();
  return _length;
  
}

/*==================================================*/

/*==================Robot Control===================*/
void move_robot_forward(long intended_distance, int what_speed)
{
  _distance = 0;
  left_wheel_ticks.write(0);
  right_wheel_ticks.write(0);
  set_motor_speed(L_MOTOR, what_speed);
  set_motor_speed(R_MOTOR, (what_speed + OFFSET));
  while (_distance < intended_distance)
  {
    _distance = read_both_motors_distance();   
  }
  set_motor_speed(L_MOTOR, 0);
  set_motor_speed(R_MOTOR, 0);
}

void move_robot_backward(long intended_distance, int what_speed)
{
  _distance = 0;
  left_wheel_ticks.write(0);
  right_wheel_ticks.write(0);
  set_motor_speed(L_MOTOR, -what_speed);
  set_motor_speed(R_MOTOR, -what_speed);
  while (_distance > -intended_distance)
  {
    _distance = read_both_motors_distance();   
  }
  set_motor_speed(L_MOTOR, 0);
  set_motor_speed(R_MOTOR, 0);

}

void make_a_square()
{
  for (int i; i < 4; i++)
  {
  move_forward_right(circumference, 50);
  delay(3000);
  move_robot_forward(circumference, SPEED);
  delay(3000);
  }
}
/*==================================================*/

/*=============Individual Wheel Control=============*/
void move_forward_left(long intended_distance, int what_speed)
{
  _distance = 0;
  left_wheel_ticks.write(0);
  set_motor_speed(L_MOTOR, what_speed);
  while (_distance < intended_distance)
  {
    _distance = read_L_motor_distance();
    Serial.println(_distance); 
  }
  set_motor_speed(L_MOTOR, 0); 
}

void move_backward_left(long intended_distance, int what_speed)
{
  _distance = 0;
  left_wheel_ticks.write(0);
  set_motor_speed(L_MOTOR, -what_speed);
  while (_distance > -intended_distance)
  {
    _distance = read_L_motor_distance();
    Serial.println(_distance);   
  }
  set_motor_speed(L_MOTOR, 0);
}

void move_forward_right(long intended_distance, int what_speed)
{
  _distance = 0;
  right_wheel_ticks.write(0);
  set_motor_speed(R_MOTOR, what_speed);
  while (_distance < intended_distance)
  {
    _distance = read_R_motor_distance();
    Serial.println(_distance); 
  }
  set_motor_speed(R_MOTOR, 0); 
}

void move_backward_right(long intended_distance, int what_speed)
{
  _distance = 0;
  right_wheel_ticks.write(0);
  set_motor_speed(R_MOTOR, -what_speed);
  while (_distance > -intended_distance)
  {
    _distance = read_R_motor_distance();
    Serial.println(_distance);   
  }
  set_motor_speed(R_MOTOR, 0);
}
/*==================================================*/

/*===============Other Functions====================*/
void set_motor_speed(int motor, int speed)
{
  if (motor == L_MOTOR)
  {
    if(speed > 0)
    {
      analogWrite(L_MOTOR_PIN_2, 0);
      analogWrite(L_MOTOR_PIN_1,speed);
    }
    else
    {
      analogWrite(L_MOTOR_PIN_1, 0);
      analogWrite(L_MOTOR_PIN_2, abs(speed));
    }
  }

  if (motor == R_MOTOR)
  {
    if(speed > 0)
    {
      analogWrite(R_MOTOR_PIN_2, 0);
      analogWrite(R_MOTOR_PIN_1,speed);
    }
    else
    {
      analogWrite(R_MOTOR_PIN_1, 0);
      analogWrite(R_MOTOR_PIN_2, abs(speed));
    }
  }
}
/*==================================================*/

/**********************************************************************************/

