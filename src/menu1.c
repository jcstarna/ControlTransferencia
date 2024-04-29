/////////////////////////////////////////////////////
//                Tratamiento de MENU              //
/////////////////////////////////////////////////////
void Menus(void){
fActDisp=0;
         switch(rMenu){
              /* case 0:
                      menu0();//MenuTest(); //
                      break;
               case 1:
                      menu01();
                      break;
               case 2:
                      menu1();
                      break;
               case 3:
                      menu2();
                      break;
               case 4:
                      menu3();
                      break;
               case 5:
                      menu4();
                      break;
               case 6:
                      menu5();
                      break;
               case 7:
                      menu6();
                      break;   
               case 8:
                      menu7();
                      break;
               case 9:
                      menu8();
                      break;
               case 10:
                      menu9();
                      break;*/
               case 11:
                      menu10();
                      break;    
               case 12:
                      menu11();
                      break;  
               case 13:
                      menu12();
                      break;                       
               case 14:
                      menuManual();
                      break;   
               case 15:
                      menu15();
                      break; 
               case 16:
                      menu16();
                      break; 
               case 17:                      
                      Menu17();
                      break; 
               case  18:
                     Menu18();
                     break;                      
               case  19:
                     Menu19();
                     break;
               case 20:PantallaAlarmas();
                     break;
               default:break;
         }
}
/*
void Menu0(){//menu ppal
         lcd_putc('\f'); //limpiar display
         //MODO TRANSFERENCIA
         lcd_gotoxy(1,1);
         printf(lcd_putc,"Sist:");
         if (rModoTransf==1)printf(lcd_putc,"AUT");
         else printf(lcd_putc,"MAN");//MAN
         
         //ESTADO ALIMENTACION
         lcd_gotoxy(1,2);
         printf(lcd_putc,"Alim:");
         
         if (input(pCO_LI))
            printf(lcd_putc,"LI");
         else if (input(pCO_GR))
            printf(lcd_putc,"GR");
         else 
            printf(lcd_putc,"--");
         
         //ESTADO GRUPO
         lcd_gotoxy(11,1);
         printf(lcd_putc,"GR:");
         if (rModoTransf==1){
               if (fRunDelayArrGr)printf(lcd_putc,"ESP");
               else if (OArrGR &!fGrupoOk)printf(lcd_putc,"ARR");
               else if (fGrupoOk & OArrGR & !fCO_GR)printf(lcd_putc,"PRE"); 
               else if (fGrupoOk & OArrGR & fCO_GR)printf(lcd_putc,"CON");
               else if (fGrupoOk & !OArrGR & !fCO_GR)printf(lcd_putc,"PAR");
               else if (!fGrupoOk & !OArrGR) printf(lcd_putc,"PDO");  
         }
}

Void Menu01(){
         lcd_putc('\f'); //limpiar display
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Modo Transf.");
         lcd_gotoxy(3,2);
         if (rTempVar==1)printf(lcd_putc,"Automatico");
         else printf(lcd_putc,"Manual");        
}

Void Menu1(){
         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Minima Tension");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"Vmin: %3u V",rTempVar);
}

void Menu2(){
         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Maxima Tension");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"Vmax: %3u V",rTempVar);
}

Void Menu3(){
         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Ret. Arr Grupo");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"T1: %3u Seg.",rTempVar);
}

Void Menu4(){
         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Ret. Con Gr");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"T2: %3u Seg.",rTempVar);
}

Void Menu5(){
         lcd_putc('\f');
         lcd_gotoxy(3,1);
         printf(lcd_putc,"Pare Grupo");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"T3: %3u Min.",rTempVar);
}

Void Menu6(){
         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Retorno Linea");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"T4: %3u Seg.",rTempVar);
}

Void Menu7(){
         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Accionamiento");
         lcd_gotoxy(1,2);
         if (rTempVar==1)
            printf(lcd_putc,"Acc: Contactores");
         if (rTempVar==2)
            printf(lcd_putc,"Acc: Interruptores");
}

Void Menu8(){
         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Tipo Arr Grupo");
         lcd_gotoxy(1,2);
         if (rTempVar==1)
            printf(lcd_putc,"Arr GR: S Cont.");
         if (rTempVar==2)
            printf(lcd_putc,"Arr GR: Mot. Arr.");            
}

Void Menu9(){
         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"T. Prec. Bujia");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"T6: %3u Seg.",rTempVar);
}
*/
Void Menu10(){
         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"T. Acc. Arr.");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"T7: %3u Seg.",rTempVar);
}

Void Menu11(){
         lcd_putc('\f');
         lcd_gotoxy(4,1);
         printf(lcd_putc,"T. Pausa");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"T8: %3u Seg.",rTempVar);
}

Void Menu12(){
         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Intentos Arr.");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"Arr: %3u",rTempVar);
}

Void Menu15(){
         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"T Fa Modo Cont.");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"T9: %3u Min",rTempVar);
}

Void Menu16(){
         lcd_putc('\f');
         lcd_gotoxy(5,1);
         printf(lcd_putc,"Cebador");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"TC: %3u Seg",rTempVar);         
}

Void Menu17(){//operacion manual de la transferencia
         lcd_putc('\f');
         lcd_gotoxy(5,1);
         printf(lcd_putc,"HS Marcha");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"HS: %5lu Hs",rHsMarcha);
}
//Refresco valores analogicos
void RefrescaLect(){

}

void Menu18(){//Menu Ajustes Tension de Grupo
         lcd_putc('\f'); //limpiar display

         lcd_gotoxy(2,1);
         printf(lcd_putc,"AJUSTE V LINEA");//MAN

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

VOID Menu19(){
         lcd_putc('\f'); //limpiar display
         
         lcd_gotoxy(2,1);
         printf(lcd_putc,"AJUSTE V GRUPO");//MAN
         
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

void MenuManual(){//MENU NRO 13
         lcd_putc('\f'); //limpiar display
         //Linea
         lcd_gotoxy(3,1);
         printf(lcd_putc,"Mantenimiento");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"Test.Transf->");         
         if (rtempvar==1)printf(lcd_putc,"ACT");
         else printf(lcd_putc,"DES");//MAN
}

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
}




