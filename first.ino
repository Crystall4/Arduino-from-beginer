#define clock_cpu_down {CLKPR = 1<<CLKPCE;CLKPR = 8;}   //уменьшаем частоту и энерго потребление
#define clock_cpu_up   {CLKPR = 1<<CLKPCE;CLKPR = 0;}  //возвращаем как было

#define ON HIGH          //включено
#define OFF LOW         //выключено

#define S_OFF 0       //состояние выключено
#define S_Pre_ON_pulse 1        //состояние когда светодиод выключен и нажали кнопку
#define S_ON 2        //состояние после отпускания кнопки, светодиод горит
#define S_Post_ON_pulse 3       //состояние после нажатия кнопки при включеном светодиоде, должен быстро моргать
#define S_Pre_OFF_pulse 4       //состояние после отпускания кнопки, должен поморгать 10 раз и выключиться

#define Pre_ON_pulse_time 1000     //Когда светодиод выключен и кнопка нажата
#define Post_ON_pulse_time 500    //время моргания когда кнопка нажата и диод горит
#define Pre_OFF_pulse_time 1000   //время моргания когда отпустили кнопку, должен 10 раз моргнуть и выключиться
#define Pre_OFF_pulse_count 25  //количество вспышек перед выключением 

#define PRESS HIGH   //нажата
#define FREE   LOW  //отпущена

#define Button_pin 8   //нога для кнопки
#define Led_Pin 13    //светодиод

#define prolong 0       //признак продолжения
#define switch_off 1   //признак переключения




boolean cpu_up=true;

void setup() {
  //инициализируем порты для светодиода и кнопки.
  pinMode(Led_Pin, OUTPUT);     
  pinMode(Button_pin, INPUT);
}

void led_Pulse(unsigned long pulse_time){        //процедура мигания, параметр период мигания
  static unsigned long previousTime = 0;        //переменная для хранения времени последнего переключения
  if(millis() - previousTime > pulse_time) {    //если с последнего переключения прошло больше времени чем передано в параметре:
      previousTime = millis();                  //сохраняем текущее время как время последнего переключения    
      digitalWrite(Led_Pin, !digitalRead(Led_Pin));  //и реверсируем состояние светодиода
  }
}

void led_count_Pulse(unsigned long pulse_count_time, unsigned int pulse_count){   //процедура мигания n раз 
  static unsigned long previousTime = 0;                    //переменная для хранения времени последнего переключения
  static unsigned int previousCount = 0;                    //переменная для хранения выполненных переключений
  do {
    if(((millis() - previousTime) > pulse_count_time)){ 
      previousCount = previousCount + 1;
      previousTime = millis();                  
      digitalWrite(Led_Pin, !digitalRead(Led_Pin)); 
    } 
  }while (previousCount < (pulse_count * 2));
  previousCount = 0;
}


//===============================Реализация состояний================================================================
byte P_OFF(){
  digitalWrite(Led_Pin,LOW);
  if (cpu_up) {
    clock_cpu_down; //снижаем частоту МК до 62.5 кГц
    cpu_up=false;
  }
  return digitalRead(Button_pin);
}

byte P_Pre_ON_pulse(){
  if (!cpu_up) {
    clock_cpu_up; //Возвращаем частоту МК на 16 МГц
    cpu_up=true;
  }
  led_Pulse(Pre_ON_pulse_time);
  return !digitalRead(Button_pin);
}

byte P_ON(){
  digitalWrite(Led_Pin, ON);
  return digitalRead(Button_pin);
}
byte P_Post_ON_pulse(){
  led_Pulse(Post_ON_pulse_time);
  return !digitalRead(Button_pin);
}
byte P_Pre_OFF_pulse(){
  led_count_Pulse(Pre_OFF_pulse_time, Pre_OFF_pulse_count);
  return switch_off;
}

//====================================================================================================================
void loop() {
  static byte Current_State=S_OFF;
  switch (Current_State) {
    case S_OFF:
      Current_State = Current_State + P_OFF();
      break;
    case S_Pre_ON_pulse:
      Current_State = Current_State + P_Pre_ON_pulse();
      break;
    case S_ON:
      Current_State = Current_State + P_ON();
      break;
    case S_Post_ON_pulse:
      Current_State = Current_State + P_Post_ON_pulse();
      break;
    case S_Pre_OFF_pulse:
      Current_State = Current_State + P_Pre_OFF_pulse();
      break;
    default:
     Current_State=S_OFF;
     break;
  }
}
