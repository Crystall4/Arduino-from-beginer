#define ON HIGH          //включено
#define OFF LOW         //выключено

#define Pre_ON_pulse_time 1000     //Когда светодиод выключен и кнопка нажата
#define Post_ON_pulse_time 500    //время моргания когда кнопка нажата и диод горит
#define Pre_OFF_pulse_time 500   //время моргания когда отпустили кнопку, должен 10 раз моргнуть и выключиться
#define Pre_OFF_pulse_count 10  //количество вспышек перед выключением 

#define PRESS HIGH   //нажата
#define FREE   LOW  //отпущена

#define Button_pin 8   //нога для кнопки

void setup() {
  //инициализируем порты для светодиода и кнопки.
  pinMode(LED_BUILTIN, OUTPUT);     //LED_BUILTIN константа ардуино библиотеки равная 13 для nano и некоторых других плат
  pinMode(Button_pin, INPUT);
}

void led_Pulse(unsigned long pulse_time){        //процедура мигания, параметр период мигания
  static unsigned long previousTime = 0;        //переменная для хранения времени последнего переключения
  if(millis() - previousTime > pulse_time) {    //если с последнего переключения прошло больше времени чем передано в параметре:
      previousTime = millis();                  //сохраняем текущее время как время последнего переключения    
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));  //и реверсируем состояние светодиода
  }
}

void led_count_Pulse(unsigned long pulse_count_time, unsigned int pulse_count){   //процедура мигания n раз 
  static unsigned long previousTime = 0;                    //переменная для хранения времени последнего переключения
  static unsigned int previousCount = 0;                    //переменная для хранения выполненных переключений
  do {
    if(((millis() - previousTime) > pulse_count_time)){ 
      previousCount = previousCount + 1;
      previousTime = millis();                  
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); 
    } 
  }while (previousCount < (pulse_count * 2));
  previousCount = 0;
}

void loop() {
  do{
    led_Pulse(Pre_ON_pulse_time);        //вызываем процедуру мигания
  }while(digitalRead(Button_pin));      //пока на кнопке высокий уровень
  digitalWrite(LED_BUILTIN, OFF);       //гасим светодиод когда на кнопке низкий

}
