//Então, ainda to tentando fazer o código. Não entendi muito as referências e to desvendando devagar. PFVR ME DÊ FEEDBACKS SOBRE!!!!

#include <avr/io.h>
#include <util/delay.h>

#define SENSOR1_PIN PC0
#define SENSOR2_PIN PC1
#define SENSOR3_PIN PC2
#define SENSOR4_PIN PC3
#define SENSOR5_PIN PC4

#define MOTOR1_PIN PB1
#define MOTOR2_PIN PB2

void init_pwm() {
  // Seta PWM pins como output
  DDRB |= (1 << MOTOR1_PIN) | (1 << MOTOR2_PIN);

  // Não entendi muito bem o que faz, mas no guia tava usando
  TCCR1A |= (1 << WGM11) | (1 << COM1A1) | (1 << COM1B1);
  TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11);

  // Seta a frequencia do PWM  para 62.5kHz
  ICR1 = 399;
}

void init_adc() {
  // ADC reference voltage to AVCC
  ADMUX |= (1 << REFS0);

  // ADC clock prescaler to 128
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

  // Liga ADC
  ADCSRA |= (1 << ADEN);
}

int read_sensor(int sensor_pin) {
  // Selecionao cana de ADC baseado no pin do sensor
  ADMUX = (ADMUX & 0xF0) | (sensor_pin & 0x0F);

  // Começa a conversão ADC
  ADCSRA |= (1 << ADSC);

  // Espera conversão completar
  while (ADCSRA & (1 << ADSC));

  // Retorna resultado do ADC
  return ADC;
}

int main() {
  // Inicializa PWM e ADC
  init_pwm();
  init_adc();

  while (1) {
    // Lê o valor dos sensores
    int sensor1 = read_sensor(SENSOR1_PIN);
    int sensor2 = read_sensor(SENSOR2_PIN);
    int sensor3 = read_sensor(SENSOR3_PIN);
    int sensor4 = read_sensor(SENSOR4_PIN);
    int sensor5 = read_sensor(SENSOR5_PIN);

    // Calcula erro
    int error = (sensor1 * 1 + sensor2 * 2 + sensor3 * 0 - sensor4 * 2 - sensor5 * 1) / (sensor1 + sensor2 + sensor3 + sensor4 + sensor5);

    // Calcula velocidade dos motores
    int motor1_speed = 100 + error * 5;
    int motor2_speed = 100 - error * 5;

    // Seta velocidade dos motores usando PWM
    OCR1A = motor1_speed;
    OCR1B = motor2_speed;

    // Dá uma segurada
    _delay_ms(10);
  }

  return 0;
}
