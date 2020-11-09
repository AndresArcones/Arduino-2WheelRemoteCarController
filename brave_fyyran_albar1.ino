#include <IRremote.h>

#define enable_izq 7
#define enable_der 2
#define control_motor_der_1 3
#define control_motor_der_2 4
#define control_motor_izq_1 5
#define control_motor_izq_2 6

const int pin_mando = 8;
IRrecv irrecv(pin_mando); //objeto de tipo IRrecv
decode_results mando; //datos del mando

bool motor_encendido = false;
int max_speed = 255; //velocidad maxima motor
int speed = 0; //velocidad actual motor


void setup()
{
  pinMode(enable_der,OUTPUT);
  pinMode(enable_izq,OUTPUT);
  pinMode(control_motor_der_1,OUTPUT);
  pinMode(control_motor_der_2,OUTPUT);
  pinMode(control_motor_izq_1,OUTPUT);
  pinMode(control_motor_izq_2,OUTPUT);
  
  
  Serial.begin(9600);
  irrecv.enableIRIn();//activamos el receptor IR
 
}

void loop()
{
  if(irrecv.decode(&mando)){ //cuando no esta encendido el motor
    
    irrecv.resume(); // Reinicia el estado del receptor IR y queda listo esperando el siguiente valor
    
    if(mando.value == 0xFD00FF) //power
      motor_encendido = !motor_encendido;
    
      if(motor_encendido == true)
      	Serial.println("Ignition");
      else Serial.println("Shutdown");
    }
  
  while(motor_encendido){ //cuando esta encendido
    
   	if(irrecv.decode(&mando)){ //cuando no esta encendido el motor
      irrecv.resume();
      
      switch(mando.value){
        case 0xFD00FF: //power
        	motor_encendido = !motor_encendido;
    
     		if(motor_encendido == true)
      			Serial.println("Ignition");
     		else Serial.println("Shutdown");
        break;
        case 0xFD40BF: //func/stop
        	//detendrá todo movimiento y "apagará" el motor, 
       	 	//haciendo que las funciones avanzar, retroceder, 
        	//girar, etc. no funcionen hasta que se presione de 
       		//nuevo el botón Power.
        break;
        
        case 0xFD807F: //vol+
        	//avanzar (se moverá a 1/2 de la velocidad máxima)
        	
        	speed = max_speed/2;
        	
        	analogWrite(enable_der, speed);//velocidad motor der
        	analogWrite(enable_izq, speed);//velocidad motor izq
        
        		//sentido motor DER
        	analogWrite(control_motor_der_1,LOW); 
        	analogWrite(control_motor_der_2,HIGH);
        
        		//sentido motor IZQ
        	analogWrite(control_motor_izq_1,LOW);
        	analogWrite(control_motor_izq_2,HIGH);
        	
        break;
        
        case 0xFD906F: //vol-
        	//retroceder (se moverá a 1/2 de la velocidad máxima)
        
        	speed = max_speed/2;
        	
        	analogWrite(enable_der, speed);//velocidad motor der
        	analogWrite(enable_izq, speed);//velocidad motor izq
        
        		//sentido motor DER
        	analogWrite(control_motor_der_1,HIGH); 
        	analogWrite(control_motor_der_2,LOW);
        
        		//sentido motor IZQ
        	analogWrite(control_motor_izq_1,HIGH);
        	analogWrite(control_motor_izq_2,LOW);
        break;
        
        case 0xFD20DF: //tecla |<<
       		//girar izquierda
        	
        	analogWrite(enable_der, speed/2);//velocidad motor der
        	analogWrite(enable_izq, speed);//velocidad motor izq
        
        break;
        
        case 0xFD609F: //tecla >>|
        	//girara derecha
        	
        	analogWrite(enable_der, speed);//velocidad motor der
        	analogWrite(enable_izq, speed/2);//velocidad motor izq
        break;
        
        case 0xFDA05F: // frenos  //tecla play
        	speed = 0;
        
        	analogWrite(enable_der, speed);//velocidad motor der
        	analogWrite(enable_izq, speed);//velocidad motor izq
        break;
        
        case 0xFD50AF: // flecha arriba
        	speed = speed + max_speed/10;
        
        	analogWrite(enable_der, speed);//velocidad motor der
        	analogWrite(enable_izq, speed);//velocidad motor izq
        break;
        
        case 0xFD10EF: // flecha abajo
        	//Reducir velocidad (se reducirá en 1/10 
        	//de la velocidad máxima posible)
        	
        	speed = speed - max_speed/10;
        
        	analogWrite(enable_der, speed);//velocidad motor der
        	analogWrite(enable_izq, speed);//velocidad motor izq
        break;
        
     }
    
    }
  }
}