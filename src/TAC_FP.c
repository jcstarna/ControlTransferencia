////////////////////////////////////////////////////////////////////////////////
////          PROYECTO TRANSFERENCIA AUTOMATICA FONDO DE PANEL
////FECHA:30/03/2013
////REVISION: 2.00
////AUTOR:CRTARG
////////////////////////////////////////////////////////////////////////////////

//******************************************************************************
//  NOTAS REVISIONES:
//
/*
Revision 1.00: 30/03/13
   Revision inicial
   
Revision 2.00: 01/01/14
   Revision
   
Revision 2.01: 08/03/21
   Agregado de arranque de motor por se~nal externa pin A4
         */


#include <16F886.h>
#DEVICE ADC=10
#fuses NOWDT,PROTECT,NOLVP,NODEBUG,HS
#use delay(clock=10000000)
#USE RS232 (baud=9600, xmit=PIN_B7,rcv=PIN_B6)
#pragma use fast_io(A)
#pragma use fast_io(B)
#pragma use fast_io(C)
#rom int8 0x2100={150,250,250,0X04,2,0X06,3,0X0,10,0X6,1,0x0,3,0x37,1,0x3c,15,00,1,0,1,0,1,0,0,0,5,0}

//    PINES   //////////


#Define  pStatusLed  PIN_C7   //Falla linea PIN_B3
#define  pCO_GR      PIN_C6   //OK FP confirmacion grupo
#Define  pCO_LI      PIN_C5   //OK FP confirmacion linea
#define  pArrExt     PIN_A4   //Arranque por senal extern
//RELES
#define  pCE_LI      PIN_C4   //OK FP Rele contactor de linea
#Define  pCE_GR      PIN_C3   //OK FP Rele contactor de grupo
#Define  pReleParGr  PIN_C2   //OK FP Rele contacto grupo
#Define  pReleArrGr  PIN_C1   //OK FP Rele arranque grupo 
#Define  pCebador    PIN_C0   //OK FP Rele cebador

//ANALOGICAS
//tensiones de linea
#Define  pVRL   13    //canal 13
#Define  pVSL   11    //canal 11
#Define  pVTL   9     //canal 9

//tensiones de grupo
#Define  pVRG   8     //canal 8
#Define  pVSG   10    //canal 10
#Define  pVTG   12    //canal 12

//Potenciometros
#define  pDyArrGr    0  //canal x  delay arranque de grupo
#define  pTmrConGr   1  //canal x  delay conexion de grupo
#define  pTmrLiOk    2  //canal x  Tiempo retorno linea
#define  pAccArr     3  //canal x  Tiempo accionamiento arranque
#define  ptCebador   4  //canal x  Tiempo de accionamiento cebador


// CONSTANTES /////////////////////////////////////////////////////////////////
//int const   kInmuni=10;    // 10mhz - 10;
int const   kTimeOut=5;    //time out de espera de respuesta de micros F12
int const   k1Seg=76;      //conteo para lograr 1 segundo con cristal de 10mhz
int const   kTmr250=19;    //Constante para tener 250 ms.

//constantes de posicion en la eeprom de los valores de configuracion
int const   kVmin=0;       //tension de falla por bajo
int const   kVmax=1;       //tension de falla por alto
int const   kTPareGR=2;    //1 Tiempo de marcha grupo despues de retorno de la linea
int const   kTRetLI=3;     //2 Tiempo de retorno de la linea despues de fallo
int const   kDlyArrGr=4;   //3 Demora al arranque del grupo ante falla de linea
int const   kTConGR=5;     //Tiempo de conexion del grupo ante falla de linea
int const   kIntArr=6;     //Intentos de arranque del grupo
int const   kTArrGR=7;     //tiempo que le da al burro en modo 2 el arranque de grupo
int const   kTPausa=8;     //Tiempo entre intento e intento de arranque
int const   kModoGr=9;     //Modo arranque de grupo
int const   kAcc=10;       //Tipo de accionamiento (contactores o interruptores
//int const   kModoTr=11;    //Modo de funcionamiento de la transf Manual: 0,  o  Automatico: 1
int const   kTestTransf=12;
int const   kCalBujia=13;  //tiempo de calentamiento bujia
int const   kFallaGr=14;   //tiempo de espera de confirmacion cuando esta en modo señal continua para marcha
int const   kGanLi=15;     //multiplicador para conversion de medicion linea
int const   kGanGr=16;     //multiplicador para conversion de mecicion grupo
int const   kCebador=17;   //usa rele de alarma para activar cebador

//constantes de deteccion de falla de operacion de contactores o interruptores

int const   kFaCoLi=4;     //2 segundos para falla a la conexion o desconexion
int const   kFaCoGr=4;     //idem para grupo
int const   kFaParGr=20;   //tiempo de falla a la parada de grupo

//inmunidad para entrada de arranque remoto
int const   kDyArrRemoto=3;//750ms cerrado el contacto arranca grupo en modo remoto (mantenimiento en grupos nafteros)

// VARIABLES EN RAM ///////////////////////////////////////////////////////////


//Flags
int1  fLineaOK=0;        //
int1  fGrupoOK=0;        //
int1  fFalla=0;          //Alguna falla en transferencia pendiented de aceptar


//Flags fallas
int1  fTempFallaGr=0;
int1  fTempFallaLi=0;

//Flag Temporizadores
int1  pul_1seg=0;
int1  p_1seg=0;
int1  p_250ms=0;

//flag de teclado
int1  fGuardarEEPROM=0;


//Variables
//Datos de tensiones para deteccion de fallas
int8  VRmin=0;
int8  VRmax=0;
int8  rFaLi=0;       //Resumen de fallas tensiones de linea
int8  rFaGr=0;
int8  rGanLi=0;
int8  rGanGr=0;

//Tensiones leidas
int16  VRGr=0;
int16  VSGr=0;
int16  VTGr=0;
int16  VRLi=0;
int16  VSLi=0;
int16  VTLi=0;

//Temporarios para filtrar medicion de tensiones
int8   rProm=0;  //contador para promedios
int16  tVRGr=0;
int16  tVSGr=0;
int16  tVTGr=0;
int16  tVRLi=0;
int16  tVSLi=0;
int16  tVTLi=0;

//estado de la transferencia
int16 Estado=0; //aca se cargan los parametros para hacer titilar el led de estado
//Estados
#define kRun       0xFFFF                   //siempre prendido, no hay falla y linea esta conectada- normal
#define kGrIni     0b1100110011001100       //Falla linea, esta arrancando grupo
#Define kGrRun     0b1111000011110000       //Grupo en marcha, pero no conectado/////    1 seg on, 1 off
#define kGrCon     0b1010101010101010       //Grupo conectado.
#define kGrPar     0b1111111100000000       //Corre tiempo de parada de grupo ///////    2 seg on, 2 off
#define kGrFaAr    0b1000000000000000       //Fallo arranque grupo un desdello del led
#define kGrFa      0b1010000000000000       //Fallo grupo. Grupo paro solo sin orden de la TA 2 destellos de led
#define kGrCoFa    0b1010100000000000       //Fallo confirmacion grupo 3 destellos
#define kLiCoFa    0b1010101000000000       //Fallo confirmacion linea 4 destellos
#define kGrFaPar   0b1010101010000000       //Fallo grupo no paro 5 destellos
#define kArrRem    0b1010101010100000       //Arranque mediante entrada externa 

//Tiempos de transferencia
//Grupo
int8  tmrGrFa=3;     //antes 5 tiempo en segundos deteccion de falla tension de grupo
int8  tmrGrOk=3;     //antes 5 Tiempo grupo OK
int8  preFVGr=3;     //antes 5 Preset de tiempo de falla tension de grupo
int8  preGrOk=3;     //antes 5 Preset Retorno de falla de tension de grupo
int8  tmrDyArrGr=5;   //Tiempo entre falla de linea y orden de arranque de grupo
int8  preDyArrGr=5;   //preset tiempo anterior  
int8  tmrPareGr=30;   //Tiempo en minutos de pare de grupo electrogeno una vez que transfirio a linea.
int8  prePareGr=30;   //Preset pare grupo.
int8  tmrConGR=5;     //Tiempo desde que arranco y esta listo el grupo, hasta que da carga
int8  preTmrConGR=5;  //Preset tiempo anterior
int8  tmrCalBuj=5;   //Tiempo entre que pone en contacto el grupo y acciona el arranque
int8  preCalBuj=5;   //Preset del tiempo anterior
int8  pasoArr=0;     //Pasos para el arranque del grupo electrogeno

//Linea
int8  tmrLiFa=3;     //antes 5 tiempo en segundos deteccion de falla tension de linea
int8  tmrLiOk=3;     //antes 5 Tiempo de linea OK
int8  preFVLi=3;     //antes 5 Preset de tiempo de falla tension de linea
int8  preLiOk=3;     //antes 5 Preset de tiempo de reotorno de linea OK
int8  TmrLiIni=1;    //Tiempo para cuando aparece la linea y no esta el grupo electrogeno


//Contador para tener un segundo
int8  tmr1seg=k1Seg; // contador para lograr 1 segundo 
int8  tmr1min=60;
int8  tmr1min1=60;
int8  tmr250ms=ktmr250; //timer 250ms


//Registros de configuracion
int8  rConfig=0;
#bit  fContactor = rConfig.0
#bit  fInterrup  = rConfig.1
int8  fSimuTransf= 0;

int1  Temp_Bool=0;
int1  Temp_Bool1=0;
int1  tmpGrOk=0;

int   rTimeOutGr=kTimeOut;
int   rTimeOutLi=kTimeOut;

//Tiempos arranque grupo electrogeno
int1  OArrGR=0;      //Orden arranque grupo electrogeno
int1  fRunDelayArrGr=0;
int1  fPausa=0;
int1  fFallaGR=0;    //Falla al arranque del grupo electrogeno
int1  DyArrGROff=0;  //no hace la demora a conexion del grupo si falla la red con el grupo en marcha
int1  fArrRemoto=0;  //flag arranque remoto

int8  ModoGr=0;      //Modo arranque grupo electrogeno 0:Rele continuo, 1: Pulso arranca, pulso para, 2:Manejo directo del burro
int8  intentosArr=6; //Intentos de arranque grupo en modo 2
int8  preIntArr=6;
int8  TArr=2;        //Tiempo de arrnaque grupo
int8  preTArr=2;     //Preset tiempo arranque grupo
int8  TPausa=15;      //Tiempo entre intentos de arranque
int8  preTPausa=15;   //Preset tiempo entre intentos
int8  tFallaGR=30;    //tiempo falla arranque en modo señal continua
int8  tPreFallaGr=1; //preset del anterior
int8  tFaGrMar=1;     //falla grupo sigue en marcha
int8  tArrRemoto=kDyArrRemoto;


//Deteccion de fallas de comando
int8  rFaCoLi=kFaCoLi;
int8  rFaApLi=kFaCoLi;
int8  rFaCoGr=kFaCoGr;
int8  rFaApGr=kFaCoGr;
int1  fOCLi=0;             //copia de salida cerrar linea
int1  fOALi=0;             //copia de salida abrir linea
int1  fOCGr=0;             //idem cerrar grupo
int1  fOAGr=0;             //idem abrir grupo

//fallas
int1 fFaCeLi=0;
int1 fFaAbLi=0;

int1 fFaCeGr=0;
int1 fFaAbGr=0;
int1 fFaParoGr=0;   //paro solo el grupo
int1 fFaNoParGr=0;   //falla grupo no paro


//confirmaciones filtradas
int1 fCO_LI=0;
int1 fCO_GR=0;

int8 rCO_LI=10;
int8 rCO_GR=10;

int8 rTPreCeb=0;  //tiempo preseteado de cebador 
int8 rTCeb=0;     //tiempo de cebador
int8 rCeb=0;      //usar cebador
#bit fCeb = rCeb.0

//PRUEBA COMANDO POR TEXTO 
char RX_Char="";
int1 fCommand=0;
int1 fLeer=0;
int1 fEsc=0;
INT1 fMon=0;
int1 fEt=0;
int1 fScan=0;
int8 ParNro=0;
int8 rx_index=0;
INT8 ParVal=0;       //valor del parametro
int8 CausaReinicio=0;



// Declaración de Funciones ///////////////////////////////////////////////////
VOID RX(void);
VOID CMD_EXE(VOID);

void StatusLed(int16 patron){
int1 res=0;
static int i=0;
static int16 ppp;
   res = shift_left(&ppp,2,0);
   if (res)
      output_high (pstatusled);
   else
      output_low (pstatusled);
if (++i >15){
   i=0;
   ppp=patron;
}
}

void Verifica_VLi(void){
      if (VRLi > VRmax)
         bit_clear(rFaLi,0);
      else
         bit_clear(rFaLi,0);
         
      if (VSLi > VRmax)
         bit_clear(rFaLi,1);
      else
         bit_clear(rFaLi,1);
         
      if (VTLi > VRmax)
         bit_clear(rFaLi,2);
      else
         bit_clear(rFaLi,2);
      
      if (VRLi < VRmin)
         bit_set(rFaLi,3);
      else
         bit_clear(rFaLi,3);
         
      if (VSLi < VRmin)
         bit_set(rFaLi,4);
      else
         bit_clear(rFaLi,4);
         
      if (VTLi < VRmin)
         bit_set(rFaLi,5);
      else
         bit_clear(rFaLi,5);
      
      if (rFaLi==0)
         fTempFallaLi=1;
      else
         fTempFallaLi=0;
}

void Verifica_VGr(void){
      if (VRGr > VRmax)
         bit_clear(rFaGr,0);
      else
         bit_clear(rFaGr,0);
         
      if (VSGr > VRmax)
         bit_clear(rFaGr,1);
      else
         bit_clear(rFaGr,1);
         
      if (VTGr > VRmax)
         bit_clear(rFaGr,2);
      else
         bit_clear(rFaGr,2);
      
      if (VRGr < VRmin)
         bit_set(rFaGr,3);
      else
         bit_clear(rFaGr,3);
         
      if (VSGr < VRmin)
         bit_set(rFaGr,4);
      else
         bit_clear(rFaGr,4);
         
      if (VTGr < VRmin)
         bit_set(rFaGr,5);
      else
         bit_clear(rFaGr,5);
      
      if (rFaGr==0)
         fTempFallaGr=1;
      else
         fTempFallaGr=0;
}

//++++++++++++++++++     TEMPORIZADOR   ++++++++++++++++++++++++++++++
/*INT1 Timer(int1 enable, int8 numero, int8 preset, int1 pulso){
int8 temp=0;
temp = timers[numero];

if (!enable){//temporizador no hablilitado, reset y fin rutina
   timers[numero]=0; //reset
   return false;
}
else{  //ESTA habilitado el temporizador
   if(pulso){
         if (temp>=preset) {
               timers[numero]=temp;
               return true;
         }
         else {
               if(++temp>=preset){
                     timers[numero]=temp;
                     return true;
               }
                else
                     timers[numero]=temp;       
         }
      }
}
}
*/

//NORMALIZA EL AD EN UN RECTA ENTRE MIN Y MAX CON 1023 VALORES POSIBLES
int  normVal (int16 AD, int min, int max){
float tempVal=0;
float temp=0;
float tempAD=0;
tempAD = (float) AD;
temp = (float) (max - min);
tempVal= ((tempAD * temp)/1023)+min;
return (int8)tempVal;
}


void LeeParTiempos(){               //lee los potenciometros para los tiempos de las transf
int16 tempAD=0;
//Tiempo delay arranque de grupo
SET_ADC_CHANNEL(pDyArrGr);  
delay_us(20);
tempAD = read_adc();
preDyArrGr = normVal(tempAD,1,240);
//tmrDyArrGr=preDyArrGr;

//Delay conexion de grupo
SET_ADC_CHANNEL(pTmrConGr);  
delay_us(20);
tempAD = read_adc();
pretmrConGR = normVal(tempAD,1,240);
//tmrConGR=pretmrConGR;

//Timer linea OK tiempo que retorna a linea
SET_ADC_CHANNEL(pTmrLiOk);  
delay_us(20);
tempAD = read_adc();
preLiOk = normVal(tempAD,1,240);
//tmrLiOk=preLiOk;

//timer accionamiento burro arranque
SET_ADC_CHANNEL(pAccArr);  
delay_us(20);
tempAD = read_adc();
preTArr= normVal(tempAD,1,10);
//TArr=preTArr;

//timer cebador
SET_ADC_CHANNEL(ptCebador);
delay_us(20);
tempAD = read_adc();
rTPreCeb= normVal(tempAD,1,25);
//rTCeb=rTPreCeb;
}



void RecuperaEEPROM(){

      VRmin=read_EEPROM(kVmin);       //tension de falla por bajo
      VRmax=read_EEPROM(kVmax);       //tension de falla por alto
      
      prePareGr=read_EEPROM(kTPareGR);    //Tiempo de marcha grupo despues de retorno de la linea
      tmrPareGr=prePareGr;
      
      tmrDyArrGr=preDyArrGr;
      tmrConGR=pretmrConGR;
      tmrLiOk=preLiOk;
      TArr=preTArr;
      rTCeb=rTPreCeb;
      //preLiOk=read_EEPROM(kTRetLI);     //Tiempo de retorno de la linea despues de fallo
      //tmrLiOk=preLiOk;
      
      //preDyArrGr=read_EEPROM(kDlyArrGr);   //Demora al arranque del grupo ante falla de linea 
      //tmrDyArrGr=preDyArrGr;
      
      //pretmrConGR=read_EEPROM(kTConGR);     //Tiempo de conexion del grupo ante falla de linea
      //tmrConGR=pretmrConGR;
      
      //preIntArr=read_EEPROM(kIntArr);     //Intentos de arranque del grupo
      intentosArr=3;//preIntArr;
      
      //preTArr=read_EEPROM(kTArrGR);     //tiempo que le da al burro en modo 2 el arranque de grupo
      //TArr=preTArr;
      
      //preTPausa=read_EEPROM(kTPausa);     //Tiempo entre intento e intento de arranque
      TPausa=15;//preTPausa;
      
      //ModoGr=read_EEPROM(kModoGr);     //Modo arranque de grupo
      //rConfig=read_EEPROM(kAcc);        //Tipo de accionamiento (contactores o interruptores
      
      //rModoTransf=read_EEPROM(kModoTr);     //Modo funcionamiento transferencia
      
      //fSimuTransf=read_EEPROM(kTestTransf);     //Modo test
      
      preCalBuj=read_EEPROM(kCalBujia);      //tiempo calentamiento bujia
      tmrCalBuj=1;//preCalBuj;
      
      tPreFallaGr=read_EEPROM(kFallaGr);      //tiempo calentamiento bujia
      tfallaGr = tPreFallaGr;
      
      rGanLi = read_EEPROM(kGanLi);      //Multiplicador
      rGanGr = read_EEPROM(kGanGr);      //Multiplicador
      
      //rTPreCeb=read_EEPROM(kCebador);    //Tiempo Cebado
      //rTPreCeb=2;
      //if (rTPreCeb==0)fCeb=0;            //seteo o reset flag segun tiempo
      //else fCeb=1;
      fCeb=1;
}

void GuardarEEPROM(){

      write_EEPROM(kVmin,VRmin);       //tension de falla por bajo
      write_EEPROM(kVmax,VRmax);       //tension de falla por alto
      
      write_EEPROM(kTPareGR,prePareGr);    //Tiempo de marcha grupo despues de retorno de la linea
      tmrPareGr=prePareGr;
      
      write_EEPROM(kTRetLI,preLiOk);     //Tiempo de retorno de la linea despues de fallo
      tmrLiOk=preLiOk;
      
      write_EEPROM(kDlyArrGr,preDyArrGr);   //Demora al arranque del grupo ante falla de linea 
      tmrDyArrGr=preDyArrGr;
      
      write_EEPROM(kTConGR,pretmrConGR);     //Tiempo de conexion del grupo ante falla de linea
      tmrConGR=pretmrConGR;
      
      write_EEPROM(kIntArr,preIntArr);     //Intentos de arranque del grupo
      intentosArr=preIntArr;
      
      write_EEPROM(kTArrGR,preTArr);     //tiempo que le da al burro en modo 2 el arranque de grupo
      TArr=preTArr;
      
      write_EEPROM(kTPausa,preTPausa);     //Tiempo entre intento e intento de arranque
      TPausa=preTPausa;
      
      write_EEPROM(kModoGr,ModoGr);     //Modo arranque de grupo
      write_EEPROM(kAcc,rConfig);        //Tipo de accionamiento (contactores o interruptores
      
      //write_EEPROM(kModoTr,rModoTransf);
      
      write_EEPROM(kTestTransf,fSimuTransf);     //Modo test  
      
      write_EEPROM(kCalBujia,preCalBuj);     //Tiempo calentamiento bujia
 
      write_EEPROM(kFallaGr ,tPreFallaGr);     //Tiempo falla grupo cuando esta en modo continuo el arr del grupo
      
      write_EEPROM(kGanLi ,rGanli);
      write_EEPROM(kGanGr ,rGanGr);
      
      write_EEPROM(kCebador ,rTPreCeb);
      
}


// INTERRUPCIONES /////////////////////////////////////////////////////////////

#int_RTCC
void RTCC_isr() {                      // Interrupción Timer 0
  
if (--tmr1seg==0){
   pul_1seg=1;
   tmr1seg=k1Seg;// 10mhz - 76; 
         //Deteccion de falla de grupo
         IF(!fTempFallaGr){                  //Si esta en falla el grupo
            if ((--tmrGrFa==0)){             //Decremento temporizador
               fGrupoOk=0;                   //Fin temporizador  Setea falla grupo
               tmrGrFa=preFVGr;              //cargo valor en el temporizador
            }
         }
         else{
               tmrGrFa=preFVGr;
         }
         
         IF(fTempFallaGr){      //Grupo ok
            if ((--tmrGrOk==0)){
               fGrupoOk=1;
               tmrGrOk=preGrOk;
            }
         }
         else{
               tmrGrOk=preGrOk;
         }
         
         if(!fTempFallaLi & fLineaOk){       //Si esta en falla la linea
            if ((--tmrLiFa==0)){
               fLineaOk=0;                         //flag de falla
            }
         }
         else{
               tmrLiFa=preFVli;
         }
         
         if(fTempFallaLi & !fLineaOk){           // & fGrupoOk   Si Linea Ok y grupo conectado
            if ((--TmrLiOk==0)){       //Retorno de linea despues de falla
               fLineaOk=1;                         //y que estoy alimentando desde grupo
               tmrLiOk=preLiOk;
            }
         }
         else{
               tmrLiOK=preLiOk;
         }
         
         if (fTempFallaLi & !input(pCO_GR)){    //Si LineaOk y grupo desconectado pongo menos tiempo para retorno de la linea
            if ((--TmrLiIni==0) & !FlineaOk){      //para la primera vez que se conecta el equipo no esperar a que pase
                  fLineaOk=1;                      //Todo el tiempo seteado
                  tmrLiIni=1;
            }
         }
       }

//Timer solo para muestreo
if (--tmr250ms==0){
   p_250ms=1;
   tmr250ms=ktmr250;
}

if (fCO_LI){             //si ya esta confirmado
   if(!input(pCO_LI)){   //si no esta la entrada
      if(rCO_LI--==0){   //decremento y si es cero seteo flag
         fCO_LI=0;
         rCO_LI=10;
      }
   }
   else
      rCO_LI=10;
}
if (!fCO_LI){             //si no esta confirmado
   if(input(pCO_LI)){     //si esta la entrada
      if(rCO_LI--==0){   //decremento y si es cero seteo flag
         fCO_LI=1;
         rCO_LI=10;
      }
   }
   else
      rCO_LI=10;
}
//IDEM GRUPO
if (fCO_GR){             //si ya esta confirmado
   if(!input(pCO_GR)){   //si no esta la entrada
      if(rCO_GR--==0){   //decremento y si es cero seteo flag
         fCO_GR=0;
         rCO_GR=10;
      }
   }
   else
      rCO_GR=10;
}
if (!fCO_GR){             //si no esta confirmado
   if(input(pCO_GR)){     //si esta la entrada
      if(rCO_GR--==0){   //decremento y si es cero seteo flag
         fCO_GR=1;
         rCO_GR=10;
      }
   }
   else
      rCO_GR=10;
}
}//Fin timer 0 ISR

#INT_RB
void RB6_isr(void){
int RX =0;
   disable_interrupts(INT_RB6); 
   rx=GETC();
   rx_char=RX;
   fCommand=1;
}



// FUNCION PRINCIPAL BUCLE ////////////////////////////////////////////////////

void main(){
CausaReinicio=RESTART_CAUSE();
//Configura timer 0

setup_timer_0(RTCC_8_BIT |RTCC_INTERNAL|RTCC_DIV_128);//

OUTPUT_A(0);
OUTPUT_B(0);
OUTPUT_C(0);

//Configuracion de puertos   
set_tris_a(0b00111111);
set_tris_b(0b01111111);
set_tris_c(0b01100000);


//Configuracion Analogicas
setup_adc(ALL_ANALOG | ADC_CLOCK_INTERNAL | VSS_VDD);
setup_comparator(NC_NC_NC_NC);            //no comparadores

////seteo interrupciones
   enable_interrupts(int_rtcc);                  // Habilita Interrupción TMR0
   enable_interrupts(INT_RB6);                  // Habilita Interrupción rb6
   enable_interrupts(global);                    // Habilita interrupciones


tmrLiIni=1;          //Tiempo para conexion inicial del sistema
fFallaGR=0;          //reset falla grupo

LeeParTiempos();     //Leo parametros de los potes
RecuperaEEPROM();    //Leo parametros almacenados en la EEPROM
///printf("INICIANDO\r\n");
delay_ms(3500);

//inicio parametros comunicacion
RX_Char="";
fCommand=0;
fLeer=0;
fEsc=0;
ParNro=0;
rx_index=0;


tmr1seg=k1Seg;
rTimeOutGr=kTimeOut;
rTimeOutLi=kTimeOut;
tmr1min=60;

//preparar todo para cuando se active la linea o el grupo
//le pase los datos.
rFaCoLi=kFaCoLi;
rFaApLi=kFaCoLi;
rFaCoGr=kFaCoGr;
rFaApGr=kFaCoGr;
tFaGrMar=kFaParGr;

SET_ADC_CHANNEL(pVRL);  //seteo ad para leer tension R linea
Estado=kRun;
////////////////////////////////////////////////////////
//Bucle principal
////////////////////////////////////////////////////////
while(true){
if (pul_1seg){
      p_1seg=1;
      pul_1seg=0;
}

if (fGuardarEEPROM){
   fGuardarEEPROM=0;
   guardarEEPROM();
}
//--------------------------------------------------------------------------------------------
//Lectura Tensiones linea y grupo
//--------------------------------------------------------------------------------------------
if(p_250ms){
   SET_ADC_CHANNEL(pVRL);  //seteo ad para leer tension R linea
   DELAY_US (20);
   tVRLi=tVRLi+read_adc();
   SET_ADC_CHANNEL(pVSL);  //seteo ad para leer tension S linea
   DELAY_US (20);
   tVSLi=tVSLi+read_adc();
   SET_ADC_CHANNEL(pVTL);  //seteo ad para leer tension T linea
   DELAY_US (20);
   tVTLi=tVTLi+read_adc();   
   SET_ADC_CHANNEL(pVRG);  //seteo ad para leer tension R grupo
   DELAY_US (20);
   tVRGr=tVRGr+read_adc();
   SET_ADC_CHANNEL(pVSG);  //seteo ad para leer tension S grupo
   DELAY_US (20);
   tVSGr=tVSGr+read_adc(); 
   SET_ADC_CHANNEL(pVTG);  //seteo ad para leer tension T grupo
   DELAY_US (20);
   tVTGr=tVTGr+read_adc(); 
   SET_ADC_CHANNEL(pVRL);  //seteo ad para leer tension R linea
   
 if (++rProm==10){  
      VRGr=tVRGr/10;
      VSGr=tVSGr/10;
      VTGr=tVTGr/10;
      VRLi=tVRLi/10;
      VSLi=tVSLi/10;
      VTLi=tVTLi/10;
      tVRGr=0;
      tVSGr=0;
      tVTGr=0;
      tVRLi=0;
      tVSLi=0;
      tVTLi=0; 
      rProm=0;
      LeeParTiempos();
    }
    StatusLed(Estado);
    //si esta activo el pin de arranque remoto
    if(input(pArrExt)){
      if(--tArrRemoto==0){
         fArrRemoto=1;
      }  
    }
    else{//si no esta, resetea contador
      tArrRemoto=kDyArrRemoto;
      fArrRemoto=0;
    }
}//fin rutina cada 250ms

if (p_250ms)p_250ms=0;


//--------------------------------------------------------------------------------------------
//Control de limites de tension linea
//--------------------------------------------------------------------------------------------
Verifica_VLi();
   
//--------------------------------------------------------------------------------------------
//Control de limites de tension Grupo
//--------------------------------------------------------------------------------------------
Verifica_VGr();

//############################################################################################
//                                OPERACION DE CONTACTORES
//############################################################################################

//--------------------------------------------------------------------------------------------
//Apertura interruptor linea
//--------------------------------------------------------------------------------------------
if(!fLineaOK & fOCLi){                           //Si falla la linea //input(pCo_LI)
      fOCLi=0;                           //Abro rele de red
      RecuperaEEPROM();                  //Recupero parametros para nueva transferencia 
}
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//Cierre interruptor de linea
//--------------------------------------------------------------------------------------------
if(fLineaOK  & !input(pCo_GR) & !fOCLi){    //Si esta ok la linea y no esta metido el grupo electrogeno
         fOCLi=1;
        // OArrGR=0;                          //conteo para apagar el grupo
}     
IF (fLineaOK & input(pCO_LI))
         OArrGR=0; 
//-------------------------------------------------------------------------------------------- 

//--------------------------------------------------------------------------------------------
//deteccion de falla de operacion de contactor
//--------------------------------------------------------------------------------------------
   if(fOCLi & !fCO_LI & !fFaCeLi) //si esta la orden de cerrar y no esta la confirmacion
      Temp_Bool=1;
   else
      Temp_Bool=0;
      
  //agrego que el sistema este en automatico para que controle la falla    
  if((!fOCLi) & fCO_LI)  //si esta configurado contactor y no esta lo orden de cerrar y esta la confirmacion del contactor     
      Temp_Bool1=1;
  else
      Temp_Bool1=0;
      
   //decremento tiempo de falla de comando
   if(p_1seg)  {
      //Falla al cierre
      if (temp_bool){
         if(rFaCoLi--==0){
            rFaCoLi=kFaCoLi;
            fFalla=1;
            fFaCeLi=1;
         }
      }
      else
         rFaCoLi=kFaCoLi;
     
     //falla a la apertura
      if (temp_bool1){
         if(rFaApLi--==0){
            rFaApLi=kFaCoLi;
            fFalla=1;
            fFaAbLi=1;
         }
      }
      else
         rFaApLi=kFaCoLi;     
   }
//--------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------
//Apertura interruptor de Grupo
//--------------------------------------------------------------------------------------------
if((fLineaOk |!fGrupoOK)){    // Si retorna la linea y esta el contactor de grupo
      fOCGr =0;               //Abro contactor de grupo
}
//--------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------
//Cierre interrutpor de grupo
//--------------------------------------------------------------------------------------------
if(fGrupoOk & !fLineaOk & !fOCGr){                //Si esta en falla la linea y listo el grupo y no esta conectado // & !fOAGr
   if(p_1seg | (DyArrGROff==1)){
      if((tmrConGR--==0) | (DyArrGROff==1)){       //demora al cierre de interruptor de grupo
               DyArrGROff=1;                       // no hacer demora si ya esta en marcha el grupo y vuelve a fallar la red
               if(!input(pCo_LI)){
                  fOCGr =1;                        //Cierre interruptor de grupo
                  }
               }
   }
}
else
   tmrConGR=pretmrConGR;
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------            
//deteccion de falla de operacion de contactor
   if(fOCGr & !fCO_GR & !fFaCeGr) //si esta la orden de cerrar y no esta la confirmacion
      Temp_Bool=1;
   else
      Temp_Bool=0;
  
  //Agregado de que este en automatico para que informe falla
  if(((fContactor & !fOCGr)) & fCO_GR & !fFaAbGr)  //si esta configurado contactor y no esta lo orden de cerrar y esta la confirmacion del contactor   //|(fInterrup & fOAGr  
      Temp_Bool1=1;
  else
      Temp_Bool1=0;
      
   //decremento tiempo de falla de comando
   if(p_1seg)  {
      //falla al cierre
      if (temp_bool){
         if(rFaCoGr--==0){
            rFaCoGr=kFaCoGr;
            fFalla=1;
            fFaCeGr=1;
         }
      }
      else
         rFaCoGr=kFaCoGr;
         
     //Falla a la apertura
      if (temp_bool1){
         if(rFaApGr--==0){
            rFaApGr=kFaCoGr;
            fFalla=1;
            fFaAbGr=1;
         }
      }
      else
         rFaApGr=kFaCoGr;     
   }
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//ESCRITURA SALIDAS CONTACTORES
//--------------------------------------------------------------------------------------------
//Cierre
if (fOCLi)
      output_high(pCE_LI);
else
      output_low(pCE_LI);

//Cierre
if (fOCGr)
      output_high(pCE_GR);
else
      output_low(pCE_GR);

//--------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------
//Orden de marcha del grupo electrogeno
//--------------------------------------------------------------------------------------------
if (((!fLineaOK  & !fFallaGR) | fCO_GR ) & !OArrGR) fRunDelayArrGr=1; //| fArrRemoto
else  fRunDelayArrGr=0;

if(fRunDelayArrGr){                                   //Si hay que arrancar el grupo
//Delay al arranque del grupo electrogeno
   if(p_1seg){
      if(--tmrDyArrGr==0){                            //si paso el tiempo de demora al arranque del grupo
            OArrGR=1;                                 //Inicio secuencia arranque grupo
            //Carga tiempos de arranque grupo
            TArr=preTArr;                             //Tiempo de arrnaque grupo
            TPausa=preTPausa;                         //Tiempo entre intentos de arranque
            fPausa=0;
            intentosArr=preIntArr;
            tmrDyArrGr=preDyArrGr;
            tmrCalBuj=preCalBuj;
            rTCeb=rTPreCeb;
            pasoArr=1;
            }
   }
}


//--------------------------------------------------------------------------------------------
//Puesta en marcha del grupo electrogeno segun tipo seteado
//--------------------------------------------------------------------------------------------
if (OArrGR){
  //arranque por motor de arranque
               switch (pasoArr) {
                  case 1:    //paso el tiempo de demora al arranque, pongo en contacto el grupo
                        output_high(pReleParGr);               //pone en contacto el grupo
                        pasoArr=2;                             //precalentamiento
                        break;
                  case 2://demora de precalentamiento
                        if (p_1seg){
                           if(tmrCalBuj--==0)
                              pasoArr=3;
                              TArr=preTArr; //carga tiempo de arranque
                              rTCeb=rTPreCeb;
                        }
                        break;
                  case 3://le da al burro el tiempo configurado
                        if (!fGrupoOk & !fFallaGR){
                              output_high(pReleArrGr);
                              output_high(pCebador);    //si esta configurado el cebador, lo activa
                        }
                        pasoArr=4;
                        break;
                  case 4://Pausa entre arranque y arranque
                        if (fGrupoOk){
                              output_low(pReleArrGr);
                              TArr=preTArr; 
                              pasoArr=5;
                        }
                        
                         if (p_1seg){ //tiempo de accionamiento del burro
                              if(tArr--==0){
                                 output_low(pReleArrGr);
                                 TPausa=preTPausa;
                                 TArr=preTArr;
                                 pasoArr=5;  //hacer pausa en arranque
                              }
                              if(fCeb){
                                 if (rTCeb--==0){//si termino tiempo de cebado
                                    output_low(pCebador); 
                                    rTCeb=rTPreCeb;
                                 }
                                 }
                        }
                        break;
                  case 5://grupo en marcha
                        if (fGrupoOk){
                              output_low(pReleArrGr);
                              pasoArr=6;
                        }
                        
                        if (p_1seg){ //tiempo de pausa
                           if(tPausa--==0){//si paso el tiempo de pausa
                              output_low(pReleArrGr);
                              if(intentosArr--!=0){    //si no hizo todos los intentos de arranque
                                 pasoArr=3;
                                 TArr=preTArr;
                                 rTCeb=rTPreCeb;
                              }
                              else{
                              //error no arranco despues de todos los intentos 
                                 OArrGR=0;             //fin secuencia de arranque.
                                 intentosArr=preIntArr;
                                 fFallaGR=1;
                                 fFalla=1;
                                 pasoArr=1;  //prepara para cuando resetea alarma ya hace el arranque
                                 tmrCalBuj=preCalBuj;
                              }
                           }
                               if(fCeb){
                                 if (rTCeb--==0){//si termino tiempo de cebado
                                    output_low(pCebador); 
                                    rTCeb=rTPreCeb;
                                 }
                              } 
                        }
                        break;
                  case 6://grupo en marcha
                        if(fCeb){//si esta activado el cebador
                           //decrementa tiempo de accionamiento de cebador
                           if(p_1seg){
                              if (rTCeb--==0){//si termino tiempo de cebado
                                 output_low(pCebador); 
                                 pasoArr=7;
                              }
                           }
                        }
                        break; 
                  case 7:
                        break;
               }
   
}//Fin if arranque grupo


//--------------------------------------------------------------------------------------------
//Parada del grupo electrogeno segun tipo seteado
//--------------------------------------------------------------------------------------------
if (!OArrGR){
   if(p_1seg){
      if( --tmr1min==0){
            tmr1min=60;
            if(--tmrPareGr==0){ 
                  DyArrGROff=0;            //VUELVE A ACTIVAR DEMORA A LA CONEXION DEL GRUPO
                  output_low(pCebador);    //Saco cebador 
                  Output_low(pReleArrGr);  //Abro contacto grupo
                  output_low(pReleParGr);  
                  }
            }
      }
   }
else{
   tmrPareGr=prePareGr;
   tmr1min=60;
}//Fin if parada grupo

//--------------------------------------------------------------------------------------------
//FALLAS
//--------------------------------------------------------------------------------------------
//Falla parada de grupo sin orden de la TA
if(OArrGR){
   if(!fGrupoOk & tmpGrOk){
         fFalla=1,
         fFaParoGr=1;   //paro solo el grupo
   }
}
tmpGrOk=fGrupoOk;

//Falla grupo no paró

if((!OArrGR & (tmrPareGr==0) & fGrupoOk )){
   if (p_1seg){
         if(--tmr1min1==0){
            tmr1min1=60;
            //if (--tFaGrMar==0){
               tFaGrMar=tPreFallaGr;
               fFalla=1;
               fFaNoParGr=1;
            //}
         }
   }
}
else
   tFaGrMar=tPreFallaGr;
//--------------------------------------------------------------------------------------------
// PASO DATOS DE FALLAS AL LED DE ESTADO
//--------------------------------------------------------------------------------------------
if(fFalla){
   if(fFaCeLi | fFaAbLi) Estado=kLiCoFa;  //falla contactor de linea
   if(fFaCeGr | fFaAbGr) Estado=kGrCoFa;  //falla contactor de grupo
   if(fFaNoParGr) Estado=kGrFaPar;        //falla grupo no paro 
   if(fFaParoGr) Estado=kGrFa;            //falla grupo paro solo 
   if(fFallaGR) Estado=kGrFaAr;           //falla arranque grupo 
   
   if(fCO_GR | fCO_Li)fFaCeGr=0, fFaAbGr=0;        //si da confirmacion de grupo saca fallas de cerrar conf grupo
   if(fCO_Li | fCO_GR)fFaCeLi=0, fFaAbLi=0;        //si da confirmacion de linea saca fallas de cerrar conf linea
   if(!fGrupoOk)fFaNoParGr=0;                      //si se cae tension de grupo saca falla grupo no paro
   if(fGrupoOk | fLineaOk)fFallaGR=0,fFaParoGr=0;  //si da tension de linea o grupo saca falla de grupo paro solo.
   
   if(!fFaCeLi & !fFaAbLi & !fFaCeGr & !fFaAbGr & !fFaNoParGr & !fFaParoGr & !fFallaGR) fFalla=0; //si desaparecieron todas las fallas baja flag de falla pendiente
}
else{
//--------------------------------------------------------------------------------------------
// PASO ESTADO DE LA TA AL LED DE ESTADO
//--------------------------------------------------------------------------------------------
      IF (OArrGr){                                          //si esta en falla la linea y orden de marcha del grupo
         if(!fCO_GR & !fGrupoOk)       Estado=kGrIni;       //si esta arrancando el grupo y no esta conectado
         else if (!fCO_GR & fGrupoOk)  Estado=kGrRun;       //si esta arrancado pero no conectado
         else if (fCO_GR & fGrupoOk)   Estado=kGrCon;       //si esta arrancado y conectado
      }
      else{
         if(fGrupoOk)                  Estado=kGrPar;          //si no esta la orden de marchar grupo pero hay tension... esta pasando el tiempo de parar grupo
         else if (!fGrupoOk & fLineaOk & fCO_LI)Estado=kRun;   //si no esta la orden de marchar y grupo parado y linea ok y conectada, modo run
      }
}
if (p_1seg)p_1seg=0;

if (fCommand) RX();
IF (fScan) CMD_EXE();

} //Fin buble infinito
}//Fin main


VOID RX(void){
/*estructura de comandos
1 byte= comando: R:leer, W:escribir, M:monitorear, E:tiempos remanentes
2 byte= decimal parametro: 0 a 9
3 byte= unidad parametro: 0 a 9
4 byte= caracter fin comando 0x0A si es lectura
        caracter = si es escritura
5 byte= centena valor parametro;
6 byte= decena  valor parametro;
7 byte= unidad valor parametro;
8 byte= caracter fin de comando 0x0A
*/
   fCommand=0;
   switch (rx_index){
      case 0://primer byte recibido (COMANDO)
            if (rx_char == 'R') fLeer =1;
            else if (rx_char == 'W')fEsc=1;
            else if (rx_char == 'M')fMon=1;
            else if (rx_char == 'E')fEt=1;
            parNro=0;
            break;
      case 1://segundo byte (DECIMAL PARAMETRO)
            if (fMon){
               if (rx_char == 'T')parNro = 1;
               else if (rx_char == 'P')parNro = 2;
               ELSE if (rx_char == 'F')parNro = 3;
            }
            else
               parNro = (rx_char - 0x30)*10;
            break;
      case 2://segundo byte (UNIDAD PARAMETRO)
            IF (!fMon)
               parNro = (rx_Char-0x30)+parNro;
            BREAK;
      case 3://caracter "=" si es escritura
            break;
      case 4://centena de valor a setear
            ParVal= (rx_char - 0x30)*100;
            break;
      case 5://decena de valor a setear
            ParVal= (rx_char - 0x30)*10 + ParVal;
            break; 
      case 6://centena de valor a setear
            ParVal= (rx_char - 0x30)+ ParVal;
            break;
   }
   rx_index++;
   
   if (rx_char == 0x0D){
      fScan=1;
      rx_index=0;
   }
   ELSE
   enable_interrupts(INT_RB6); 
}

void CMD_EXE(void){
fScan=0;
int16 tempAD=0;
//PRINTF("CMD_EXE");
DISable_interrupts(INT_RB6);

if (fLeer) printf("Valor: %u\r\n",READ_EEPROM(ParNro));

if (fEsc){
  WRITE_EEPROM(ParNro,ParVal);
  delay_us(100);
  printf("Par/Val: %u, %u\r\n",ParNro,READ_EEPROM(ParNro));
}

IF (fMon){
   if(ParNro==1){       //tensiones
      SET_ADC_CHANNEL(pVRL);  
      delay_us(20);
      tempAD = read_adc();
      printf("VRL: %Lu\r\n",tempAD);
      
      SET_ADC_CHANNEL(pVSL);  
      delay_us(20);
      tempAD = read_adc();
      printf("VSL: %Lu\r\n",tempAD);
      
      SET_ADC_CHANNEL(pVTL);  
      delay_us(20);
      tempAD = read_adc();
      printf("VTL: %Lu\r\n",tempAD);
      
      SET_ADC_CHANNEL(pVRG);  
      delay_us(20);
      tempAD = read_adc();
      printf("VRG: %Lu\r\n",tempAD);
      
      SET_ADC_CHANNEL(pVSG);  
      delay_us(20);
      tempAD = read_adc();
      printf("VSG: %Lu\r\n",tempAD);
      
      SET_ADC_CHANNEL(pVTG);  
      delay_us(20);
      tempAD = read_adc();
      printf("VTG: %Lu\r\n",tempAD);
   }
   else if(ParNro==2){  //potenciometros
      LeeParTiempos();
      printf("Dy Arr Gr: %u s\r\n",preDyArrGr);
      printf("Dy Con Gr: %u s\r\n",pretmrConGR);
      printf("Dy Ret Li: %u s\r\n",preLiOk);
      printf("T Acc Arr: %u s\r\n",preTArr);
      printf("T Acc Ceb: %u s\r\n",rTPreCeb);
   }
   else if (ParNro==3){
      printf("Linea OK: %u \r\n",fLineaOK);
      printf("Grupo OK: %u \r\n",fGrupoOk);
      printf("Co Lin: %u \r\n",fCO_LI);
      printf("Co Gr: %u \r\n",fCO_GR);
      printf("FA Gr: %u \r\n",fFallaGR);
      printf("O Arr: %u \r\n",OArrGR);
      printf("Dy Arr: %u \r\n",fRunDelayArrGr);
      printf("Estado: %LX \r\n",ESTADO);
   }
   ELSE fMon=0;
} 
if (fEt){
   //if(ParNro==1){  //Tienmpos actuales temporizadores
      printf("T Arr Gr: %u s\r\n",tmrDyArrGr);
      printf("T Con Gr: %u s\r\n",tmrConGR);
      printf("T Ret Li: %u s\r\n",TmrLiOk);
      printf("T Acc Arr: %u s\r\n",tArr);
      printf("T Acc Ceb: %u s\r\n",rTCeb);
      //printf("T1 min: %u s\r\n",tmr1min);
      printf("Tmr Pare Gr: %u:%u m\r\n",tmrPareGr,tmr1min);
      printf("Paso Arr: %u \r\n",pasoArr);
   //}
   //ELSE fEt=0;
}

if(!fLeer && !fEsc && !fMon && !fEt) printf("CMD ERROR\r\n");
parNro=0;
rx_index=0;
fLeer=0;
fEsc=0;
fMon=0;
fEt=0;
enable_interrupts(INT_RB6); 
}

