/*#ifdef COMMANDAUTO
void Cmd_auto(){
  Serial.println("#Auto command interpreter");
  int temp;
  char *arg;
  arg = SCmd.next(); 
  if (arg != NULL){
    temp=atoi(arg);
  }else{
    Serial.println("#Temperature missing");
    ParseResult(-5);
    return ;
  }
  ParseResult(Cmd_Auto_Temp(temp));
}
#endif*/
