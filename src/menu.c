/////////////////////////////////////////////////////
//                Tratamiento de MENU              //
/////////////////////////////////////////////////////
//#org 0x1000, 0x17ff
//#separate
void Menu(void){
fActDisp=0;
lcd_putc('\f'); //limpiar display
         switch(rMenu){
               case 0:
                      menu0();
                      break;
               case 1:
                      menu1();
                      break;
               case 2:
                      menu2();
                      break;
               case 3:
                      menu3();
                      break;
               case 4:
                      menu4();
                      break;
               case 5:
                      menu5();
                      break;
               case 6:
                      menu6();
                      break;
               case 7:
                      menu7();
                      break;   
               case 8:
                      menu8();
                      break;
               case 9:
                      menu9();
                      break;
               case 10:
                      menu10();
                      break;
              case 11:
                      menu11();
                      break;    
               case 12:
                      menu12();
                      break;  
               case 13:
                      menu13();
                      break;                       
               case 14:
                      menu14();
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
               case 18:
                     Menu18();
                     break;
               case 19:
                     Menu19();
                     break;
               case 20:
                     Menu20();
                     break;
               case 21://mostrar tensiones que esta midiendo la linea
                     Menu21();
                     break;
               case 22://mostrar tensiones que esta midiendo el grupo
                     Menu22();
                     break;
               case 23://onfigura si acciona linea inmediatamente que sale el grupo
                     Menu23();
                     break;
               case  38://ajuste tensiones de linea R
                     Menu38();
                     break;                      
               case  39://ajuste tensiones de linea S
                     Menu38();
                     break;
               case  40://ajuste tensiones de Linea T
                     Menu38();
                     break;  
               case  41://ajuste tensiones de grupo R
                     Menu41();
                     break;
               case  42://ajuste tensiones de grupo S
                     Menu41();
                     break;
               case  43://ajuste tensiones de grupo T
                     Menu41();
                     break;                     
               Case 44://ajuste histeresis linea
                     Menu44();
                     break;
               Case 45://ajuste histeresis grupo
                     Menu44();
                     break;
               Case 46:fActDisp=1;//Alarmas
                     break;                     
               default:break;
         }
}


void Menu0(){//menu ppal
//         lcd_putc('\f'); //limpiar display
         //MODO TRANSFERENCIA
         lcd_gotoxy(1,1);
         printf(lcd_putc,"Sist:");
         
         //ESTADO ALIMENTACION
         lcd_gotoxy(1,2);
         printf(lcd_putc,"Alim:");
         
         //ESTADO GRUPO
         lcd_gotoxy(11,1);
         printf(lcd_putc,"GR:");
}

Void Menu1(){
//         lcd_putc('\f'); //limpiar display
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Modo Transf.");     
}

Void Menu2(){
//         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Minima V Linea");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"Vmin: ");
}

void Menu3(){
//         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Maxima V Linea");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"Vmax: ");
}
Void Menu4(){
//         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Minima V Grupo");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"Vmin: ");
}

void Menu5(){
//         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Maxima V Grupo");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"Vmax: ");
}
Void Menu6(){
//         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Ret. Arr Grupo");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"T1: ");
}

Void Menu7(){
//         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Ret. Con Gr");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"T2: ");
}

Void Menu8(){
//         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Pare Grupo");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"T3: ");
}

Void Menu9(){
//         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Retorno Linea");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"T4: ");
}

Void Menu10(){
//         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Accionamiento");
         lcd_gotoxy(1,2);
         if (rTempVar==1)
            printf(lcd_putc,"Acc: ");
         if (rTempVar==2)
            printf(lcd_putc,"Acc: ");
}

Void Menu11(){
//         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Tipo Arr Grupo");
         lcd_gotoxy(1,2);
         if (rTempVar==1)
            printf(lcd_putc,"Arr GR: ");
         if (rTempVar==2)
            printf(lcd_putc,"Arr GR: ");            
}

Void Menu12(){
//         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"T. Prec. Bujia");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"T6: ");
}

Void Menu13(){
//         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"T. Acc. Arr.");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"T7: ");
}

Void Menu14(){
//         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"T. Pausa");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"T8: ");
}

Void Menu15(){
//         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Intentos Arr.");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"Arr: ");
}

void Menu16(){//MENU NRO 13
//       lcd_putc('\f'); //limpiar display
         //Linea
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Arranque Manual");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"Arr. Grupo:");         
}

Void Menu17(){
//         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"T Fa Modo Cont.");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"T9: ");
}

Void Menu18(){
//         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Tiempo Cebador");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"TC: ");         
}

Void Menu19(){//operacion manual de la transferencia
//         lcd_putc('\f');
         lcd_gotoxy(5,1);
         printf(lcd_putc,"HS Marcha");
}

Void Menu20(){//operacion manual de la transferencia
//         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Tipo Pare Gr");
}

Void Menu21(){//operacion manual de la transferencia
//         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Medicion RED");
}

Void Menu22(){//operacion manual de la transferencia
//         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Medicion GRUPO");
}

Void Menu23(){//operacion manual de la transferencia
//         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Acc Contactor");
}
//Refresco valores analogicos
//void RefrescaLect(){

//}

void Menu38(){//Menu Ajustes Tension de Grupo
//         lcd_putc('\f'); //limpiar display
         lcd_gotoxy(1,1);
         printf(lcd_putc,"AJUSTE LINEA");//MAN
         lcd_gotoxy(1,2);
         printf(lcd_putc,"FASE");//MAN
         lcd_gotoxy(13,2);
         if (fTempOkLi==1)
            printf(lcd_putc,"L:OK");//MAN
         ELSE
            printf(lcd_putc,"L:FA");//MAN
}

void Menu39(){//Menu Ajustes Tension de Grupo
//         lcd_putc('\f'); //limpiar display
         lcd_gotoxy(2,1);
         printf(lcd_putc,"AJUSTE V LINEA");//MAN

}
void Menu40(){//Menu Ajustes Tension de Grupo
//         lcd_putc('\f'); //limpiar display
         lcd_gotoxy(2,1);
         printf(lcd_putc,"AJUSTE V LINEA");//MAN

}
VOID Menu41(){
//         lcd_putc('\f'); //limpiar display        
         lcd_gotoxy(1,1);
         printf(lcd_putc,"AJUSTE V GRUPO");//MAN
         //printf(lcd_putc,"FA GR: %X",rFaGr);//MAN
         lcd_gotoxy(1,2);
         printf(lcd_putc,"FASE");//MAN         
/*         lcd_gotoxy(13,2);
         if (fTempOkGr==1)
            printf(lcd_putc,"G:OK");//MAN
         ELSE
            printf(lcd_putc,"G:FA");//MAN*/
}

VOID Menu42(){
//         lcd_putc('\f'); //limpiar display        
         lcd_gotoxy(2,1);
         printf(lcd_putc,"AJUSTE V GRUPO");//MAN
}
VOID Menu43(){
//         lcd_putc('\f'); //limpiar display        
         lcd_gotoxy(2,1);
         printf(lcd_putc,"AJUSTE V GRUPO");//MAN
}
VOID Menu44(){
//         lcd_putc('\f'); //limpiar display        
         lcd_gotoxy(2,1);
         printf(lcd_putc,"HISTERESIS");//MAN
}






