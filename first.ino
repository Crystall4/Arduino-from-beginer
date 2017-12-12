#define ON HIGH          //включено
#define OFF LOW         //выключено

#define Pre_ON_pulse_time 1000     //Когда светодиод выключен и кнопка нажата
#define Post_ON_pulse_time 500    //время моргания когда кнопка нажата и диод горит
#define Pre_OFF_pulse_time 500   //время моргания когда отпустили кнопку, должен 10 раз моргнуть и выключиться

#define PRESS HIGH   //нажата
#define FREE   LOW  //отпущена

#define Button_pin 8   //нога для кнопки

void setup() {
  //инициализируем порты для светодиода и кнопки.
  pinMode(LED_BUILTIN, OUTPUT);     //LED_BUILTIN константа ардуино библиотеки равная 13 для nano и некоторых других плат
  pinMode(Button_pin, INPUT);
}

void ledPulse(unsigned long pulse_time){        //процедура мигания, параметр период мигания
  static unsigned long previousTime = 0;        //переменная для хранения времени последнего переключения
  if(millis() - previousTime > pulse_time) {    //если с последнего переключения прошло больше времени чем передано в параметре:
      previousTime = millis();                  //сохраняем текущее время как время последнего переключения    
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));  //и реверсируем состояние светодиода
  }
}

void loop() {
  do{
    ledPulse(Pre_ON_pulse_time);        //вызываем процедуру мигания
  }while(digitalRead(Button_pin));      //пока на кнопке высокий уровень
  digitalWrite(LED_BUILTIN, OFF);       //гасим светодиод когда на кнопке низкий

}
