#define clock_cpu_down {CLKPR = 1<<CLKPCE;CLKPR = 8;}   //уменьшаем частоту и энерго потребление
#define clock_cpu_up   {CLKPR = 1<<CLKPCE;CLKPR = 0;}  //возвращаем как было

#define ON HIGH          //включено
#define OFF LOW         //выключено

#define Pre_ON_pulse_time 1000     //Когда светодиод выключен и кнопка нажата
#define Post_ON_pulse_time 500    //время моргания когда кнопка нажата и диод горит
#define Pre_OFF_pulse_time 500   //время моргания когда отпустили кнопку, должен 10 раз моргнуть и выключиться
#define Pre_OFF_pulse_count 10  //количество вспышек перед выключением 

#define PRESS HIGH   //нажата
#define FREE   LOW  //отпущена

#define Button_pin 8   //нога для кнопки
#define Led_Pin 13 		//светодиод

#define prolong 0				//признак продолжения
#define switch_off 1	 //признак переключения


boolean cpu_up=true;

void setup() {
  //инициализируем порты для светодиода и кнопки.
  pinMode(Led_Pin, OUTPUT);     //LED_BUILTIN константа ардуино библиотеки равная 13 для nano и некоторых других плат
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


//====================================================================================================================
void loop() {
  do{
    P_Pre_ON_pulse();                    //вызываем обработчик S_Pre_ON
  }while(digitalRead(Button_pin));      //пока на кнопке высокий уровень
  
  P_OFF(); //пока кнопка свободна гасим все и ложимся спать     

}
