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
                      break;
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
               case 18://ajuste tipo de parada grupo
                     MenuParGr();
                      break;
               case  38://ajuste tensiones de linea
                     Menu18();
                     break;                      
               case  39://ajuste tensiones de grupo
                     Menu19();
                     break;
               Case 40:fActDisp=1;//Alarmas
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

Void Menu01(){
//         lcd_putc('\f'); //limpiar display
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Modo Transf.");     
}

Void Menu1(){
//         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Minima Tension");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"Vmin: ");
}

void Menu2(){
//         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Maxima Tension");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"Vmax: ");
}

Void Menu3(){
//         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Ret. Arr Grupo");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"T1: ");
}

Void Menu4(){
//         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Ret. Con Gr");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"T2: ");
}

Void Menu5(){
//         lcd_putc('\f');
         lcd_gotoxy(3,1);
         printf(lcd_putc,"Pare Grupo");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"T3: ");
}

Void Menu6(){
//         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Retorno Linea");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"T4: ");
}

Void Menu7(){
//         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Accionamiento");
         lcd_gotoxy(1,2);
         if (rTempVar==1)
            printf(lcd_putc,"Acc: ");
         if (rTempVar==2)
            printf(lcd_putc,"Acc: ");
}

Void Menu8(){
//         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Tipo Arr Grupo");
         lcd_gotoxy(1,2);
         if (rTempVar==1)
            printf(lcd_putc,"Arr GR: ");
         if (rTempVar==2)
            printf(lcd_putc,"Arr GR: ");            
}

Void Menu9(){
//         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"T. Prec. Bujia");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"T6: ");
}

Void Menu10(){
//         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"T. Acc. Arr.");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"T7: ");
}

Void Menu11(){
//         lcd_putc('\f');
         lcd_gotoxy(4,1);
         printf(lcd_putc,"T. Pausa");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"T8: ");
}

Void Menu12(){
//         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Intentos Arr.");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"Arr: ");
}

Void Menu15(){
//         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"T Fa Modo Cont.");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"T9: ");
}

Void Menu16(){
//         lcd_putc('\f');
         lcd_gotoxy(5,1);
         printf(lcd_putc,"Cebador");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"TC: ");         
}

Void Menu17(){//operacion manual de la transferencia
//         lcd_putc('\f');
         lcd_gotoxy(5,1);
         printf(lcd_putc,"HS Marcha");
}
Void MenuParGr(){//operacion manual de la transferencia
//         lcd_putc('\f');
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Tipo Pare Gr");
}
//Refresco valores analogicos
//void RefrescaLect(){

//}

void Menu18(){//Menu Ajustes Tension de Grupo
//         lcd_putc('\f'); //limpiar display
         lcd_gotoxy(2,1);
         printf(lcd_putc,"AJUSTE V LINEA");//MAN

}

VOID Menu19(){
//         lcd_putc('\f'); //limpiar display        
         lcd_gotoxy(2,1);
         printf(lcd_putc,"AJUSTE V GRUPO");//MAN
}

void MenuManual(){//MENU NRO 13
//         lcd_putc('\f'); //limpiar display
         //Linea
         lcd_gotoxy(2,1);
         printf(lcd_putc,"Arranque Manual");
         lcd_gotoxy(1,2);
         printf(lcd_putc,"Arr. Grupo:");         
}






