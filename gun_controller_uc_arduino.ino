#define DEFAULT_BAUD_RATE 115200
#define PACKET_SIZE 64

//Packet and control
#define PACKET_START_1 0x66
#define PACKET_START_2 0x66
#define PACKET_END_1 0x042
#define PACKET_END_2 0x042

#define CONTROL_START 0x05
#define CONTROL_END 0x10

//Data types
#define DATA_ANALOG_1 1
#define DATA_ANALOG_2 2
#define DATA_ANALOG_3 3
#define DATA_ANALOG_4 4
#define DATA_ANALOG_5 5
#define DATA_ANALOG_6 6
#define DATA_ANALOG_7 7
#define DATA_ANALOG_8 8
#define DATA_ANALOG_9 9
#define DATA_ANALOG_10 10
#define DATA_ANALOG_11 11
#define DATA_ANALOG_12 12
#define DATA_ANALOG_13 13
#define DATA_ANALOG_14 14
#define DATA_ANALOG_15 15
#define DATA_ANALOG_16 16

#define DATA_DIGITAL_1 17
#define DATA_DIGITAL_2 18
#define DATA_DIGITAL_3 19
#define DATA_DIGITAL_4 20

#define analog_filter .6

struct dataStorage{
		uint16_t analog_1;
		uint16_t analog_2;
		uint16_t analog_3;
		uint16_t analog_4;
		uint16_t analog_5;
		uint16_t analog_6;
		uint16_t analog_7;
		uint16_t analog_8;
		uint16_t analog_9;
		uint16_t analog_10;
		uint16_t analog_11;
		uint16_t analog_12;
		uint16_t analog_13;
		uint16_t analog_14;
		uint16_t analog_15;
		uint16_t analog_16;
		
		uint16_t digital_1;
		uint16_t digital_2;
		uint16_t digital_3;
		uint16_t digital_4;
	} data;

//Functions
void setup_hardware(void);
void read_buttons(void);
void read_analog(void);
void pack_data(void);
void transmit(void);
void setup_struct(void);

uint8_t buffer[PACKET_SIZE];

void setup(){
  Serial.begin(DEFAULT_BAUD_RATE);
  setup_hardware();
  setup_struct();
  analogReadResolution(16);
}

void loop(){
  read_buttons();
  read_analog();

  if(Serial.available() >= 1){
     if(Serial.read() == CONTROL_START){
       pack_data();
       transmit(); 
     }
  }
}

void transmit(void){
  //Serial.write(buffer, PACKET_SIZE);
  Serial.write(buffer, PACKET_SIZE);
}

void read_buttons(void){
  static uint8_t flag_pin[16] = {0};
  static uint8_t old_pin[16] = {0};
  uint8_t i;
  
  for(i=0;i<13;i++)
  {
    old_pin[i] = old_pin[i] * .75;
    if(!digitalRead(i)){old_pin[i] = old_pin[i] + 0x3F;}
    //trigger to switch when the analog value gets high or low
    if((old_pin[i] > 0xF0)&&(flag_pin[i]==0)){flag_pin[i]=1; data.digital_1 &= ~(1 << i);}
    if((old_pin[i] < 0x0F)&&(flag_pin[i]==1)){flag_pin[i]=0; data.digital_1 |= (1 << i);}
  }
}

void setup_struct(void){
  data.analog_1 = 0xFFFF;
  data.analog_2 = 0xFFFF;
  data.analog_3 = 0xFFFF;
  data.analog_4 = 0xFFFF;
  data.analog_5 = 0xFFFF;
  data.analog_6 = 0xFFFF;
  data.analog_7 = 0xFFFF;
  data.analog_8 = 0xFFFF;
  data.analog_9 = 0xFFFF;
  data.analog_10 = 0xFFFF;
  data.analog_11 = 0xFFFF;
  data.analog_12 = 0xFFFF;
  data.analog_13 = 0xFFFF;
  data.analog_14 = 0xFFFF;
  data.analog_15 = 0xFFFF;
  data.analog_16 = 0xFFFF;
  
  data.digital_1 = 0xFFFF;
  data.digital_2 = 0xFFFF;
  data.digital_3 = 0xFFFF;
  data.digital_4 = 0xFFFF;
}


void read_analog(void){
  data.analog_1 = (data.analog_1 * analog_filter) + ((1-analog_filter) * analogRead(0));
  data.analog_2 = (data.analog_2 * analog_filter) + ((1-analog_filter) * analogRead(1));
  data.analog_3 = (data.analog_3 * analog_filter) + ((1-analog_filter) * analogRead(2));
  data.analog_4 = (data.analog_4 * analog_filter) + ((1-analog_filter) * analogRead(3));
  data.analog_5 = (data.analog_5 * analog_filter) + ((1-analog_filter) * analogRead(4));
  data.analog_6 = (data.analog_6 * analog_filter) + ((1-analog_filter) * analogRead(5));
  data.analog_7 = (data.analog_7 * analog_filter) + ((1-analog_filter) * analogRead(6));
  data.analog_8 = (data.analog_8 * analog_filter) + ((1-analog_filter) * analogRead(7));
  data.analog_9 = (data.analog_9 * analog_filter) + ((1-analog_filter) * analogRead(8));
  data.analog_10 = (data.analog_10 * analog_filter) + ((1-analog_filter) * analogRead(9));
  data.analog_11 = (data.analog_11 * analog_filter) + ((1-analog_filter) * analogRead(10));
  //data.analog_12 = (data.analog_12 * analog_filter) + ((1-analog_filter) * analogRead(11));
  //data.analog_13 = (data.analog_13 * analog_filter) + ((1-analog_filter) * analogRead(12));
  //data.analog_14 = (data.analog_14 * analog_filter) + ((1-analog_filter) * analogRead(13));
  //data.analog_15 = (data.analog_15 * analog_filter) + ((1-analog_filter) * analogRead(14));
  //data.analog_16 = (data.analog_16 * analog_filter) + ((1-analog_filter) * analogRead(15));
}

void setup_hardware(void){
  //Set pins 0-12 as inputs with pull up resistors enabled
  for(int i=0;i<13;i++){
    pinMode(i, INPUT_PULLUP);
  }
}

void pack_data(void){
  buffer[0] = PACKET_START_1;
  buffer[1] = PACKET_START_2;
	
  buffer[2] = DATA_ANALOG_1;
  buffer[3] = (data.analog_1 >> 8);
  buffer[4] = data.analog_1 & 0xFF;
		
  buffer[5] = DATA_ANALOG_2;
  buffer[6] = (data.analog_2 >> 8);
  buffer[7] = data.analog_2 & 0xFF;
		
  buffer[8] = DATA_ANALOG_3;
  buffer[9] = (data.analog_3 >> 8);
  buffer[10] = data.analog_3 & 0xFF;
  		
  buffer[11] = DATA_ANALOG_4;
  buffer[12] = (data.analog_4 >> 8);
  buffer[13] = data.analog_4 & 0xFF;
  		
  buffer[14] = DATA_ANALOG_5;
  buffer[15] = (data.analog_5 >> 8);
  buffer[16] = data.analog_5 & 0xFF;
  		
  buffer[17] = DATA_ANALOG_6;
  buffer[18] = (data.analog_6 >> 8);
  buffer[19] = data.analog_6 & 0xFF;
  		
  buffer[20] = DATA_ANALOG_7;
  buffer[21] = (data.analog_7 >> 8);
  buffer[22] = data.analog_7 & 0xFF;
  		
  buffer[23] = DATA_ANALOG_8;
  buffer[24] = (data.analog_8 >> 8);
  buffer[25] = data.analog_8 & 0xFF;
  		
  buffer[26] = DATA_ANALOG_9;
  buffer[27] = (data.analog_9 >> 8);
  buffer[28] = data.analog_9 & 0xFF;
  		
  buffer[29] = DATA_ANALOG_10;
  buffer[30] = (data.analog_10 >> 8);
  buffer[31] = data.analog_10 & 0xFF;
  		
  buffer[32] = DATA_ANALOG_11;
  buffer[33] = (data.analog_11 >> 8);
  buffer[34] = data.analog_11 & 0xFF;
  		
  buffer[35] = DATA_ANALOG_12;
  buffer[36] = (data.analog_12 >> 8);
  buffer[37] = data.analog_12 & 0xFF;
  		
  buffer[38] = DATA_ANALOG_13;
  buffer[39] = (data.analog_13 >> 8);
  buffer[40] = data.analog_13 & 0xFF;
  		
  buffer[41] = DATA_ANALOG_14;
  buffer[42] = (data.analog_14 >> 8);
  buffer[43] = data.analog_14 & 0xFF;
  		
  buffer[44] = DATA_ANALOG_15;
  buffer[45] = (data.analog_15 >> 8);
  buffer[46] = data.analog_15 & 0xFF;
  		
  buffer[47] = DATA_ANALOG_16;
  buffer[48] = (data.analog_16 >> 8);
  buffer[49] = data.analog_16 & 0xFF;
  		
  buffer[50] = DATA_DIGITAL_1;
  buffer[51] = (data.digital_1 >> 8);
  buffer[52] = data.digital_1 & 0xFF;
  		
  buffer[53] = DATA_DIGITAL_2;
  buffer[54] = (data.digital_2 >> 8);
  buffer[55] = data.digital_2 & 0xFF;
  		
  buffer[56] = DATA_DIGITAL_3;
  buffer[57] = (data.digital_3 >> 8);
  buffer[58] = data.digital_3 & 0xFF;
  		
  buffer[59] = DATA_DIGITAL_4;
  buffer[60] = (data.digital_4 >> 8);
  buffer[61] = data.digital_4 & 0xFF;
  	
  buffer[PACKET_SIZE-2] = PACKET_END_1;
  buffer[PACKET_SIZE-1] = PACKET_END_2;
}

