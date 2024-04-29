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
                      vmenu01();
                      break;
               case 2:
                      vmenu1();
                      break;
               case 3:
                      vmenu2();
                      break;
               case 4:
                      vmenu3();
                      break;
               case 5:
                      vmenu4();
                      break;
               case 6:
                      vmenu5();
                      break;
               case 7:
                      vmenu6();
                      break;   
               case 8:
                      vmenu7();
                      break;
               case 9:
                      vmenu8();
                      break;
               case 10:
                      vmenu9();
                      break;
              case 11:
                      vmenu10();
                      break;    
               case 12:
                      vmenu11();
                      break;  
               case 13:
                      vmenu12();
                      break;                       
               case 14:
                      vmenuManual();
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
               case 18://ajuste tipo de parada de grupo                      
                      vMenuParGr();
                      break;                       
               case  38:
                     vMenu18();
                     break;                      
               case  39:
                     vMenu19();
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
         if (rModoTransf==1){
               if (fRunDelayArrGr)printf(lcd_putc,"ESP");
               else if (OArrGR &!fGrupoOk)printf(lcd_putc,"ARR");
               else if (fGrupoOk & OArrGR & !fCO_GR & !fArrManual & !fArrRem)printf(lcd_putc,"PRE"); 
               else if (fGrupoOk & OArrGR & fCO_GR)printf(lcd_putc,"CON");
               else if (fGrupoOk & !OArrGR & !fCO_GR)printf(lcd_putc,"PAR");
               else if (!fGrupoOk & !OArrGR & !fArrRem) printf(lcd_putc,"PDO");  
               else if (fGrupoOk & OArrGR & fArrRem & !fCO_GR)printf(lcd_putc,"REM");
               else if (fGrupoOk & OArrGR & fArrManual & !fCO_GR)printf(lcd_putc,"USR");
         }
         else 
               printf(lcd_putc,"   ");
}

Void vMenu01(){
//         lcd_putc('\f'); //limpiar display
         lcd_gotoxy(3,2);
         if (rTempVar==1)printf(lcd_putc,"Automatico");
         else            printf(lcd_putc,"Manual    ");        
}

Void vMenu1(){
//         lcd_putc('\f');
         lcd_gotoxy(7,2);
         printf(lcd_putc,"%3u V",rTempVar);
}

void vMenu2(){
//         lcd_putc('\f');
         lcd_gotoxy(7,2);
         printf(lcd_putc,"%3u V",rTempVar);
}

Void vMenu3(){
//         lcd_putc('\f');
         lcd_gotoxy(5,2);
         printf(lcd_putc,"%3u Min.",rTempVar);
}

Void vMenu4(){
//         lcd_putc('\f');
         lcd_gotoxy(5,2);
         printf(lcd_putc,"%3u Min.",rTempVar);
}

Void vMenu5(){
//         lcd_putc('\f');
         lcd_gotoxy(5,2);
         printf(lcd_putc,"%3u Min.",rTempVar);
}

Void vMenu6(){
//         lcd_putc('\f');
         lcd_gotoxy(5,2);
         printf(lcd_putc,"%3u Min.",rTempVar);
}

Void vMenu7(){
//         lcd_putc('\f');
         lcd_gotoxy(6,2);
         if (rTempVar==1)
            printf(lcd_putc,"Contactores");
         if (rTempVar==2)
            printf(lcd_putc,"Interruptor");
}

Void vMenu8(){
//         lcd_putc('\f');
         lcd_gotoxy(9,2);
         if (rTempVar==1)
            printf(lcd_putc,"S Cont. ");
         if (rTempVar==2)
            printf(lcd_putc,"Mot. Arr");            
}

Void vMenu9(){
//         lcd_putc('\f');
         lcd_gotoxy(5,2);
         printf(lcd_putc,"%3u Seg.",rTempVar);
}

Void vMenu10(){
//         lcd_putc('\f');
         lcd_gotoxy(5,2);
         printf(lcd_putc,"%3u Seg.",rTempVar);
}

Void vMenu11(){
//         lcd_putc('\f');
         lcd_gotoxy(5,2);
         printf(lcd_putc,"%3u Seg.",rTempVar);
}

Void vMenu12(){
//         lcd_putc('\f');
         lcd_gotoxy(6,2);
         printf(lcd_putc,"%3u",rTempVar);
}

Void vMenu15(){
//         lcd_putc('\f');
         lcd_gotoxy(5,2);
         printf(lcd_putc,"%3u Min",rTempVar);
}

Void vMenu16(){
//         lcd_putc('\f');
         lcd_gotoxy(5,2);
         printf(lcd_putc,"%3u Seg",rTempVar);         
}

Void vMenu17(){//operacion manual de la transferencia
//         lcd_putc('\f');
         lcd_gotoxy(1,2);
         printf(lcd_putc,"%5luHs,%2uM",rHsMarcha,rMinMarcha);
}

Void vMenuParGr(){//18  operacion manual de la transferencia
//         lcd_putc('\f');
         lcd_gotoxy(2,2);
         if (!fCeb){
            if (rTempVar==0)printf(lcd_putc,"Contacto");
            else if (rTempVar==1)printf(lcd_putc,"ElectroV");
         }
         else
            printf(lcd_putc,"Contacto");
}
//Refresco valores analogicos
//void RefrescaLect(){

//}

void vMenu18(){//Menu Ajustes Tension de Grupo
//         lcd_putc('\f'); //limpiar display
         lcd_gotoxy(1,2);
         if (input(pFallaLi))
            printf(lcd_putc,"%3u",VRLi);//MAN
         else
            printf(lcd_putc,"???");//MAN
            
         lcd_gotoxy(5,2);
         if (input(pFallaLi))
            printf(lcd_putc,"%3u",VSLi);//MAN
         else
            printf(lcd_putc,"???");//MAN
            
         lcd_gotoxy(9,2);
         if (input(pFallaLi))
            printf(lcd_putc,"%3u",VTLi);//MAN
         else
            printf(lcd_putc,"???");//MAN
            
         lcd_gotoxy(13,2);
         if (fTempFallaLi==1)
            printf(lcd_putc,"L:OK");//MAN
         ELSE
            printf(lcd_putc,"L:FA");//MAN
}

VOID vMenu19(){
//         lcd_putc('\f'); //limpiar display
        
         lcd_gotoxy(1,2);
         if (input(pFallaGr))
            printf(lcd_putc,"%3u",VRGr);//MAN
         else
            printf(lcd_putc,"???");//MAN
            
         lcd_gotoxy(5,2);
         if (input(pFallaGr))
            printf(lcd_putc,"%3u",VSGr);//MAN
         else
            printf(lcd_putc,"???");//MAN
            
         lcd_gotoxy(9,2);
         if (input(pFallaGr))
            printf(lcd_putc,"%3u",VTGr);//MAN 
         else
            printf(lcd_putc,"???");//MAN 
            
         lcd_gotoxy(13,2);
         if (fTempFallaGr==1)
            printf(lcd_putc,"G:OK");//MAN
         ELSE
            printf(lcd_putc,"G:FA");//MAN
}

void vMenuManual(){//MENU NRO 13
//         lcd_putc('\f'); //limpiar display
         //Linea
         lcd_gotoxy(12,2);
         //printf(lcd_putc,"%3u",pasoArr);
         if (rtempvar==1)printf(lcd_putc,"ARR");
         else printf(lcd_putc,"PAR");//MAN
}






