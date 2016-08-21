#ifdef COMMANDHEAT
int Cmd_Heat_Fan_Temp(int fan, int temp){
  if(fan<0){ return -3; }
  if(fan>4){ return -4; }   
  
       if(fan==0){ return Cmd_Heat_Fan0_Temp(temp); }
  else if(fan==1){ return Cmd_Heat_Fan1_Temp(temp); }
  else if(fan==2){ return Cmd_Heat_Fan2_Temp(temp); }
  else if(fan==3){ return Cmd_Heat_Fan3_Temp(temp); }
  else if(fan==4){ return Cmd_Heat_Fan4_Temp(temp); }
}

#endif
