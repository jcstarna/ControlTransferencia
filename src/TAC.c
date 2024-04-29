////////////////////////////////////////////////////////////////////////////////
////                   PROYECTO TRANSFERENCIA AUTOMATICA
////FECHA:22/07/2022
////REVISION: 7.01
////AUTOR:CRTARG
////////////////////////////////////////////////////////////////////////////////

//******************************************************************************
//  NOTAS REVISIONES:
//
/*
Revision 4.15: 14/06/11
         Se elimina fallas de operacion de contactores o interruptores si el sistema
         esta en manual
         
Revision 4.16: 15/06/11
         Se agrega la opcion de ver las tensiones con un nuevo menu, que se accede
         apretando simultaneamente las teclas menu y ok.
         Se modifica que al salir una alarma y uno esta programando, al aceptar la
         alarma, se vuelve al menu de programacion que estaba.
        
Revision 4.17 18/06/11
         Se agrega calibracion de mediciones mientras esta visualizando las mediciones
         
Revision 4.20 03/03/12
         Se agrega opcion de pasar de manual a automatico con boton de OK.
         
Revision 4.30 14/03/12
         Modificacion del funcionamiento en manual del equipo.

Revision 5.00 16/08/12
         Implementacion de reloj con timer 0
         Implementacion de salida de alarma como activacion de cebador para 
         motores nafteros
         
Revision 5.01  15/01/13
         Problema que se activaba el cebador cuando el grupo estaba en marcha
         y volvia a caer la linea, transferia y activaba el cebador. Resuelto
         
Revision 5.02 26/01/13
         Al pasar con la tecla OK al menu de cebador, mantenia el valor del menu
         anterior y no el valor del tiempo del cebador -> Corregido
         
Revision 5.10 11-02-13
         Modificacion para apagar el display y salir de la configuracion
         si no hay actividad del usuario presionando teclas.
         Modificacion para el 16F886
         Bug del cebador al parar, si no alcanzo a arrancar quedaba pegado el cebador
Revision 5.11 02-03-13

Revision 5.20 16-11-13 
         1.-Modificacion funcionamiento cebador para que se pueda desactivar antes de terminar el tiempo de accionamiento del burro. (OK)
         2.-Agregado de un cuenta horas de marcha del grupo (probado, anda, test, en 5hs error 1 minuto)
         3.-Posibilidad de arrancar el grupo desde la TA (no... por ahora no hay lugar)
         4.-Desactivar el test de transferencia al encender la central.    (listo, probar) (OK)
         5.-Puesta a cero del contador de horas... (OK)
         
Revision 5.25 15/01/14
         1.-Optimizar memoria
         2.-Eliminar parpadeo de display...(OK)
         3.-Funcion pare con electrovalvula usando salida alarma (OK)
         4.-Arranque del grupo desde la TA (probar)   
         25/12/14
         1.-Correccion para que no de la falla de contactores sl la linea o grupo esta en falla
         2.-Se saca que tenga que estar en automatico para el arranque del grupo desde la TA
Revision 6.00
         1.-Arranque remoto por entrada externa
Revision 7.00
         1.-Cambio de micro 18f26k22  
         2.-Calibracion independiente de fases de grupo y linea
         3.-Deteccion de que alguna fase supero el minimo para detectar arranque
         4.-Se elimina del registro de fallas, que el pin del micro de medicion este en alto
            ya que reseteaba contador de linea ok, y este pin se controla antes de iniciar
            la medicion.
         5.-prueba de comunicacion en 16 bits entre micros de med y ppal... OK
         6.-implementacion de aproximacion cuadratica de la medicion...Probada
         7.-Correccion en % de lo calculado en la aprox cuadratica....Probada
         8.-Agregado de visualizacion de tension de grupo y linea Menu 21 y 22... probado
         9.-Retardo a la conexion del contactor de linea (ver de eliminar tiempo) probado
         10.- Menu para configuracion de lo anterior, probado
         11.- Agregado de una histersis para grupo y para linea. falta probar
         12.- Menu para configuracion de lo anterior, falta
         
Revision 7.01
         1.-Correccion de caracteres raros en pantalla, se deshabilita interrupciones antes de act display

Revision 7.02         
         1.-Correccion de error de medicion cuando hay tension cero (falta de fase)
         */

#include <18f25k22.h>
#fuses HSM,NOWDT,NOLVP,NODEBUG,NOPLLEN
#use delay(clock=10000000)
#pragma use fast_io(A)
#pragma use fast_io(B)
#pragma use fast_io(C)
#rom  int8 getenv("EEPROM_ADDRESS")= {180,250,180,250,2,10,10,10,3,4,15,2,1,0,0,2,1,0x70,   0,0,0,0,0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x0,0,0,0,0,0,0,5,5}
                                                                                      //,0x70,2,    0xBA,0xB9,0xB8,0xBA,0xBA,0xB
//    PINES   //////////

#define  pBacklight  PIN_A0   //Control LED DISPLAY
#define  pCO_GR      PIN_A1   //Confirmacion Marcha - Contactor 2
#Define  pReleArrGr  PIN_A2   //OK Rele ARRANQUE GRUPO
#Define  pCE_GR      PIN_A3   //OK Rele CERRAR INT GRUPO
#Define  pCO_LI      PIN_A4   //Confirmacion Marcha - Contactor 1
#Define  pReleParGr  PIN_A5   //OK Rele PARE GRUPO

#Define  pALARM      PIN_C0   //Rele 4 ALARMA
#DEFINE  pAB_GR      PIN_C1   //OK Rele abrir int grupo
#Define  pCE_LI      PIN_C2   //OK Rele Cerrar int linea
#define  pAB_LI      PIN_C3   //OK Rele 3 Rele contactor de linea
#Define  pClkLi      PIN_C4   //clock micro linea
#Define  pClkGr      PIN_C5   //clock micro grupo
#Define  pArrRem     PIN_C6   //Pin externo para arranque remoto
#Define  pFallaLi    PIN_C7   //Falla linea PIN_B3

///LCD

#define LCD_RS_PIN      PIN_B0                                    ////
#define LCD_RW_PIN      PIN_B1                                    ////
#define LCD_ENABLE_PIN  PIN_B2                                    ////
#Define pFallaGr        PIN_B3   
#define LCD_DATA4       PIN_B4                                    ////
#define LCD_DATA5       PIN_B5                                    ////
#define LCD_DATA6       PIN_B6                                    ////
#define LCD_DATA7       PIN_B7 

#include <lcd.c>


// CONSTANTES /////////////////////////////////////////////////////////////////
int const   kInmuni=15;    // 10mhz - 10;
int const   kInmuniRep=10; // 10mhz - 10;                // Constante para repeticion de la tecla.
int const   kTmrOK=15;     //Tiempo que tiene apretada una tecla para pasar de manual a automatico
int const   kTimeOut=5;    //time out de espera de respuesta de micros F12
int const   k1Seg=75;      //conteo para lograr 1 segundo con cristal de 10mhz
int const   kTmr250=19;    //Constante para tener 250 ms. 19 VALOR ORIGINAL
int const   kTmrInact=240;  //tiempo en segundo de inactividad del teclado cuando esta configurando

//constantes de posicion en la eeprom de los valores de configuracion
int const   kVminLi=0;     //tension de falla por bajo
int const   kVmaxLi=1;     //tension de falla por alto
int const   kVminGr=2;     //tension de falla por bajo
int const   kVmaxGr=3;     //tension de falla por alto
int const   kTPareGR=4;    //Tiempo de marcha grupo despues de retorno de la linea
int const   kTRetLI=5;     //Tiempo de retorno de la linea despues de fallo
int const   kDlyArrGr=6;   //Demora al arranque del grupo ante falla de linea
int const   kTConGR=7;     //Tiempo de conexion del grupo ante falla de linea
int const   kIntArr=8;     //Intentos de arranque del grupo
int const   kTArrGR=9;     //tiempo que le da al burro en modo 2 el arranque de grupo
int const   kTPausa=10;    //Tiempo entre intento e intento de arranque
int const   kModoGr=11;    //Modo arranque de grupo
int const   kAcc=12;       //Tipo de accionamiento (contactores o interruptores
int const   kModoTr=13;    //Modo de funcionamiento de la transf Manual: 0,  o  Automatico: 1
int const   kTipoPare=14;  //Tipo de parada de grupo 0 = por contacto, 1 = por electrovalvula
int const   kCalBujia=15;  //tiempo de calentamiento bujia
int const   kFallaGr=16;   //tiempo de espera de confirmacion cuando esta en modo señal continua para marcha
//int const   kGanLi=17;     //multiplicador para conversion de medicion linea
//int const   kGanGr=18;     //multiplicador para conversion de mecicion grupo
int const   kCebador=18;   //Antes 19 usa rele de alarma para activar cebador
int const   kHsMarcha=19;  //Antes 20 direccion del primer byte de las horas de marcha (16 bits)
int const   kMinMarcha=21; //direccion minutos e marcha
int const   kGanLiR=22;    //multiplicador para correccion medicion linea Fase R
int const   kGanLiS=23;    //multiplicador para correccion medicion linea Fase S
int const   kGanLiT=24;    //multiplicador para correccion medicion linea Fase T
int const   kGanGrR=25;    //multiplicador para correccion medicion grupo Fase R
int const   kGanGrS=26;    //multiplicador para correccion medicion grupo Fase S
int const   kGanGrT=27;    //multiplicador para correccion medicion grupo Fase T
int const   kTEvPare=28;   //preset tiempo ev pare
int const   kDyArrRem=29;  //delay para aceptar
int const   kF_SD1=30;     //funcion salida digital 1
int const   kF_SD2=31;     //funcion salida digital 2
int const   kF_SD3=32;     //funcion salida digital 3
int const   kDyLi=33;      //tiempo para hacer entrar el contactor de linea luego de abrir el de grupo
int const   kHistLi=34;    //histeresis en voltios de la linea
int const   kHistGr=35;    //histerasis en voltios del grupo

//int const   kTOffCeb=18; //tiempo de desactivacion de cebador 


//constantes de deteccion de falla de operacion de contactores o interruptores

int const   kFaCoLi=10;     //2 segundos para falla a la conexion o desconexion
int const   kFaCoGr=10;     //idem para grupo
int const   kFaParGr=20;   //tiempo de falla a la parada de grupo


int const   k0hora=10;     //10 segundos presionada la tecla subir pone a cero el cuentahoras
int const   kTActV=10;     //10 segundos para detectar que ha tension en las 3 fases

// VARIABLES EN RAM ///////////////////////////////////////////////////////////

//int   xbuff=0x00;                       // Índice: siguiente char en cbuff
//int8  [8]=0,0,0,0,0,0,0,0;              // Buffer

//char  Rxbuff[lenbuff];                  // Buffer
//char  rcvchar=0x00;                     // último caracter recibido
//int   txlen=0;
//int   txpoint=0;

//variables de teclado
int   rInmunidad=kInmuni;
int   rTmrOK=kTmrOK;

//Flags
int1  fDataOkGr=0;       //Dato transmitido ok al micro de grupo
int1  fDataOkLi=0;       //Dato Transmitido ok al micro de linea
int1  fLineaOK=0;        //
int1  fGrupoOK=0;        //
//int1  fGrupoMarcha=0;  //Flag de grupo en marcha
int1  fFalla=0;          //Alguna falla en transferencia pendiented de aceptar
int1  fRxOk=0;
int1  fRxOkLi=0;
int1  fRxOkGr=0;


//Flags fallas
int1  fTempOkGr=0;
int1  fTempOkLi=0;

//Flag cada un segundo
int1  pul_1seg=0;
int1  p_1seg=0;
int1  p_250ms=0;
int1  p_1seg0H=0;

//flag de teclado
int1  fTeclaOK=0;
int1  fOK=0;
int1  fArriba=0;
int1  fAbajo=0;
int1  fMenu=0;
int1  fTecOk=0;      //flag que la tecla ok se mantuvo apretada
int1  fActDisp=0;
int1  fActVal=0;
int1  fBackLight=0;  //Estado del backlight
int1  fGuardarEEPROM=0;
int1  fGuardaHS=0;
int1  fLeerHS=0;

int1  fAuxArrMan=0;
int1  fArrRem=0;
int1  fPareManual=0;
int1  fAuxFP=0;
int1  fPareGr=0;

int8  rMenu=0;
int8  rMaxMenu=46;//maxima cantidad de menues a mostrar segun grupo configurado
int8  rTmrInact=kTmrInact;//tiempo de inactividad del teclado

int8  rBackMenu=0;   //Menu en el que estaba antes de una falla

//Variables
//Datos de tensiones para deteccion de fallas
int8  VRminLi=0;
int8  VRmaxLi=0;
int8  VRminGr=0;
int8  VRmaxGr=0;
int8  rFaLi=0;       //Resumen de fallas tensiones de linea
int8  rFaGr=0;
//int8  rGanLi=0;
//int8  rGanGr=0;

//Ganancias de tensiones
signed int8  rGanLiR=0;
signed int8  rGanLiS=0;
signed int8  rGanLiT=0;
signed int8  rGanGrR=0;
signed int8  rGanGrS=0;
signed int8  rGanGrT=0;

//Tensiones leidas desde el pic 12f
int16  VRGr=0;
int16  VSGr=0;
int16  VTGr=0;
int16  VRLi=0;
int16  VSLi=0;
int16  VTLi=0;

//Datos crudos desde micro
int16  rDataRx1Li=0;
int16  rDataRx2Li=0;
int16  rDataRx3Li=0;
int16  rDataRx4Li=0;
int16  rDataRx1Gr=0;
int16  rDataRx2Gr=0;
int16  rDataRx3Gr=0;
int16  rDataRx4Gr=0;

//Registros de comunicacion
int16  rDataRx1=0;
int16  rDataRx2=0;
int16  rDataRx3=0;
int16  ChkRx=0;
int16  chksum=0;
//int8  rErrCGr=0;  //Errores en comunicacion con micro de grupo
//int8  rErrCLi=0;  //Errores en comunicacion con micro de Linea
int8   rHistLi=0;     //histeresis de linea
int8   rHistGr=0;     //histeresis de grupo

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
int8  rTipoPare=0;
int8  rDyCeLi=0;     //temporizador a cerrar linea
int8  preDyCeLi=0;     //2 segundos delay cierre de linea
int8  rTActV=kTActV;    //temporizador falla tension de grupo

//Linea
int8  tmrLiFa=7;     //antes 5 tiempo en segundos deteccion de falla tension de linea
int8  tmrLiOk=3;     //antes 5 Tiempo de linea OK
int8  preFVLi=7;     //antes 5 Preset de tiempo de falla tension de linea
int8  preLiOk=3;     //antes 5 Preset de tiempo de reotorno de linea OK
int8  TmrLiIni=1;    //Tiempo para cuando aparece la linea y no esta el grupo electrogeno

//Modo transferencia
int8  rModoTransf=0; ////Modo de funcionamiento de la transf Manual: 0,  o  Automatico: 1

//Salidas Digitales
int8  rfunSD1=0;
int8  rfunSD2=0;
int8  rfunSD3=0;

//Contador para tener un segundo
int8  tmr1seg=k1Seg; // contador para lograr 1 segundo 
int8  tmr1min=60;
int8  tmr1min1=60;
int8  tmr250ms=ktmr250; //timer 250ms


//Registros de configuracion
int8  rTempVar=0;       //Variable a modificar en los menues, contiene el valor de la variable a modificar
int8  rConfig=0;
#bit  fContactor = rConfig.0
#bit  fInterrup  = rConfig.1
int8  fSimuTransf= 0;
INT1  fArrManual=0;

int1  Temp_Bool=0;
int1  Temp_Bool1=0;
int1  tmpGrOk=0;

int   rTimeOutGr=kTimeOut;
int   rTimeOutLi=kTimeOut;

//Tiempos arranque grupo electrogeno
int1  OArrGR=0;      //Orden arranque grupo electrogeno
int1  fRunDelayArrGr=0;
//int1  fFinSecArr=0;  //Fin secuencia de arranque grupo
int1  fReleArr=0;    //Copia de la salida de arrnque grupo
int1  fPausa=0;
int1  fFallaGR=0;    //Falla al arranque del grupo electrogeno
int1  DyArrGROff=0;  //no hace la demora a conexion del grupo si falla la red con el grupo en marcha
int1  fEvPare=0;     //esta activa la EV de pare, corre tiempo de act. de valvula
int1  fSecArr=0;     //hizo la secuencia de arranque
int8  rTEvPare=kTEvPare;   //tiempo de activacion ev de pare
int8  ModoGr=0;      //Modo arranque grupo electrogeno 0:Rele continuo, 1: Pulso arranca, pulso para, 2:Manejo directo del burro
int8  intentosArr=6; //Intentos de arranque grupo en modo 2
int8  preIntArr=6;
int8  TArr=2;        //Tiempo de arrnaque grupo
int8  preTArr=2;     //Preset tiempo arranque grupo
int8  TPausa=8;      //Tiempo entre intentos de arranque
int8  preTPausa=8;   //Preset tiempo entre intentos
int8  tFallaGR=30;    //tiempo falla arranque en modo señal continua
int8  tPreFallaGr=1; //preset del anterior
int8  tFaGrMar=1;     //falla grupo sigue en marcha

// 14-11-13 agregado de registro hs de marcha
int16 rHsMarcha=0;   //Horas de marcha
int8  rMinMarcha=0;
int8  rSegMarcha=0;  //Segundos de marcha

int8  f0hora=k0hora;

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
int1 fFallVGr=0;     //falla grupo no genera las 3 fases

//confirmaciones filtradas
int1 fCO_LI=0;
int1 fCO_GR=0;

int8 rCO_LI=10;
int8 rCO_GR=10;
int8 rDyArrRem=10;

int8 rTPreCeb=0;  //tiempo preseteado de cebador 
int8 rTCeb=0;     //tiempo de cebador
int8 rCeb=0;      //usar cebador
#bit fCeb = rCeb.0

// Declaración de Funciones ///////////////////////////////////////////////////

void menu0(void);           
void menu01(void);
void menu1(void);
void menu2(void); 
void menu3(void);
void menu4(void); 
void menu5(void); 
void menu6(void); 
void menu7(void); 
void menu8(void); 
void menu9(void); 
void menu10(void); 
void menu11(void); 
void menu12(void); 
void menu13(void); 
void menu14(void); 
void menu15(void); 
void menu16(void); 
void menu17(void); 
void menu18(void);
void menu19(void);
void menu20(void); 
void menu21(void);
void menu22(void);
void menu23(void); 
void menu38(void); 
void menu39(void);
void menu40(void);
void menu41(void);
void menu42(void);
void menu43(void);
void menu44(void);
void menu45(void);
void menuManual(void);
void PantallaAlarmas(void);
void MenuParGr(void);

void vmenu0(void);           
void vmenu01(void);
void vmenu1(void);
void vmenu2(void); 
void vmenu3(void);
void vmenu4(void); 
void vmenu5(void); 
void vmenu6(void); 
void vmenu7(void); 
void vmenu8(void); 
void vmenu9(void); 
void vmenu10(void); 
void vmenu11(void); 
void vmenu12(void); 
void vmenu13(void); 
void vmenu14(void); 
void vmenu15(void); 
void vmenu16(void); 
void vmenu17(void); 
void vmenu18(void);
void vmenu19(void);
void vmenu20(void); 
void vmenu21(void);
void vmenu22(void);
void vmenu23(void); 
void vmenu38(void); 
void vmenu39(void);
void vmenu40(void);
void vmenu41(void);
void vmenu42(void);
void vmenu43(void);
void vmenu44(void);
void vmenu45(void);
void vmenuManual(void);
void vMenuParGr(void);

Void PantallaAlarmas(){
INT   CantAlarmas=1;
fActDisp=0;
lcd_putc('\f'); //limpiar display
lcd_gotoxy(1,1);
if (fFallaGR)printf(lcd_putc,"Falla Arr Grupo!"),cantAlarmas++;

lcd_gotoxy(1,CantAlarmas);
if (fFaAbLi)printf(lcd_putc,"Fa Aperutura Li!"),cantAlarmas++;

if (cantalarmas==3)return;
lcd_gotoxy(1,CantAlarmas);
if (fFaCeLi)printf(lcd_putc,"Fa Cierre Li!"),cantAlarmas++;

if (cantalarmas==3)return;
lcd_gotoxy(1,CantAlarmas);
if (fFaAbGr)printf(lcd_putc,"Fa Apertura. Gr!"),cantAlarmas++;

if (cantalarmas==3)return;
lcd_gotoxy(1,CantAlarmas);
if (fFaCeGr)printf(lcd_putc,"Fa Cierre. Gr!"),cantAlarmas++;

if (cantalarmas==3)return;
lcd_gotoxy(1,CantAlarmas);
if (fFaParoGr)printf(lcd_putc,"Falla Grupo!"),cantAlarmas++;

if (cantalarmas==3)return;
lcd_gotoxy(1,CantAlarmas);
if (fFaNoParGr)printf(lcd_putc,"Falla Pare Grupo"),cantAlarmas++;

if (cantalarmas==3)return;
lcd_gotoxy(1,CantAlarmas);
if (fFallVGr)printf(lcd_putc,"Falla Volt Grupo"),cantAlarmas++;

}

//Funciones
void Lee_Micro (int Micro){
int8  i=0;
int8  j=0;
int16  Dato1=0;
float  factor=0;
float vEscalado=0;
chksum=0;
for(j=0;j<4;j++){//3 datos + chksum
   //Recibe bit a bit
   i=0;
   for (i=0;i<16;i++){
         if (micro==1)output_low(pClkLi);     //led opto encendido pin en alto del 12f 
         else output_low(pClkGr);             //led opto encendido pin en alto del 12f
         delay_us(200);     
         //leo dato por bit de falla
         if (Micro==1){ //micro de linea
               output_high(pClkLi);            //led opto apagado pin en bajo del 12f
               delay_us(300);
               shift_right(&Dato1,2,input(pFallaLi));
               }
         if (Micro==0){ //micro de grupo
               output_high(pClkGr);   //led opto apagado pin en bajo del 12f
               delay_us(300);
               shift_right(&Dato1,2,input(pFallaGr));
               }
         }
         delay_us(200);
         //guardo valores crudos en cada registro segun micro
          if (Micro == 1){
            *((&rDataRx1Li) + (j))=Dato1;
            factor = *(&rGanLiR + j);
          }
          if (Micro == 0){
            *((&rDataRx1Gr) + (j))=Dato1;
            factor = (float)*(&rGanGrR + j);
          }
          if (j!=3){
               chksum=chksum+Dato1;
               //si el dato es mayor a 50 calcula, de otra manera, fuerza a cero
               if (Dato1 > 50)
                  {
                     vEscalado = (float)Dato1;
                     vEscalado = (-0.00007*(vEscalado*vEscalado))+(0.4156*vEscalado)-61.273;  //=-0,00007*R7^2+0,4156*R7-61,273
                     
                     vEscalado=((vEscalado * (factor / 1000))+vEscalado);
                  }
               else
                  vEscalado = 0;
            //transfiere dato a registro de medicion      
            *((&rDataRx1) + (j)) = (int16)(vEscalado);   //  /1000
         }
         else{
            ChkRx=chksum;
             if (micro==1){
               if (Dato1==chksum)fRxOkLi=1;
            }
            else{
               if (Dato1==chksum)fRxOkGr=1;
            }
         }
      }
}


void Verifica_VLi(void){
int16 VMax=(int16)vRmaxLi;
int16 VMin=(int16)vRminLi;
int16 VHMax=vMax - rHistLi;
int16 VHMin=vMin + rHistLi;

//activo bit de falla por alto si supero maximo
      if (VRLi > VMax)
         bit_set(rFaLi,0);
         
//si esta activa la falla, tiene que bajar de la histereis para salir         
      if (bit_test(rFaLi,0)){
         if(VRLi < VHMax)
            bit_clear(rFaLi,0);
      } 

//si supera la tension activa falla
      if (VSLi > VMax)
         bit_set(rFaLi,1);

//si esta activa la falla
      if (bit_test(rFaLi,1)){      
         if(VSLi < VHMax)
            bit_clear(rFaLi,1);
      }
 
//si la fase T supera la maxima configurada 
      if (VTLi > VMax)
         bit_set(rFaLi,2);

//si esta activada la falla
      if (bit_test(rFaLi,2)){      
         if(VTLi < VHMax)
            bit_clear(rFaLi,2);
      }
//////Alarmas por minimo//////////
// si baja del minimo, activa falla
      if (VRLi < VMin)
         bit_set(rFaLi,3);

//si esta activada la falla
      if (bit_test(rFaLi,3)){      
         if(VRLi > VHMin)
            bit_clear(rFaLi,3);
      }

//si baja del minimo, activo falla
      if (VSLi < VMin)
         bit_set(rFaLi,4);
//si esta activada la falla
      if (bit_test(rFaLi,4)){      
         if(VSLi > VHMin)
            bit_clear(rFaLi,4);
      }      

//si baja del minimo, activo falla
      if (VTLi < VRminLi)
         bit_set(rFaLi,5);
//si esta activada la falla
      if (bit_test(rFaLi,5)){      
         if(VTLi > VHMin)
            bit_clear(rFaLi,5);
      }
      
      /*if (!input(pFallaLi)) 
         bit_set(rFaLi,6);
      else
         bit_clear(rFaLi,6);   
      */
      if (rFaLi==0)
         fTempOkLi=1;
      else
         fTempOkLi=0;
}

void Verifica_VGr(void){

int16 VMax=(int16)vRmaxGr;
int16 VMin=(int16)vRminGr;
int16 VHMax=vMax - rHistGr;
int16 VHMin=vMin + rHistGr;

//activo bit de falla por alto si supero maximo
      if (VRGr > VMax)
         bit_set(rFaGr,0);
         
//si esta activa la falla, tiene que bajar de la histereis para salir         
      if (bit_test(rFaGr,0)){
         if(VRGr < VHMax)
            bit_clear(rFaGr,0);
      } 

//si supera la tension activa falla
      if (VSGr > VMax)
         bit_set(rFaGr,1);

//si esta activa la falla
      if (bit_test(rFaGr,1)){      
         if(VSGr < VHMax)
            bit_clear(rFaGr,1);
      }
 
//si la fase T supera la maxima configurada 
      if (VTGr > VMax)
         bit_set(rFaGr,2);

//si esta activada la falla
      if (bit_test(rFaGr,2)){      
         if(VTGr < VHMax)
            bit_clear(rFaGr,2);
      }
      
//////Alarmas por minimo//////////
// si baja del minimo, activa falla
      if (VRGr < VMin)
         bit_set(rFaGr,3);

//si esta activada la falla
      if (bit_test(rFaGr,3)){      
         if(VRGr > VHMin)
            bit_clear(rFaGr,3);
      }

//si baja del minimo, activo falla
      if (VSGr < VMin)
         bit_set(rFaGr,4);
//si esta activada la falla
      if (bit_test(rFaGr,4)){      
         if(VSGr > VHMin)
            bit_clear(rFaGr,4);
      }      

//si baja del minimo, activo falla
      if (VTGr < VRminGr)
         bit_set(rFaGr,5);
//si esta activada la falla
      if (bit_test(rFaGr,5)){      
         if(VTGr > VHMin)
            bit_clear(rFaGr,5);
      }
      bit_clear(rFaGr,7);
      if (rFaGr==0)
         fTempOkGr=1;
      else
         fTempOkGr=0;
         
      if ((VRGr > VRminGr) || (VSGr > VRminGr) || (VTGr > VRminGr))
         bit_set(rFaGr,7);
      else
         bit_clear(rFaGr,7);
               
}


//++++++++++++++++++ ESCANEO DE TECLADO ++++++++++++++++++++++++++++++
void  ScanTeclado(){
//revision para placa nueva con teclado separado de placa principal

   int rTempTec;                          //Temporal teclado
   output_low(LCD_ENABLE_PIN);            //Desactivo lcd
   set_tris_b (0b11111000);               //configuro entradas para teclas.
   rTempTec=input_B();
   rTempTec=swap(rTempTec);
   rTempTec=rTempTec & 0b00001111;
   
      if (rTempTec!=0) //si=0 ninguna tecla apretada... me voy
         {
         rInmunidad--;
            if(rInmunidad==0 && fTeclaOK==0){
               rInmunidad=kInmuni;
               rTmrInact=kTmrInact;
               fBackLight=1;
               switch (rTempTec){
                           case 1: fOK=1;       //14
                                   if (rMenu!=0) fTeclaOK=1;
                                   break;
                           case 2: fArriba=1;
                                   rInmunidad=kInmuniRep;//idem anterior 
                                   break;
                           case 8: fMenu=1;
                                   fTeclaOK=1;
                                   break;
                           case 4: fAbajo=1;
                                   rInmunidad=kInmuniRep;//cargo inmunidad para repeticion de tecla
                                   break;
                           case 9: IF (rMenu==0){
                                       rMenu=38;//menu ajuste de tensiones de linea
                                       fTeclaOK=1;
                                       fActDisp=1;
                                   }
                                   break;
                           default:fTeclaOK=0;
                                   rInmunidad=kInmuni;
                                   fOK=0;
                                   fMenu=0;
                                   fAbajo=0;
                                   fArriba=0;
                                   fTecOk=0;
                                   break;
                                 }
               }
            }
      else{
      rInmunidad=kInmuni;
      fTeclaOK=0;
      rTmrOK=kTmrOk;
      f0hora=k0hora;
      }
      if (fOK==1 && rMenu==0){
         if (--rTmrOk==0){
             rTmrOK=kTmrOk;
             fTecOk=1;
         }
      }
}

void leeHorasEE(){
int16 punt=0;
punt=&rHsMarcha;
      rMinMarcha=read_EEPROM(kMinMarcha);
      *punt=read_EEPROM(kHsMarcha);
      *(punt+1)=read_EEPROM(kHsMarcha+1);
}

void escHorasEE(){
int16 punt=0;
punt=&rHsMarcha;
     write_EEPROM(kMinMarcha,rMinMarcha); 
     write_EEPROM(kHsMarcha,*punt);
     write_EEPROM(kHsMarcha+1,*(punt+1));
}

void RecuperaEEPROM(){

      VRminLi=read_EEPROM(kVminLi);       //tension de falla por bajo
      VRmaxLi=read_EEPROM(kVmaxLi);       //tension de falla por alto

      VRminGr=read_EEPROM(kVminGr);       //tension de falla por bajo
      VRmaxGr=read_EEPROM(kVmaxGr);       //tension de falla por alto
      
      prePareGr=read_EEPROM(kTPareGR);    //Tiempo de marcha grupo despues de retorno de la linea
      tmrPareGr=prePareGr;
      
      preLiOk=read_EEPROM(kTRetLI);     //Tiempo de retorno de la linea despues de fallo
      tmrLiOk=preLiOk;
      
      preDyArrGr=read_EEPROM(kDlyArrGr);   //Demora al arranque del grupo ante falla de linea 
      tmrDyArrGr=preDyArrGr;
      
      pretmrConGR=read_EEPROM(kTConGR);     //Tiempo de conexion del grupo ante falla de linea
      tmrConGR=pretmrConGR;
      
      preIntArr=read_EEPROM(kIntArr);     //Intentos de arranque del grupo
      intentosArr=preIntArr;
      
      preTArr=read_EEPROM(kTArrGR);     //tiempo que le da al burro en modo 2 el arranque de grupo
      TArr=preTArr;
      
      preTPausa=read_EEPROM(kTPausa);     //Tiempo entre intento e intento de arranque
      TPausa=preTPausa;
      
      ModoGr=read_EEPROM(kModoGr);     //Modo arranque de grupo
      rConfig=read_EEPROM(kAcc);        //Tipo de accionamiento (contactores o interruptores
      
      rModoTransf=read_EEPROM(kModoTr);     //Modo funcionamiento transferencia
      
      fSimuTransf=0;                   //Desactivar siempre el test al apagar la central read_EEPROM(kTestTransf);     //Modo test
      
      preCalBuj=read_EEPROM(kCalBujia);      //tiempo calentamiento bujia
      tmrCalBuj=preCalBuj;
      
      tPreFallaGr=read_EEPROM(kFallaGr);      //tiempo calentamiento bujia
      tfallaGr = tPreFallaGr;
 
//recupera multiplicadores de linea y grupo
      rGanLiR = read_EEPROM(kGanLiR);      //Multiplicador
      rGanLiS = read_EEPROM(kGanLiS);      //Multiplicador
      rGanLiT = read_EEPROM(kGanLiT);      //Multiplicador
      rGanGrR = read_EEPROM(kGanGrR);      //Multiplicador
      rGanGrS = read_EEPROM(kGanGrS);      //Multiplicador
      rGanGrT = read_EEPROM(kGanGrT);      //Multiplicador
      
      rTPreCeb=read_EEPROM(kCebador);    //Tiempo Cebado
      if (rTPreCeb==0)fCeb=0;            //seteo o reset flag segun tiempo
      else fCeb=1;
      
      rTipoPare=read_EEPROM(kTipoPare);    //Tipo de pare de grupo
      rTEvPare=kTEvPare; 
      
      preDyCeLi=read_EEPROM(kDyLi);    //Tiempo entre sacar contactor de gr y meter de linea
      rDyCeLi = preDyCeLi;
      
      rHistLi=read_EEPROM(kHistLi);     //histeresis de linea
      rHistGr=read_EEPROM(kHistGr);     //histeresis de grupo
}

void GuardarEEPROM(){

      write_EEPROM(kVminLi,VRminLi);       //tension de falla por bajo
      write_EEPROM(kVmaxLi,VRmaxLi);       //tension de falla por alto

      write_EEPROM(kVminGr,VRminGr);       //tension de falla por bajo
      write_EEPROM(kVmaxGr,VRmaxGr);       //tension de falla por alto
      
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
      
      write_EEPROM(kModoTr,rModoTransf);
      
      write_EEPROM(kTipoPare,rTipoPare);     //Modo test  
      
      write_EEPROM(kCalBujia,preCalBuj);     //Tiempo calentamiento bujia
 
      write_EEPROM(kFallaGr ,tPreFallaGr);     //Tiempo falla grupo cuando esta en modo continuo el arr del grupo
      
      write_EEPROM(kGanLiR ,rGanliR);
      write_EEPROM(kGanLiS ,rGanliS);
      write_EEPROM(kGanLiT ,rGanliT);
      
      write_EEPROM(kGanGrR ,rGanGrR);
      write_EEPROM(kGanGrS ,rGanGrS);      
      write_EEPROM(kGanGrT ,rGanGrT);
      
      write_EEPROM(kCebador ,rTPreCeb);
      
      write_EEPROM(kDyLi ,preDyCeLi);
      
      write_EEPROM(kHistLi ,rHistLi);
      write_EEPROM(kHistGr ,rHistGr);
}

#include <menu.c>
#include <valmenu.c>
#include <teclado.c>

// INTERRUPCIONES /////////////////////////////////////////////////////////////

#int_RTCC
void RTCC_isr() {                      // Interrupción Timer 0
  
if (--tmr1seg==0){
   pul_1seg=1;
   tmr1seg=k1Seg;// 10mhz - 76; 
         //Deteccion de falla de grupo
         IF(!fTempOkGr){                  //Si esta en falla el grupo
            if ((--tmrGrFa==0)){      //Decremento temporizador
               fGrupoOk=0;                      //Fin temporizador  Setea falla grupo
               tmrGrFa=preFVGr;                 //cargo valor en el temporizador
            }
         }
         else{
               tmrGrFa=preFVGr;
         }
         
         IF(fTempOkGr){      //Grupo ok
            if ((--tmrGrOk==0)){
               fGrupoOk=1;
               tmrGrOk=preGrOk;
            }
         }
         else{
               tmrGrOk=preGrOk;
         }
         
         if(!fTempOkLi | (fSimuTransf==1)){     //Si esta en falla la linea o estoy simulando la transferencia
            if ((--tmrLiFa==0)){
               fLineaOk=0;                         //flag de falla
            }
         }
         else{
               tmrLiFa=preFVli;
         }
         
         if(fTempOkLi & (fSimuTransf==0)){           // & fGrupoOk   Si Linea Ok y grupo conectado
            if ((--TmrLiOk==0)){       //Retorno de linea despues de falla
               fLineaOk=1;                         //y que estoy alimentando desde grupo
               tmrLiOk=preLiOk;
            }
         }
         else{
               tmrLiOK=preLiOk;
         }
         
         if (fTempOkLi & !input(pCO_GR)){    //Si LineaOk y grupo desconectado pongo menos tiempo para retorno de la linea
            if ((--TmrLiIni==0) & !FlineaOk){      //para la primera vez que se conecta el equipo no esperar a que pase
                  fLineaOk=1;                      //Todo el tiempo seteado
                  tmrLiIni=1;
            }
         }
         if ((--rTmrInact==0) & (fBackLight==1)){
            rTmrInact=kTmrInact;
            fBackLight=0;
            rMenu=0;
            fActDisp=1;
            fGuardarEEPROM=1;  
         }
         //CUENTA HORAS VER 5.20
         if (fGrupoOK)rSegMarcha++;
            if (rSegMarcha == 60){
               rSegMarcha = 0;
               fGuardaHS=1;
               if (++rMinMarcha==60){
                  rMinMarcha=0;
                  rHsMarcha++;
               }          
            }
          }
ScanTeclado();
//Timer solo para muestreo
if (--tmr250ms==0){
   p_250ms=1;
   tmr250ms=ktmr250;
}

if (!fArrRem){
   if (input(pArrRem)){
      if(rDyArrRem--==0){
         rDyArrRem=kDyArrRem;
         fArrRem=1;
      }
   }
   else
         rDyArrRem=kDyArrRem;
}
if (fArrRem){
   if (!input(pArrRem)){
      if(rDyArrRem--==0){
         rDyArrRem=kDyArrRem;
         fArrRem=0;
      }
   }
   else
         rDyArrRem=kDyArrRem;
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



// FUNCION PRINCIPAL BUCLE ////////////////////////////////////////////////////

void main(){
//++++++++++++++++++Configura puertos analogicos++++++++++++++++++++++++++++++
setup_timer_0(RTCC_INTERNAL|RTCC_DIV_128|T0_8_BIT);//

OUTPUT_A(0);
OUTPUT_B(0);
OUTPUT_C(0);

//Configuracion de puertos   
set_tris_a(0b00010010);
set_tris_b(0b00001000);
PORT_B_PULLUPS(false);
set_tris_c(0b11000000);

//Configuracion Analogicas
setup_adc(NO_ANALOGS|ADC_OFF);
SETUP_DAC(DAC_OFF);
setup_comparator(NC_NC_NC_NC);            //no comparadores
SETUP_DAC(DAC_OFF);
setup_vref(VREF_OFF); 
////seteo interrupciones
   enable_interrupts(int_rtcc);                  // Habilita Interrupción TMR0
   enable_interrupts(global);                    // Habilita interrupciones

//Seteos iniciales
fDataOkGr=0;         //Falta enviar datos al micro de grupo
fDataOkLi=0;         //Falta enviar datos al micro de linea
output_high(pClkLi);  //Pin en bajo del clock micro linea
output_high(pClkGr);  //Pin en bajo clock micro grupo
//output_high(pTx);    //cero para el 12f
rInmunidad=kInmuni;
fArrManual=0;
tmrLiIni=1;          //Tiempo para conexion inicial del sistema

lcd_init();          //Inicio LCD
rMenu=0;             //Pantalla ppal
fActDisp=1;          //Escribir pantalla
fActVal=1;          //Escribir pantalla
fFallaGR=0;          //reset falla grupo

output_high(pBacklight);   //prendo backlight
fBackLight=1;
output_HIGH(pClkLi);
output_HIGH(pClkGr);

lcd_putc('\f'); //limpiar display
lcd_gotoxy(4,1);
printf(lcd_putc,"ControlARG");
lcd_gotoxy(1,2);
printf(lcd_putc,"TA 380   Fw:7.02");
RecuperaEEPROM();    //Leo parametros almacenados en la EEPROM
fLeerHS=1;
delay_ms(3500);

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

if(fInterrup & input(pCO_LI)){//si esta en modo interruptor y esta cerrado el interruptor de linea
//leo micro de linea para ver como estan las tensiones para no abrir el interruptor
   if (input(pFallaLi)){  //si esta vivo el micro de linea
     lee_micro(1);
         if(fRxOkLi){
            //fRxOk=0;
            VRLi=rDataRx1;
            VsLi=rDataRx2;
            VtLi=rDataRx3;
         } 
   }
   else{
        VRLi=0;
        VsLi=0;
        VtLi=0; 
        fRxOkLi=0;
   }
}
Verifica_VLi();

if(fInterrup & input(pCO_GR)){//si esta en modo interruptor y esta cerrado el interruptor de Grupo
//leo micro de linea para ver como estan las tensiones para no abrir el interruptor
   if (input(pFallaGr)){  //si esta vivo el micro de grupo
     lee_micro(0);
         if(fRxOkGr){
            fRxOkGr=0;
            VRGr=rDataRx1;
            VsGr=rDataRx2;
            VrGr=rDataRx3;
         } 
   }
   else{
        VRGr=0;
        VsGr=0;
        VrGr=0;   
        fRxOkGr=0;
   }
}
Verifica_VGr();

////////////////////////////////////////////////////////
//Bucle principal
////////////////////////////////////////////////////////
while(true){
if (pul_1seg){
      p_1seg=1;
      pul_1seg=0;
      fActVal=1;
      p_1seg0H=1;
}

if(fActVal)
{
   disable_interrupts(global);                    // Habilita interrupciones
   vMenu();
   enable_interrupts(global);                    // Habilita interrupciones
}
if(fActDisp)
{
    disable_interrupts(global);                    // Habilita interrupciones
    Menu();
    fActVal=1;
    enable_interrupts(global);                    // Habilita interrupciones
}


//--------------------------------------------------------------------------------------------
//Lectura Tensiones Micro linea
//--------------------------------------------------------------------------------------------
if(p_250ms){ //antes p_1seg aumento tiempo de muestreo
   if (input(pFallaLi)){
         lee_micro(1);
         if(fRxOkLi){
            //fRxOkLi=0;
            VRLi=rDataRx1;
            VsLi=rDataRx2;
            VtLi=rDataRx3;
         }
   }
   else{
        VRLi=0;
        VsLi=0;
        VtLi=0; 
        fRxOkLi=0;
   }
}

//--------------------------------------------------------------------------------------------
//Lectura Tensiones Micro Grupo
//--------------------------------------------------------------------------------------------
if(p_250ms){ //antes p_1seg aumento tiempo de muestreo
   if (input(pFallaGr)){
      lee_micro(0); 
      if(fRxOkGr){ 
         //fRxOkGr=0;
         VRGr=rDataRx1;
         VsGr=rDataRx2;
         VtGr=rDataRx3;
      } 
   }
      else{
        VrGr=0;
        VsGr=0;
        VtGr=0;   
        fRxOkGr=0;
   }   
}
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
//                                OPERACION DE INTERRUPTORES
//############################################################################################

//--------------------------------------------------------------------------------------------
//Apertura interruptor linea
//--------------------------------------------------------------------------------------------
if(!fLineaOK & (rModoTransf==1)){                           //Si falla la linea y esta en auto y no esta la salida de cerrar
//si esta config para trabajar con contactor-abre contactor
   if(fContactor){
      fOCLi=0;                       //Abro rele de red
   }
   
//Si esta config para interruptores-energiza bobina de apertura mientras este el int cerrado
   if(fInterrup & !fOCLi){
      fOALi=1;
   }
//Si ya se abrio el interruptor-desenergizo bobina
   if(fInterrup & !input(pCO_LI)){
      fOALi=0;
   }
}
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//Cierre interruptor de linea
//--------------------------------------------------------------------------------------------
if(fLineaOK  & !input(pCo_GR) & (rModoTransf==1)& !fOALi){    //Si esta ok la linea y no esta metido el grupo electrogeno y esta en auto y no esta la salida de abrir
      if(rDyCeLi==0){
             fOCLi=1;
             rDyCeLi=preDyCeLi;
      }
      else{
         if (p_1seg & (fOCLi==0)) {
            if (--rDyCeLi==0){
                  fOCLi=1;
                  rDyCeLi=preDyCeLi;
            }
         }
      }
      if (!fArrManual & !fArrRem)
            OArrGR=0;                  //conteo para apagar el grupo
         
      //Si esta config con interruptor y ya se Cerro el interruptor-desenergizo bobina
      if(fInterrup & input(pCO_LI)) {
            fOCLi=0;
      }
}
//-------------------------------------------------------------------------------------------- 


//--------------------------------------------------------------------------------------------
//deteccion de falla de operacion de contactor
//--------------------------------------------------------------------------------------------
   if(fOCLi & !fCO_LI & !fFaCeLi & (rModoTransf==1) & fLineaOk) //si esta la orden de cerrar y no esta la confirmacion
      Temp_Bool=1;                                              //agregdado que este la linea ok para detectar falla 
   else
      Temp_Bool=0;
      
  //agrego que el sistema este en automatico para que controle la falla    
  if(((fContactor & !fOCLi)|(fInterrup & fOALi)) & fCO_LI & !fFaAbLi & (rModoTransf==1))  //si esta configurado contactor y no esta lo orden de cerrar y esta la confirmacion del contactor     
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
if((fLineaOk |!fGrupoOK) & (rModoTransf==1)){    // Si retorna la linea y esta el contactor de grupo
//Si esta configurado con interruptor
   if(fContactor){
      fOCGr =0;       //Abro contactor de grupo
   }
//Si esta configurado con interruptor, energizo bobina de apertura   
   if(fInterrup & !fOCGr){
      fOAGr =1;
   }
//Si habrio el interruptor, desenergizo bobina
   if(fInterrup & !input(pCO_GR)){
      fOAGr=0;
   }
}
//--------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------
//Cierre interrutpor de grupo
//--------------------------------------------------------------------------------------------
if(fGrupoOk & !fLineaOk & (rModoTransf==1) & !fOAGr){   //Si esta en falla la linea y listo el grupo y no esta conectado
   if(p_1seg | (DyArrGROff==1)){
      if((tmrConGR--==0) | (DyArrGROff==1)){      //demora al cierre de interruptor de grupo
               DyArrGROff=1;  // no hacer demora si ya esta en marcha el grupo y vuelve a fallar la red
               if(!input(pCo_LI)){
                  fOCGr =1;                      //Cierre interruptor de grupo
                  }
               }
   }
//Si esta configurado para interruptor, y ya cerro interruptor, desenergiza bobina
       if(fInterrup & input(pCO_GR)){
            fOCGr =0;
       }
}
else
   tmrConGR=pretmrConGR;
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------            
//deteccion de falla de operacion de contactor
   if(fOCGr & !fCO_GR & !fFaCeGr & (rModoTransf==1)& fGrupoOk) //si esta la orden de cerrar y no esta la confirmacion
      Temp_Bool=1;                                             //agregado que este el grupo OK para que de la falla
   else
      Temp_Bool=0;
  
  //Agregado de que este en automatico para que informe falla
  if(((fContactor & !fOCGr)|(fInterrup & fOAGr)) & fCO_GR & !fFaAbGr & (rModoTransf==1))  //si esta configurado contactor y no esta lo orden de cerrar y esta la confirmacion del contactor     
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
//Orden de marcha del grupo electrogeno
//--------------------------------------------------------------------------------------------
if (((!fLineaOK  & !fFallaGR) | fCO_GR) & (rModoTransf==1) & !OArrGr)
      fRunDelayArrGr=1;
else
      fRunDelayArrGr=0;

if(fRunDelayArrGr | fAuxArrMan){                  //Si hay que arrancar el grupo
//Delay al arranque del grupo electrogeno
   if(p_1seg | fAuxArrMan){
      if((--tmrDyArrGr==0)| fAuxArrMan){                            //si paso el tiempo de demora al arranque del grupo
            OArrGR=1;                   //Inicio secuencia arranque grupo
            //Carga tiempos de arranque grupo
            TArr=preTArr;                             //Tiempo de arrnaque grupo
            TPausa=preTPausa;                         //Tiempo entre intentos de arranque
            fPausa=0;
            intentosArr=preIntArr;
            tmrDyArrGr=preDyArrGr;
            tmrCalBuj=preCalBuj;
            rTCeb=rTPreCeb;
            pasoArr=1;
            fSecArr=1; 
            fAuxArrMan=0;
            fPareGr=0;
            }
   }
}
//SI ESTA EN AUTOMATICO, ARRANCARON MANUALMENTE EL GRUPO
//Y SE PRODUCE UNA FALLA DE LINEA, BAJA FLAG MANUAL DE ARRANQUE DE GRUPO
//Y DEJA EN MARCHA EL GRUPO
if(fArrManual & (rModoTransf==1) & !fLineaOK){
   fArrManual=0;
   fAuxFP=0;
}

//FLANCO DE LA ORDEN DE MARCHA
if((fArrManual) & !fAuxFP) fAuxArrMan=1;

if((fArrRem) & !fAuxFP) fAuxArrMan=1;
//FLANCO DE LA ORDEN DE PARADA

if (!fArrManual & (!fArrRem & !input(pCo_GR)) & fAuxFP) fPareManual=1,OArrGR=0;

//IGUALO FLANCO CON ORDEN
if(fArrManual | fArrRem) fAuxFP=1;
else fAuxFP=0;

//--------------------------------------------------------------------------------------------
//Puesta en marcha del grupo electrogeno segun tipo seteado
//--------------------------------------------------------------------------------------------
if (OArrGR & !fFaParoGr){ //& (rModoTransf==1) eliminado para permitir el arranque del grupo con la TA en manual
   switch (ModoGr){
     case 1:
         output_high(pReleArrGr);
         if (!fGrupoOk){
            if (p_1seg){
               if(--tmr1min1==0){
                  tmr1min1=60;
                  if(tFallaGR--==0){
                     fFalla=1;
                     OArrGR=0; 
                     fFallaGR=1;
                     tFallaGR=tPreFallaGr;
                  }  
               }
            }
         }
         else
            tFallaGR=tPreFallaGr;
            
            break;
     case 2:   //arranque por motor de arranque
               switch (pasoArr) {
                  case 1://paso el tiempo de demora al arranque, pongo en contacto el grupo
                        output_high(pReleParGr);         //pone en contacto el grupo
                        pasoArr=2;                       //precalentamiento
                        break;
                  case 2://demora de precalentamiento
                        if (p_1seg){
                           if(tmrCalBuj--==0)
                              pasoArr=3;
                        }
                        break;                       
                  case 3://le da al burro el tiempo configurado
                        TArr=preTArr; //carga tiempo de arranque
                        rTCeb=rTPreCeb;
                        if (!fGrupoOk & !fFallaGR){
                              output_high(pReleArrGr);
                              if (fCeb)output_high(pAlarm);    //si esta configurado el cebador, lo activa
                        }
                        pasoArr=4;
                        break;
                  case 4://cuenta tiempo de actuacion de arranque y cebador
                         if (p_1seg){ //tiempo de accionamiento del burro
                              if(tArr--==0){
                                 output_low(pReleArrGr);
                                 TPausa=preTPausa;
                                 pasoArr=5;  //hacer pausa en arranque
                              }
                              if(fCeb){
                                 if (rTCeb--==0){//si termino tiempo de cebado
                                    output_low(pAlarm); 
                                    rTCeb=rTPreCeb;
                                 }
                              }
                        }
                        if (fGrupoOk | bit_test(rFaGr,7)){
                           output_low(pReleArrGr);
                           TPausa=preTPausa;
                           pasoArr=5;  //hacer pausa en arranque
                        }
                        break;
                  case 5://Pausa entre arranque y arranque
                        if (fGrupoOk | bit_test(rFaGr,7)){
                              output_low(pReleArrGr);
                              pasoArr=6;
                        }
                        
                        if (p_1seg){ //tiempo de pausa
                           if(tPausa--==0){//si paso el tiempo de pausa
                              output_low(pReleArrGr);
                              if(intentosArr--!=0){    //si no hizo todos los intentos de arranque
                                 pasoArr=3;
                                 TArr=preTArr;
                              }
                              else{
                              //error no arranco despues de todos los intentos 
                                 OArrGR=0;             //fin secuencia de arranque.
                                 intentosArr=preIntArr;
                                 fFallaGR=1;
                                 fFalla=1;
                                 output_low(pReleParGr);
                                 pasoArr=1;  //prepara para cuando resetea alarma ya hace el arranque
                                 tmrCalBuj=preCalBuj;
                              }
                           }
                               if(fCeb){
                                 if (rTCeb--==0){//si termino tiempo de cebado
                                    output_low(pAlarm); 
                                    rTCeb=rTPreCeb;
                                 }
                              } 
                        }
                        break;
                  case 6://grupo en marcha
                  //chequeo que aparezcan todas las tensiones
                        if (bit_test(rFaGr,7) && !fGrupoOk){
                           if(p_1seg){
                              if(rTActV--==0){     //llego temporizador a cero, falla tension grupo
                                 rTActV=kTActV;    //reset temporizador
                                 fFalla=1;         //indico falla
                                 fFallVGr=1;
                              }
                           }
                        }
                        else
                           rTActV=kTActV;    //reset temporizador
                           
                        if(fCeb){//si esta activado el cebador
                           //decrementa tiempo de accionamiento de cebador
                           if(p_1seg){
                              if (rTCeb--==0){//si termino tiempo de cebado
                                 output_low(pAlarm); 
                                 rTCeb=rTPreCeb;
                              }
                           }
                        }
                        break;
               }  
            break;//fin case de tipo de grupo
   }
}//Fin if arranque grupo


//--------------------------------------------------------------------------------------------
//Parada del grupo electrogeno segun tipo seteado
//--------------------------------------------------------------------------------------------
if (!OArrGR & fSecArr){ //& (rModoTransf==1) eliminado para permitir operacion con la TA en manual
   if(p_1seg){
      if( --tmr1min==0 | fPareManual){
            tmr1min=60;
            if(--tmrPareGr==0 | fPareManual){ 
                  fPareGr=1;
                  //tmrPareGr=prePareGr; //comentado porque recargaba el tiempo de parada y no daba nunca la falla de grupo no paro
                  DyArrGROff=0;     //VUELVE A ACTIVAR DEMORA A LA CONEXION DEL GRUPO
                  if (rTipoPare==0){//tipop de pare por contacto
                     if(fCeb)//si esta activado el cebador
                        output_low(pAlarm);   
                     if (ModoGr==1)
                        output_low(pReleArrGr);
                     else if(ModoGr==2){
                        output_low(pReleParGr); //Abre rele de contacto de grupo
                     }
                     fSecArr=0; 
                     fPareManual=0;
                  }
                  else if (rTipoPare==1){//tipo pare por electrovalvula
                     output_high(pAlarm);
                     fEvPare=1;
                  }
            }      
      }
      if (fEvPare){
         if (--rTEvPare==0){
            fEvPare=0;
            rTEvPare=kTEvPare; 
            output_low(pAlarm);
            output_low(pReleParGr);
            fSecArr=0; 
            fPareManual=0;
         }
      }
   }
     //Si hay orden de parar y esta haciendo el encendido del grupo
     //en modo 2, corta todo....
      if (modoGR == 2){
         output_low(pReleArrGr);
         fReleArr=0;
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
if(OArrGR & (rModoTransf==1)){
   if(!fGrupoOk & tmpGrOk){
         fFalla=1,
         fFaParoGr=1;   //paro solo el grupo
   }
}
tmpGrOk=fGrupoOk;


//Falla grupo no paró
if((!OArrGR & fPareGr  & fGrupoOk & (rModoTransf==1))){ //(tmrPareGr==0)
   if (p_1seg){
         if(--tmr1min1==0){
            tmr1min1=60;
            //if (--tFaGrMar==0){
               fPareGr=0;
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

//rMaxMenu=45; 
TecMenu();
if(fFalla & (rMenu!=60)) {   //Si hay falla paso a pantalla fallas ANTES 44
   rBackmenu=rMenu;
   rMenu=60;
   fActDisp=1;
}
if (fFalla & fActDisp)
      pantallaalarmas();
      
if(fFalla){
  if (p_1seg) output_toggle (pBacklight);
  if (!fCeb && rTipoPare==0) output_high(pAlarm); 
}
else{
   //output_high (pBacklight);
   if (fBackLight) output_high (pBacklight);
   else output_low (pBacklight);
   if (!fCeb && rTipoPare==0) output_low(pAlarm);
}


if (p_1seg)p_1seg=0;

if (fGuardaHS){
   fGuardaHS=0;
   escHorasEE();
}  
if (fLeerHS){
   fLeerHS=0;
   leeHorasEE();
}
if (fGuardarEEPROM){
   fGuardarEEPROM=0;
   guardarEEPROM();
}
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//ESCRITURA SALIDAS CONTACTORES O INTERRUPTORES
//--------------------------------------------------------------------------------------------
//Cierre+
if (fOCLi)
      output_high(pCE_LI);
else
      output_low(pCE_LI);
//Apertura
if (fOALi)
      output_high(pAB_LI);
else
      output_low(pAB_LI);


//Cierre
if (fOCGr)
      output_high(pCE_GR);
else
      output_low(pCE_GR);
//Apertura
if (fOAGr)
      output_high(pAB_Gr);
else
      output_low(pAB_Gr);
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//Verifica si presiono la tecla ok mucho tiempo
//--------------------------------------------------------------------------------------------
if (fTecOk){
fTecOk=0;
   if(rModoTransf==0)
      rModoTransf=1;
   else 
      rModoTransf=0;
      fGuardarEEPROM=1;
//guardarEEPROM();
}
} //Fin buble infinito

}//Fin main
