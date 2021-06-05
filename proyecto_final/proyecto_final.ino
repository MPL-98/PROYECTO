#include <LiquidCrystal.h>

LiquidCrystal lcd(0, 1, 2, 3, 4, 5);//Configuracion de pines para la lcd
unsigned char seg=59,min=59,hor=11;//Variables del reloj
unsigned char min_a=0,hor_a=12;//Variables de la alarma

bool aum_min=0;//Variable para el ajuste de minutos
bool aum_hor=0;//Variable para el ajuste de horas
bool am_pm=0;//Variable para el am y pm
bool am_pm_a=1;
bool alarm=0;//Variable para ver si se esta mostrando la alarma o no

//////////////////////////////////////Configuracion del timer//////////////////////////////////////////

void setup() {
  TCCR1B |= (1 << WGM12);

  OCR1A = 15624;
  TCCR1B |= ((1 << CS10) | (1 << CS12));
  TCCR1B &= ~((1 << CS11));
  TCCR1B &= ~((1 << WGM13));

  TCCR1A = 0x00;
  TIMSK1 |= (1 << OCIE1A);
  sei();
    lcd.begin(16, 2);

    DDRB=0X00;//En este puerto ponemostodos los pulsadores
    DDRC=0xff;//En este puerto ponemos el led de la alarma

}

////////////////////////////////////////Rutina de la interruccion//////////////////////////////////////

ISR(TIMER1_COMPA_vect) {
  seg++;
  if(seg>59) {seg=0,min++;}
  if(min>59) {min=0,hor++;}
  if(hor>12) {hor=1;}
    if(hor==12&&min==0&&seg==0){am_pm=!am_pm; }
TCNT1 = 0x00;
}

//////////////////////////////////////Funcion para mostrar el reloj////////////////////////////////////

void mostrar_reloj (){
  lcd.setCursor ( 0, 0 );
  lcd.print("Reloj Digital");
  lcd.setCursor ( 0, 1 );

  if(hor<10){lcd.print('0');}
  lcd.print(hor);
  lcd.print(':');
  if(min<10){lcd.print('0');}
  

  lcd.print(min);
  lcd.print(':');
  if(seg<10){lcd.print('0');}
  lcd.print(seg);
    if(am_pm==0){lcd.print(" AM");}
    else{lcd.print(" PM");}
}

////////////////////////////////////////Funcion para mostrar la alarma/////////////////////////////////

void mostrar_alarm (){
  lcd.setCursor ( 0, 0 );
  lcd.print("    alarma   ");
  lcd.setCursor ( 0, 1 );

  if(hor_a<10){lcd.print('0');}
  lcd.print(hor_a);
  lcd.print(':');
  if(min_a<10){lcd.print('0');}
  

  lcd.print(min_a);
  
    if(am_pm_a==0){lcd.print(" AM  ");}
    else{lcd.print(" PM ");}
}

//////////////////////////////////////////Funcion loop/////////////////////////////////////////////////

void loop() {
  alarm=digitalRead(10);
    if(alarm==0){
      mostrar_alarm();
      aum_min==digitalRead(8);
      if(aum_min==0){min_a++;
        if(min_a>59){min_a=0;}}

  aum_hor=digitalRead(9);
    if(aum_hor==0){hor_a++;
      if(hor_a>12){hor_a=1;}
      if(hor_a==12){am_pm_a=!am_pm_a;}}

    }else{mostrar_reloj();
    aum_min=digitalRead(8);
      if(aum_min==0){min++;
          if(min>59){min=0;}
      }
    aum_hor=digitalRead(9);
      if(aum_hor==0){hor++;
                if(hor>12){hor=1;}
                if(hor==12){am_pm=!am_pm;}
      }
    }

    if(hor==hor_a&&min==min_a&&am_pm==am_pm_a){digitalWrite(14,HIGH);}
    else{digitalWrite(14,LOW);}
    

}
