/////////////////////////////////////////////////////
//                Tratamiento de MENU              //
/////////////////////////////////////////////////////
//#org 0x1000, 0x17ff
//#separate
void vMenu(void){
fActVal=0;
         switch(rMenu){
               case 0:
                      vmenu0();
                      break;
               case 1:
                      vmenu1();
                      break;
               case 2:
                      vmenu2();//menu que muestra tensiones
                      break;
               case 3:
                      vmenu2();//menu que muestra tensiones
                      break;
               case 4:
                      vmenu2();//menu que muestra tensiones
                      break;
               case 5:
                      vmenu2();//menu que muestra tensiones
                      break;
               case 6:
                      vmenu6();
                      break;
               case 7:
                      vmenu7();
                      break;   
               case 8:
                      vmenu8();
                      break;
               case 9:
                      vmenu9();
                      break;
               case 10:
                      vmenu10();
                      break;
              case 11:
                      vmenu11();
                      break;    
               case 12:
                      vmenu12();
                      break;  
               case 13:
                      vmenu13();
                      break;                       
               case 14:
                      vmenu14();
                      break;   
               case 15:
                      vmenu15();
                      break; 
               case 16:
                      vmenu16();
                      break; 
               case 17:                      
                      vMenu17();
                      break; 
               case 18:                
                      vMenu18();
                      break;  
               case 19:                
                      vMenu19();
                      break;
               case 20:                
                      vMenu20();
                      break;  
               case 21:                
                      vMenu21();
                      break; 
               case 22:                
                      vMenu22();
                      break;           
               case 23:                
                      vMenu23();
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
                     vMenu44();//HISTERESIS LINEA
                     break;                     
               case  45:
                     vMenu45();//HISTERESIS GRUPO
                     break;                     
               //Case 20:fActDisp=1;
               //      break;
               default:break;
         }
}


void vMenu0(){//menu ppal
//         lcd_putc('\f'); //limpiar display
         //MODO TRANSFERENCIA
         lcd_gotoxy(6,1);
         if (rModoTransf==1)printf(lcd_putc,"AUT");
         else printf(lcd_putc,"MAN");//MAN
         
         //ESTADO ALIMENTACION
         lcd_gotoxy(6,2);
         if (input(pCO_Li))
            printf(lcd_putc,"LI");
         else if (input(pCO_GR))
            printf(lcd_putc,"GR");
         else 
            printf(lcd_putc,"--");
         
         //ESTADO GRUPO
         lcd_gotoxy(14,1);
         //if (rModoTransf==1){
               if (fRunDelayArrGr)printf(lcd_putc,"ESP");
               else if (OArrGR &!fGrupoOk)printf(lcd_putc,"ARR");
               else if (fGrupoOk & OArrGR & !fCO_GR & !fArrManual & !fArrRem)printf(lcd_putc,"PRE"); 
               else if (fGrupoOk & OArrGR & fCO_GR)printf(lcd_putc,"CON");
               else if (fGrupoOk & !OArrGR & !fCO_GR)printf(lcd_putc,"PAR");
               else if (!fGrupoOk & !OArrGR & !fArrRem) printf(lcd_putc,"PDO");  
               else if (fGrupoOk & OArrGR & fArrRem & !fCO_GR)printf(lcd_putc,"REM");
               else if (fGrupoOk & OArrGR & fArrManual & !fCO_GR)printf(lcd_putc,"USR");
         //}
         //else 
         //      printf(lcd_putc,"   ");
         //lcd_gotoxy(10,2);      
         //printf(lcd_putc,"%2X %u",rFaLi,TmrLiOk);      
}

Void vMenu1(){
//         lcd_putc('\f'); //limpiar display
         lcd_gotoxy(3,2);
         if (rTempVar==1)printf(lcd_putc,"Automatico");
         else            printf(lcd_putc,"Manual    ");        
}

Void vMenu2(){
//       MENU GENERAL PARA MOSTRAR TENSIONES
         lcd_gotoxy(7,2);
         printf(lcd_putc,"%3u V",rTempVar);
}

Void vMenu6(){
//         lcd_putc('\f');
         lcd_gotoxy(5,2);
         printf(lcd_putc,"%3u Seg.",rTempVar);
}

Void vMenu7(){
//         lcd_putc('\f');
         lcd_gotoxy(5,2);
         printf(lcd_putc,"%3u Seg.",rTempVar);
}

Void vMenu8(){
//         lcd_putc('\f');
         lcd_gotoxy(5,2);
         printf(lcd_putc,"%3u Min.",rTempVar);
}

Void vMenu9(){
//         lcd_putc('\f');
         lcd_gotoxy(5,2);
         printf(lcd_putc,"%3u Seg.",rTempVar);
}

Void vMenu10(){
//         lcd_putc('\f');
         lcd_gotoxy(6,2);
         if (rTempVar==1)
            printf(lcd_putc,"Contactores");
         if (rTempVar==2)
            printf(lcd_putc,"Interruptor");
}

Void vMenu11(){
//         lcd_putc('\f');
         lcd_gotoxy(9,2);
         if (rTempVar==1)
            printf(lcd_putc,"S Cont. ");
         if (rTempVar==2)
            printf(lcd_putc,"Mot. Arr");            
}

Void vMenu12(){
//         lcd_putc('\f');
         lcd_gotoxy(5,2);
         printf(lcd_putc,"%3u Seg.",rTempVar);
}

Void vMenu13(){
//         lcd_putc('\f');
         lcd_gotoxy(5,2);
         printf(lcd_putc,"%3u Seg.",rTempVar);
}

Void vMenu14(){
//         lcd_putc('\f');
         lcd_gotoxy(5,2);
         printf(lcd_putc,"%3u Seg.",rTempVar);
}

Void vMenu15(){
//         lcd_putc('\f');
         lcd_gotoxy(6,2);
         printf(lcd_putc,"%3u",rTempVar);
}

void vMenu16(){//MENU NRO 13
//         lcd_putc('\f'); //limpiar display
         //Linea
         lcd_gotoxy(12,2);
         //printf(lcd_putc,"%3u",pasoArr);
         if (rtempvar==1)printf(lcd_putc,"ARR");
         else printf(lcd_putc,"PAR");//MAN
}

Void vMenu17(){
//         lcd_putc('\f');
         lcd_gotoxy(5,2);
         printf(lcd_putc,"%3u Min",rTempVar);
}

Void vMenu18(){
//         lcd_putc('\f');
         lcd_gotoxy(5,2);
         printf(lcd_putc,"%3u Seg",rTempVar);         
}

Void vMenu19(){//operacion manual de la transferencia
//         lcd_putc('\f');
         lcd_gotoxy(1,2);
         printf(lcd_putc,"%5luHs,%2uM",rHsMarcha,rMinMarcha);
}

Void vMenu20(){//18  operacion manual de la transferencia
//         lcd_putc('\f');
         lcd_gotoxy(2,2);
         if (!fCeb){
            if (rTempVar==0)printf(lcd_putc,"Contacto");
            else if (rTempVar==1)printf(lcd_putc,"ElectroV");
         }
         else
            printf(lcd_putc,"Contacto");
}

void vMenu21(){// medidion de tensiones de linea
         lcd_gotoxy(1,2);
         //printf(lcd_putc,"R:");//MAN  
         //lcd_gotoxy(8,2);
         if (fRxOkLi)         //input(pFallaLi))
            printf(lcd_putc,"%3lu %3lu %3lu",VRLi,VSLi,VTLi);//MAN
         else
            printf(lcd_putc,"--- --- ---");//MAN
         lcd_gotoxy(13,2);
         if (fTempOkLi==1)
            printf(lcd_putc,"L:OK");//MAN
         ELSE
            printf(lcd_putc,"L:FA");//MAN      
}

void vMenu22(){// medidion de tensiones de grupo
         lcd_gotoxy(1,2);
         //printf(lcd_putc,"R:");//MAN  
         //lcd_gotoxy(8,2);
         if (fRxOkGr)         //input(pFallaLi))
            printf(lcd_putc,"%3lu %3lu %3lu",VRGr,VSGr,VTGr);//MAN
         else
            printf(lcd_putc,"--- --- ---");//MAN
         lcd_gotoxy(13,2);
         if (fTempOkGr==1)
            printf(lcd_putc,"G:OK");//MAN
         ELSE
            printf(lcd_putc,"G:FA");//MAN 
}

//elegir si eliminar o no tiempo de retardo en sacar contactor
//de linea y meter de grupo
void vMenu23(){// medidion de tensiones de grupo
         lcd_gotoxy(1,2);
         if (rTempVar==0)printf(lcd_putc,"Inmediato");
         else
            printf(lcd_putc,"Retardo=%u",rTempVar);
}
//Refresco valores analogicos
//void RefrescaLect(){

//}

void vMenu38(){//Menu Ajustes Tension de Linea
//         lcd_putc('\f'); //limpiar display
         lcd_gotoxy(6,2);
         printf(lcd_putc,"R:");//MAN  
         //lcd_gotoxy(8,2);
         if (fRxOkLi)         //input(pFallaLi))
            printf(lcd_putc,"%3luV",VRLi);//MAN
         else
            printf(lcd_putc,"???V");//MAN
         lcd_gotoxy(13,2);
         if (fTempOkLi==1)
            printf(lcd_putc,"L:OK");//MAN
         ELSE
            printf(lcd_putc,"L:FA");//MAN 
//         lcd_gotoxy(1,1);   
//         printf(lcd_putc,"%4lu%4lu%4lu%4lu",rDataRx1Li,rDataRx2Li,rDataRx3Li,rDataRx4Li);
//         lcd_gotoxy(1,2);   
//         printf(lcd_putc,"%d %u",rGanLiR,VRLi);

}

void vMenu39(){//Menu Ajustes Tension de Linea
         lcd_gotoxy(6,2);
         printf(lcd_putc,"S:");//MAN  
         //lcd_gotoxy(8,2);
         if (fRxOkLi)         //input(pFallaLi))
            printf(lcd_putc,"%3luV",VSLi);//MAN
         else
            printf(lcd_putc,"???V");//MAN
         lcd_gotoxy(13,2);
         if (fTempOkLi==1)
            printf(lcd_putc,"L:OK");//MAN
         ELSE
            printf(lcd_putc,"L:FA");//MAN
}

void vMenu40(){//Menu Ajustes Tension de Linea
         lcd_gotoxy(6,2);
         printf(lcd_putc,"T:");//MAN  
         //lcd_gotoxy(8,2);
         if (fRxOkLi)         //input(pFallaLi))
            printf(lcd_putc,"%3luV",VTLi);//MAN
         else
            printf(lcd_putc,"???V");//MAN
         lcd_gotoxy(13,2);
         if (fTempOkLi==1)
            printf(lcd_putc,"L:OK");//MAN
         ELSE
            printf(lcd_putc,"L:FA");//MAN
}            


VOID vMenu41(){
//         lcd_putc('\f'); //limpiar display
         lcd_gotoxy(6,2);
         printf(lcd_putc,"R:");//MAN  
         //lcd_gotoxy(8,2);
         if (fRxOkGr)         //input(pFallaGr))
            printf(lcd_putc,"%3luV",VRGr);//MAN
         else
            printf(lcd_putc,"???V");//MAN
         lcd_gotoxy(13,2);
         if (fTempOkGr==1)
            printf(lcd_putc,"G:OK");//MAN
         ELSE
            printf(lcd_putc,"G:FA");//MAN            
}

VOID vMenu42(){
         lcd_gotoxy(6,2);
         printf(lcd_putc,"S:");//MAN  
         //lcd_gotoxy(8,2);
         if (fRxOkGr)         //input(pFallaGr))
            printf(lcd_putc,"%3luV",VSGr);//MAN
         else
            printf(lcd_putc,"???V");//MAN
         lcd_gotoxy(13,2);
         if (fTempOkGr==1)
            printf(lcd_putc,"G:OK");//MAN
         ELSE
            printf(lcd_putc,"G:FA");//MAN            
}

VOID vMenu43(){
         lcd_gotoxy(6,2);
         printf(lcd_putc,"T:");//MAN  
         //lcd_gotoxy(8,2);
         if (fRxOkGr)         //input(pFallaGr))
            printf(lcd_putc,"%3luV",VTGr);//MAN 
         else
            printf(lcd_putc,"???V");//MAN
         lcd_gotoxy(13,2);
         if (fTempOkGr==1)
            printf(lcd_putc,"G:OK");//MAN
         ELSE
            printf(lcd_putc,"G:FA");//MAN               
}

//regulacion histeresis de linea
void vMenu44(){
         lcd_gotoxy(1,2);
         printf(lcd_putc,"Linea:%3u Volts",rHistLi);//MAN  
}

//regulacion histeresis de linea
void vMenu45(){
         lcd_gotoxy(1,2);
         printf(lcd_putc,"Grupo:%3u Volts",rHistGr);//MAN  
}







