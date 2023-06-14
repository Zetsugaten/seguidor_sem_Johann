//Então, ainda to tentando fazer o código. Não entendi muito as referências e to desvendando devagar. PFVR ME DÊ FEEDBACKS SOBRE!!!!
//Atualizado dia 14/06

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
  // Selecionao canal de ADC baseado no pin do sensor
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

  // Definir array para armazenar os valores dos sensores
  int sensor_pins[] = {SENSOR1_PIN, SENSOR2_PIN, SENSOR3_PIN, SENSOR4_PIN, SENSOR5_PIN};
  int num_sensors = sizeof(sensor_pins) / sizeof(sensor_pins[0]);

  while (1) {
    // Lê os valores dos sensores e calcula soma e numerador do erro
    int sensor_values[num_sensors];
    int sensor_sum = 0;
    int error_numerator = 0;

    for (int i = 0; i < num_sensors; i++) {
      sensor_values[i] = read_sensor(sensor_pins[i]);
      sensor_sum += sensor_values[i];
      error_numerator += sensor_values[i] * (i == 0 ? 1 : i + 1);
    }

    // Calcula o inverso da soma dos sensores
    float inverse_sum = 1.0f / sensor_sum;

    // Calcula erro
    float error = error_numerator * inverse_sum;

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
