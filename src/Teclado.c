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
   if(rMenu>0 & (rMenu!=40) ){
      if(fMenu){
            fMenu=0;
            fActDisp=1;
            if(++rMenu>=rMaxMenu)      //rmaxmenu maxima cantidad de menues a mostrar, varia segun tipo de grupo seleccionado
                  {rMenu=0;
                     //guardarEEPROM();
                     fGuardarEEPROM=1;
                  }
            switch   (rMenu){
                  case 2:  rTempVar=VRmin;
                           break;
                  case 3:  rTempvar=VRmax;
                           break;
                  case 4:  rTempVar=preDyArrGr;
                           break;
                  case 5:  rTempVar=pretmrConGR;
                           break;
                  case 6:  rTempVar=prePareGr;
                           break;
                  case 7:  rTempVar=preLiOk;
                           break;
                  case 8:  rTempVar=rConfig;
                           break;
                  case 9:  rTempVar=ModoGR;
                           break;
                  case 10: rTempVar=preCalBuj;
                           break;                           
                  case 11: rTempVar=preTArr;
                           break;
                  case 12: rTempVar=preTPausa;
                           break;   
                  case 13: rTempVar=preIntArr;
                           break;  
                  case 14: rTempVar=fArrManual;////////////////////////////
                           break;   
                  case 15: rTempVar=tPreFallaGr;
                           break;
                  case 16: rTempVar=rTPreCeb;
                           break;
                  case 17: rTempVar=rTipoPare;
                           break;                          
                  case 38: rMenu=39;       
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
                           //Regulacion de tension minima
                  case 2:  if (rTempvar < 150)rTempVar=150;
                           if (rtempvar >210)rtempVar=210;
                           break;
                           //Regulacion de tension maxima
                  case 3:  if (rTempvar > 250)rTempVar=250;
                           if (rtempvar < 225)rtempvar=225;
                           break;
                           //Retardo arranque de grupo
                  case 4:  preDyArrGr=rTempvar;
                           break;
                           //Retardo conexion de grupo
                  case 5:  pretmrConGR=rTempvar;
                           tmrConGR=rTempvar;
                           break;
                           //Tiempo parada de grupo
                  case 6:  prePareGr=rTempvar;
                           tmrPareGr=rTempvar;
                           break;   
                           //Tiempo deteccion de linea OK
                  case 7:  preLiOk=rTempvar;
                           break;
                           //Tipo de accionamiento
                  case 8:  if (rTempVar>2)rtempvar=2;
                           if (rTempVar<1)rtempvar=1;
                           break;
                           //Modo grupo electrogeno 
                  case 9:  if (rtempvar < 1) rtempvar=1;
                           if (rtempvar >2)rtempvar=2;
                           break;
                           //tiempo calentamiento bujia
                  case 10: if (rtempvar<1)rTempVar=1;
                           break;                           
                           //tiempo accionamiento motor arranque
                  case 11: if (rtempvar<1)rTempVar=1;
                           break;
                           //pausa entre arranque y arranque
                  case 12: if(rtempvar<15)rTempVar=15;
                           break;
                           //Cantidad de intentos de arranques
                  case 13: if(rtempvar<1)rTempVar=1;
                           break;
                           //Simulacion de transferencia
                           //arranque manual del grupo
                  case 14: if (rtempvar>1)rTempVar=0;
                           break;
                           //Tiempo de falla con grupo en modo pulso continuo
                  case 15: if (rtempvar==0)rTempVar=1;
                           break;
                           //tiempo de actuacion de cebador (aca los limites si existieran=
                  case 16: 
                           break;
                           //Operacion manual de la transferencia (teclas arriba o abajo)
                  case 17:
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
                  case 18://ajuste tipo de parada de grupo
                           if(rTempVar==2)rTempVar=1;
                           if(rTempVar>2)rTempVar=0;
                           break;
                  case 38: if (fArriba) rGanLi++;
                           if (fAbajo)  rGanLi--;
                           break;                  
                  case 39: if (fArriba) rGanGr++;
                           if (fAbajo)  rGanGr--;
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
                           rTempVar=VRmin;
                           break;
                  case 2:  VRmin=rTempvar;
                           rTempVar=VRmax;
                           break;
                  case 3:  VRmax=rTempvar;
                           rTempVar=preDyArrGr;
                           break;
                  case 4:  preDyArrGr=rTempvar;
                           tmrDyArrGr=preDyArrGr;
                           rTempvar=pretmrConGR;
                           break;
                  case 5:  pretmrConGR=rTempvar;
                           tmrConGR=pretmrConGR;
                           rTempvar=prePareGr;
                           break;
                  case 6:  prePareGr=rTempvar;
                           tmrPareGR=prePareGR;
                           rTempVar=preLiOk;
                           break;            
                  case 7:  preLiOk=rTempvar;
                           tmrLiOk=preLiOk;;
                           rTempVar=rConfig;
                           break;
                  case 8:  rConfig=rTempVar;
                           rTempVar=ModoGR;
                           break;
                  case 9:  ModoGR=rTempVar;
                           if (modoGr==1)rTipoPare=0;
                           rTempVar=preCalBuj;
                           break;
                  case 10:  preCalBuj=rTempVar;
                           tmrCalBuj=preCalBuj;
                           rTempVar=preTArr;
                           break;                           
                  case 11:  preTArr=rTempVar;
                           TArr=preTArr;
                           rTempVar=preTPausa;
                           break;
                  case 12: preTPausa=rTempVar;
                           TPausa=preTPausa;
                           rTempVar=preIntArr;
                           break;
                  case 13: preIntArr=rTempVar;
                           intentosArr=preIntArr;
                           rTempVar=fArrManual;
                           break;
                  case 14: fArrManual=rTempVar;
                           //rtempvar=tPreFallaGr;
                           rMenu=13;//queda en el mismo menu
                           break; 
                           //Tiempo de falla con grupo en modo pulso continuo
                  case 15: tPreFallaGr=rTempVar;
                           tFallaGR=tPreFallaGr; 
                           rTempVar=rTPreCeb;
                           break;
                           //tiempor de actuacion de cebador
                  case 16: rTPreCeb=rTempVar;
                           if (rTPreCeb != 0) fCeb=1;
                           else fCeb=0;
                           break;
                  case 17: rTempVar=rTipoPare;
                           break;
                  case 18: if(!fCeb)rTipoPare=rTempVar;
                           ELSE rTipoPare=0;
                           //rMenu=37;//menu 18 es tipo de pare, cuando apreta ok, asigno
                           break;   //valor 37, que sumado al final da 38, que es el iniicio de calibracion
                  case 38: rMenu=38;
                           break;
                  case 39: rMenu=18;
                           break;
                  default: break;
            }
            if(++rMenu==rMaxMenu){
               rMenu=0;
               fGuardarEEPROM=1;
            }
      }//fin tecla OK
      break;
   }



//////  PANTALLA DE ALARMAS  ///////
         if(rMenu==40){
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
               fActDisp=1;
               rMenu=rBackMenu;
            }
            if(fArriba)fArriba=0;
            if(fAbajo)fAbajo=0;
         if(fMenu)fMenu=0;
       }//Fin Menu 10
}//fin funcion menu


