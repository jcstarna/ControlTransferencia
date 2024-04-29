#include <12F1572.h>
#DEVICE ADC=10
#fuses INTRC_IO,NOWDT,NOPROTECT,NOMCLR
#use delay(clock=4000000)
#include <math.h>


#use fast_io(A)
#BYTE    WPU   =   0x95

//    PINES   //////////

#define  VRN      PIN_A0      //Analogica
#define  VSN      PIN_A1      //Analogica
#define  VTN      PIN_A2      //Analogica
#define  pFalla   PIN_A4      //Digital - Salida
#define  pRX      PIN_A5      //Digital - Entrada
#define  pClock   PIN_A3      //Digital - Entrada


#define CantProm 50

INT16 BitCount=0;             //Cantidad de bits recibidos
int8  ByteCount=0;            //Cantidad de Bytes recibidos

int1  fRX_Start=0;            //Hay comunicacion en curso
int1  fDataOk=0;              //Se recibieron bien los datos del micro principal



//Limites de verificacion de tensiones
int16  Dato=0;                 //Dato a enviar por puerto serie

//Tensiones medidas
int16  rVR=0;
int16  rVS=0;
int16  rVT=0;
int16  chksum=0;

int16 tempVR=0;
int16 tempVS=0;
int16 tempVT=0;

int8 RXTimeOut=100;


//funciones
int16 RMSCalc(int ADC_Channel);

// INTERRUPCIONES /////////////////////////////////////////////////////////////
#int_RA
void Clock_RX(){
int16 temp=0;

temp=input_a();
CLEAR_INTERRUPT(int_ra);
//output_low(pFalla);// antes high
if(input(pClock) & !fRX_Start){   
   fRX_Start=1;
   BitCount=0;
   ByteCount=0;
   Dato=*((&rVR) + ByteCount);//Primer dato a enviar
   chksum=Dato;
}
      
if(!input(pClock) & fRX_Start){
   output_bit(pFalla,(shift_right(&Dato,2,0)));
   fRX_Start=1;    //indico que estoy recibiendo  
   //controla si envio todos los bits del Byte
   if (BitCount++==15) {     //llegaron todos los bits del byte
      BitCount=0;
      //Enviaron todos los datos?
         ByteCount++; 
         Dato=*((&rVR) + ByteCount);//Proximo dato a enviar
         if (ByteCount!=3){    
            chksum=chksum+Dato;
         }
         if (ByteCount==4)
         {
            Delay_us(400);
            fRX_Start=0;
            output_high(pFalla);
         }
      }
    }
}



#int_TIMER0
void RTCC_isr() {                      // Interrupción Timer 0
//Watch dog de la comunicacion
//para evitar que el micro quede siempre en recepcion
//sin controlar la tension
   if (fRX_Start){
      if (--RXTimeOut==0){
         fRX_Start=0;
         RXTimeOut=200;                //prueba
         BitCount=0;
         ByteCount=0;
         output_high(pFalla);
      }
   }
   else
      RXTimeOut=200;                //prueba
}


void main() {
int16 Promedio=0;
int i=0;
//Configuracion Oscilador
SETUP_TIMER_0(RTCC_INTERNAL|RTCC_DIV_8);

//Configuracion puertos
Setup_comparator(NC_NC);   //Comparador OFF
port_a_pullups(FALSE);
SET_TRIS_a(0b00101111);            //Direccion de los pines e o s

//WPU = 0;

//Configuracion Analogicas
setup_adc(ADC_CLOCK_DIV_8);
SETUP_ADC_PORTS(sAN0|sAN1|sAN2|VSS_VDD);
SET_ADC_CHANNEL(0);
//Delay_ms(500);

//Interrupciones
ENABLE_INTERRUPTS(INT_TIMER0);     //Interrupcion por timer0
ENABLE_INTERRUPTS(INT_RA3);      //Interrupcion por cambio en el pin Clock
ENABLE_INTERRUPTS(GLOBAL);

//Informar al micro ppal que arranco, para que transmita los datos
output_high(pFalla);
fDataOk=0;

   while (TRUE) {
            rVr=RMSCalc(0);
            Delay_ms(10);
            rVs=RMSCalc(1);
            Delay_ms(10);
            rVt=RMSCalc(2);            
            //-------------------------
            //Medicion de tensiones
            /*SET_ADC_CHANNEL(0);
            Delay_ms(10);
            for (i=0;i<CantProm;i++){
               Delay_ms(1);
               tempVR=read_ADC();
               Promedio = Promedio + tempVR;
            }
               rVR = (promedio/CantProm);
               promedio=0;
               i=0;
            SET_ADC_CHANNEL(1);
            Delay_ms(10);
            for (i=0;i<CantProm;i++){
               Delay_ms(1);
               tempVS=read_ADC();
               Promedio = Promedio  + tempVS;
            }
               rVS = (promedio/CantProm);
               promedio=0;
               i=0;   
               
            SET_ADC_CHANNEL(2);
            Delay_ms(10);
            for (i=0;i<CantProm;i++){
               Delay_ms(1);
               tempVT=read_ADC();
               Promedio = Promedio + tempVT;
            }
               rVT =(promedio/CantProm);
               promedio=0;
               i=0; 
            */
   }//Fin While infinito   
}//Fin Main


int16 RMSCalc(int ADC_Channel){
float fTemp=0;
float fSqrAdd=0;
int8 nMuestras=0;
int16 Lectura=0;
SET_ADC_CHANNEL(ADC_Channel);

//muestrear
   for (nMuestras=0;nMuestras<=50;nMuestras++){
      fTemp=(float)read_ADC();
      fSqrAdd=fSqrAdd+(fTemp*fTemp); 
      Delay_ms(1);
   }
   fTemp = sqrt(fSqrAdd/50);
   return (int16)fTemp;
}
