if(!fLineaOk & !OArrGR & !fFallaGR){                  //Si hay falla de la linea
//Delay al arranque del grupo electrogeno
   if(p_1seg){
      if(--tmrDyArrGr==0){                            //si paso el tiempo de demora al arranque del grupo
            if(!fGrupoOK) OArrGR=1;                   //Inicio secuencia arranque grupo
            //Carga tiempos de arranque grupo
            TArr=preTArr;                             //Tiempo de arrnaque grupo
            TPausa=preTPausa;                         //Tiempo entre intentos de arranque
            fPausa=0;
            intentosArr=preIntArr;
            tmrDyArrGr=preDyArrGr;
            }
   }
}
else
   {
      tmrDyArrGr=preDyArrGr;
   }
