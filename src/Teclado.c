void TecMenu(){
//////  PANTALLA PRINCIPAL   ///////
   if (rMenu==0){
      if(fOk) {
         fOk=0;
      }
      
      if(fArriba ){
         fArriba=0;
      }
      
      if(fAbajo)fAbajo=0;

      if(fMenu){
         fMenu=0;
         rMenu=1;
         fActDisp=1;
         rTempVar=rModoTransf;
         break;
      }
   }

//PRESIONO LA TECLA MENU
   if(rMenu>0 & (rMenu!=60) ){
      if(fMenu){
            fMenu=0;
            fActDisp=1;
            if(++rMenu==24)//rMaxMenu)      //rmaxmenu maxima cantidad de menues a mostrar, varia segun tipo de grupo seleccionado
                  {rMenu=0;
                     fGuardarEEPROM=1;
                  }
            if (rMenu > 45)
               rMenu=0;
            switch   (rMenu){
                  case 2:  rTempVar=VRminLi;
                           break;
                  case 3:  rTempvar=VRmaxLi;
                           break;
                  case 4:  rTempVar=VRminGr;
                           break;
                  case 5:  rTempvar=VRmaxGr;
                           break;                           
                  case 6:  rTempVar=preDyArrGr;
                           break;
                  case 7:  rTempVar=pretmrConGR;
                           break;
                  case 8:  rTempVar=prePareGr;
                           break;
                  case 9:  rTempVar=preLiOk;
                           break;
                  case 10: rTempVar=rConfig;
                           break;
                  case 11: rTempVar=ModoGR;
                           break;
                  case 12: rTempVar=preCalBuj;
                           break;                           
                  case 13: rTempVar=preTArr;
                           break;
                  case 14: rTempVar=preTPausa;
                           break;   
                  case 15: rTempVar=preIntArr;
                           break;  
                  case 16: rTempVar=fArrManual;////////////////////////////
                           break;   
                  case 17: rTempVar=tPreFallaGr;
                           break;
                  case 18: rTempVar=rTPreCeb;
                           break;
                  case 20: rTempVar=rTipoPare;
                           break;          
                  case 21: rTempVar=rfunSD1;
                           break;   
                  case 22: rTempVar=preDyCeLi;
                           break; 
                  case 23: //rTempVar=rfunSD3;
                           break;                           
                  case 38: rMenu=38; 
                           break;
                  case 39: rMenu=39;
                           break;
                  case 40: rMenu=40; 
                           break;
                  case 41: rMenu=41;
                           break;  
                  case 42: rMenu=42;
                           break;     
                  case 43: rMenu=43;
                           break;                              
                  case 44: rMenu=44;
                           break; 
                  case 45: rMenu=45;
                           break;                            
                  default: break;
            }
      }
      //Teclas arriba o abajo
      if((fArriba|fAbajo)){
                  if(fArriba){
                           rTempVar++;         
                  }
                  if(fAbajo){
                           rTempVar--;
                  }
            switch   (rMenu){
                  case 1:  //Modo manual o automatico
                           if (rtempvar >1)rtempvar=1;
                           if (rtempvar ==255)rtempvar=0;
                           break;
                           //Regulacion de tension minima linea
                  case 2:  if (rTempvar < 180)rTempVar=180;
                           if (rtempvar > 210)rtempVar=210;
                           break;
                           //Regulacion de tension maxima linea
                  case 3:  if (rTempvar > 250)rTempVar=250;
                           if (rtempvar < 225)rtempvar=225;
                           break;
                           //Regulacion de tension minima grupo
                  case 4:  if (rTempvar < 180)rTempVar=180;
                           if (rtempvar > 210)rtempVar=210;
                           break;
                           //Regulacion de tension maxima grupo
                  case 5:  if (rTempvar > 250)rTempVar=250;
                           if (rtempvar < 225)rtempvar=225;
                           break;                           
                           //Retardo arranque de grupo
                  case 6:  preDyArrGr=rTempvar;
                           break;
                           //Retardo conexion de grupo
                  case 7:  pretmrConGR=rTempvar;
                           tmrConGR=rTempvar;
                           break;
                           //Tiempo parada de grupo
                  case 8:  prePareGr=rTempvar;
                           tmrPareGr=rTempvar;
                           break;   
                           //Tiempo deteccion de linea OK
                  case 9:  preLiOk=rTempvar;
                           break;
                           //Tipo de accionamiento
                  case 10: if (rTempVar>2)rtempvar=2;
                           if (rTempVar<1)rtempvar=1;
                           break;
                           //Modo grupo electrogeno 
                  case 11: if (rtempvar < 1) rtempvar=1;
                           if (rtempvar >2)rtempvar=2;
                           break;
                           //tiempo calentamiento bujia
                  case 12: if (rtempvar<1)rTempVar=1;
                           break;                           
                           //tiempo accionamiento motor arranque
                  case 13: if (rtempvar<1)rTempVar=1;
                           break;
                           //pausa entre arranque y arranque
                  case 14: if(rtempvar<15)rTempVar=15;
                           break;
                           //Cantidad de intentos de arranques
                  case 15: if(rtempvar<1)rTempVar=1;
                           break;
                           //Simulacion de transferencia
                           //arranque manual del grupo
                  case 16: if (fArriba){
                              if (rtempvar>1)rTempVar=1;
                           }
                           if (fAbajo){
                              if (rtempvar==255)rTempVar=0;
                           }
                           break;
                           //Tiempo de falla con grupo en modo pulso continuo
                  case 17: if (rtempvar==0)rTempVar=1;
                           break;
                           //tiempo de actuacion de cebador (aca los limites si existieran=
                  case 18: 
                           break;
                           //Operacion manual de la transferencia (teclas arriba o abajo)
                  case 19:
                           if(p_1seg0H){
                              if(--f0hora==0){
                                 f0hora=k0hora;
                                 rHsMarcha=0;   //Horas de marcha
                                 rMinMarcha=0;
                                 rSegMarcha=0; 
                                 fGuardaHS=1;
                              }
                           }
                           break;
                  case 20://ajuste tipo de parada de grupo
                           if(rTempVar==2)rTempVar=1;
                           if(rTempVar>2)rTempVar=0;
                           break;
                  //case 21 medicion de tension de red
                  //case 22 medicion de tension de grupo
                  case 23://Config tiempo accionamiento contactor linea
                           if(rTempVar>3)rTempVar=3;
                           if(rTempVar==255)rTempVar=0;
                           break;   
                  case 24://Funcion SD2
                           //if(rTempVar>6)rTempVar=6;
                           //if(rTempVar==255)rTempVar=0;
                           break;   
                  case 25://Funcion SD3
                           if(rTempVar>6)rTempVar=6;
                           if(rTempVar==255)rTempVar=0;
                           break;                              
                  case 38: if (fArriba) rGanLiR++;
                           if (fAbajo)  rGanLiR--;
                           break;                  
                  case 39: if (fArriba) rGanLiS++;
                           if (fAbajo)  rGanLiS--;
                           break;
                  case 40: if (fArriba) rGanLiT++;
                           if (fAbajo)  rGanLiT--;
                           break;
                  case 41: if (fArriba) rGanGrR++;
                           if (fAbajo)  rGanGrR--;
                           break;
                  case 42: if (fArriba) rGanGrS++;
                           if (fAbajo)  rGanGrS--;
                           break;       
                  case 43: if (fArriba) rGanGrT++;
                           if (fAbajo)  rGanGrT--;
                           break;         
                  case 44: if (fArriba) rHistLi++;
                           if (rHistLi >10) rHistLi=10;
                           if (fAbajo)  rHistLi--;
                           if (rHistLi < 1) rHistLi=1;
                           break;  
                  case 45: if (fArriba) rHistGr++;
                           if (rHistGr >10) rHistGr=10;
                           if (fAbajo)  rHistGr--;
                           if (rHistGr <1) rHistGr=1;
                           break;                           
                  default: break;
            }   
      fArriba=0;
      fAbajo=0;
      p_1seg0H=0;
      //fActDisp=1;
      fActVal=1;
      break;
      }
      //Tecla OK
      if(fOk){
         fActDisp=1;
         fOk=0;
            switch   (rMenu){
                  case 1:  rModoTransf=rtempvar;
                           rTempVar=VRminLi;
                           break;
                  case 2:  VRminLi=rTempvar;
                           rTempVar=VRmaxLi;
                           break;
                  case 3:  VRmaxLi=rTempvar;
                           rTempVar=VRminGr;
                           break;
                  case 4:  VRminGr=rTempvar;
                           rTempVar=VRMaxGr;
                           break;    
                  case 5:  VRMaxGr=rTempvar;
                           rTempVar=preDyArrGr;
                           break;                             
                  case 6:  preDyArrGr=rTempvar;
                           tmrDyArrGr=preDyArrGr;
                           rTempvar=pretmrConGR;
                           break;
                  case 7:  pretmrConGR=rTempvar;
                           tmrConGR=pretmrConGR;
                           rTempvar=prePareGr;
                           break;
                  case 8:  prePareGr=rTempvar;
                           tmrPareGR=prePareGR;
                           rTempVar=preLiOk;
                           break;            
                  case 9:  preLiOk=rTempvar;
                           tmrLiOk=preLiOk;;
                           rTempVar=rConfig;
                           break;
                  case 10: rConfig=rTempVar;
                           rTempVar=ModoGR;
                           break;
                  case 11: ModoGR=rTempVar;
                           if (modoGr==1)rTipoPare=0;
                           rTempVar=preCalBuj;
                           break;
                  case 12: preCalBuj=rTempVar;
                           tmrCalBuj=preCalBuj;
                           rTempVar=preTArr;
                           break;                           
                  case 13: preTArr=rTempVar;
                           TArr=preTArr;
                           rTempVar=preTPausa;
                           break;
                  case 14: preTPausa=rTempVar;
                           TPausa=preTPausa;
                           rTempVar=preIntArr;
                           break;
                  case 15: preIntArr=rTempVar;
                           intentosArr=preIntArr;
                           rTempVar=fArrManual;
                           break;
                  case 16: fArrManual=rTempVar;
                           //rtempvar=tPreFallaGr;
                           rMenu=15;//queda en el mismo menu
                           break; 
                           //Tiempo de falla con grupo en modo pulso continuo
                  case 17: tPreFallaGr=rTempVar;
                           tFallaGR=tPreFallaGr; 
                           rTempVar=rTPreCeb;
                           break;
                           //tiempor de actuacion de cebador
                  case 18: rTPreCeb=rTempVar;
                           if (rTPreCeb != 0) fCeb=1;
                           else fCeb=0;
                           break;
                  case 19: rTempVar=rTipoPare;
                           break;
                  case 20: if(!fCeb)rTipoPare=rTempVar;
                           ELSE rTipoPare=0;
                           //rTempVar=rFunSD1;
                           break;   
                  case 21: //rTempVar=rFunSD2;
                           //rFunSD1=rTempVar;
                           break;                           
                  case 22: //rTempVar=rFunSD3;
                           //rFunSD2=rTempVar;
                           rTempVar=preDyCeLi;
                           break; //valor 37, que sumado al final da 38, que es el iniicio de calibracion
                  case 23: rMenu=50;//menu 18 es tipo de pare, cuando apreta ok, asigno
                           preDyCeLi=rTempVar;
                           rDyCeLi=preDyCeLi;
                           break;                           
                  case  38:
                        vMenu38();//linea R
                        break;                      
                  case  39:
                        vMenu39();//linea S
                        break;
                  case  40:
                        vMenu40();//linea T
                        break; 
                  case  41:
                        vMenu41();//grupo R
                        break;
                  case  42:
                        vMenu42();//grupo S
                        break; 
                  case  43:
                        vMenu43();//grupo T
                        break; 
                  case  44:
                        vMenu44();//histeresis linea
                        break; 
                  case  45:
                        vMenu45();//histeresis grupo
                        break;                         
                  default: break;
            }
            if(++rMenu>=rMaxMenu){
               rMenu=0;
               fGuardarEEPROM=1;
            }
      }//fin tecla OK
      break;
   }



//////  PANTALLA DE ALARMAS  ///////
         if(rMenu==60){
            if(fOk){
               fOk=0;
               fFalla=0;
               fFallaGr=0;
               fFaAbLi=0;
               fFaCeLi=0;
               fFaAbGr=0;
               fFaCeGr=0; 
               fFaNoParGr=0;
               fFaParoGr=0;   //paro solo el grupo
               fFallVGr=0;    //falla grupo no genera las 3 tensiones
               fActDisp=1;
               rTActV=kTActV;    //reset temporizador
               rMenu=rBackMenu;
            }
            if(fArriba)fArriba=0;
            if(fAbajo)fAbajo=0;
         if(fMenu)fMenu=0;
       }//Fin Menu 10
}//fin funcion menu


