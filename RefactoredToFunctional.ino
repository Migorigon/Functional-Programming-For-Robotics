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
const long CIRCUMFERENCE = 2*PI*RADIUS;
long total_displacement = 0;
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
void check_ticks(int encoder)
{
  Serial.println(encoder.read());
}

//same as checking one wheel's distance
long convert_ticks_to_mm(long ticks)
{
   return (ticks*CIRCUMFERENCE)/REV;
}

long check_robot_distance(int right_encoder, int left_encoder)
{
  long lmm = convert_ticks_to_mm(left_encoder.read());
  long rmm = convert_ticks_to_mm(-right_encoder.read());
  return average_distance(lmm,rmm);
}

long average_distance(long mm1,long mm2){
  return ((lmm + rmm)/2);
}

void reset_displacement()
{
  total_displacement = 0;
  left_wheel_ticks.write(0);
  right_wheel_ticks.write(0);
}

/*==================================================*/

/*==================Robot Control===================*/
void move_robot_forward(long intended_distance, int what_speed)
{
  intended_distance += total_displacement;
  set_motor_speed(L_MOTOR, what_speed);
  set_motor_speed(R_MOTOR, (what_speed + OFFSET));
  while (total_displacement < intended_distance)
  {
    total_displacement = check_robot_distance(right_wheel_ticks,left_wheel_ticks);   
  }
  set_motor_speed(L_MOTOR, 0);
  set_motor_speed(R_MOTOR, 0);
}

void move_robot_backward(long intended_distance, int what_speed)
{
  intended_distance += total_displacement;
  set_motor_speed(L_MOTOR, -what_speed);
  set_motor_speed(R_MOTOR, -what_speed);
  while (total_displacement > -intended_distance)
  {
    total_displacement = check_robot_distance(right_wheel_ticks,left_wheel_ticks);   
  }
  set_motor_speed(L_MOTOR, 0);
  set_motor_speed(R_MOTOR, 0);

}

void make_a_square()
{
  for (int i; i < 4; i++)
  {
  move_forward_right(CIRCUMFERENCE, 50);
  delay(3000);
  move_robot_forward(CIRCUMFERENCE, SPEED);
  delay(3000);
  }
}
/*==================================================*/

/*=============Individual Wheel Control=============*/
void move_forward_left(long intended_distance, int what_speed)
{
  intended_distance += total_displacement;
  set_motor_speed(L_MOTOR, what_speed);
  while (total_displacement < intended_distance)
  {
    total_displacement = convert_ticks_to_mm(left_wheel_ticks.read()); 
  }
  set_motor_speed(L_MOTOR, 0); 
}

void move_backward_left(long intended_distance, int what_speed)
{
  intended_distance += total_displacement;
  set_motor_speed(L_MOTOR, -what_speed);
  while (total_displacement > -intended_distance)
  {
    total_displacement = convert_ticks_to_mm(left_wheel_ticks.read());    
  }
  set_motor_speed(L_MOTOR, 0);
}

void move_forward_right(long intended_distance, int what_speed)
{
  intended_distance += total_displacement;
  set_motor_speed(R_MOTOR, what_speed);
  while (total_displacement < intended_distance)
  {
    total_displacement = convert_ticks_to_mm(right_wheel_ticks.read());
  }
  set_motor_speed(R_MOTOR, 0); 
}

void move_backward_right(long intended_distance, int what_speed)
{
  intended_distance += total_displacement;
  set_motor_speed(R_MOTOR, -what_speed);
  while (total_displacement > -intended_distance)
  {
    total_displacement = convert_ticks_to_mm(right_wheel_ticks.read());     
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

