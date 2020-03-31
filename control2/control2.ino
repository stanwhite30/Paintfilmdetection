
#define X A8
#define Y A10
#define Z A9
#define light A14

#define red A13
#define green A12
// 0 有效
#define X_head 24
#define Y_head 26
#define Z_head 22

#define X_end 25
#define Y_end 27
#define Z_end 23

#define control_switch 28
#define light_switch 30
String comdata = "";
int flag_switch=0;
int light_flag = 0;
void setup() {
  
  Serial.begin(115200);
  //Output
  pinMode(X,OUTPUT);
  pinMode(Y,OUTPUT);
  pinMode(Z,OUTPUT);
  pinMode(light,OUTPUT);
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);

  
//传感器
  pinMode(X_head,INPUT);
  pinMode(Y_head,INPUT);
  pinMode(Z_head,INPUT);
  pinMode(X_end,INPUT);
  pinMode(Y_end,INPUT);
  pinMode(Z_end,INPUT);
  //开关
  pinMode(control_switch,INPUT);
  pinMode(light_switch,INPUT);
  Serial.println("开始");
  //digitalWrite(Z,HIGH);
  delay(3000);
  digitalWrite(green,LOW);
  digitalWrite(red,LOW);
  

  
  Serial.println("等待开关按下");
}

void loop()
{

      while (Serial.available() > 0)  
    {
        comdata += char(Serial.read());
        delay(2);
    }

    if(comdata.startsWith("curve"))
    {
      flag_switch = 1;
      }    
  
    else if(comdata.startsWith("light"))
    {
        if(comdata.endsWith("on"))
        {
            digitalWrite(light,HIGH);
        }
          else if(comdata.endsWith("off"))
          {
            digitalWrite(light,LOW);
          }
      }
      else if(comdata.startsWith("adjust"))
      {
             if(comdata.endsWith("on"))
              {
                //digitalWrite(Z,HIGH);
                
                while(digitalRead(Z_head) == 0);
                if(digitalRead(Z_head) == 1)
                {
                  digitalWrite(Z,HIGH);
                  digitalWrite(green,HIGH);
                  digitalWrite(red,LOW);
                  Serial.println("等待Z轴放下");
                  
                }
                
                delay(2000);
                digitalWrite(green,LOW);
                digitalWrite(red,HIGH);
                Serial.println("Z轴放下");
                delay(1000);
                
              }
            else if(comdata.endsWith("off"))
            {
                digitalWrite(Z,LOW);
                digitalWrite(red,LOW);
                
                //digitalWrite(green,LOW);
                //digitalWrite(red,HIGH);
                Serial.println("等待Z轴收回");
                while(digitalRead(Z_head) == 0);
                if(digitalRead(Z_head) == 1) Serial.println("Z轴收回");
                delay(2000);
                
            }
        }

    if(flag_switch)
    { 

      Serial.println("等待X轴推出");
      while(digitalRead(X_head) == 0);
      if(digitalRead(X_head) == 1)
      {
        Serial.println("X轴推出");
        digitalWrite(X,HIGH);
        Serial.println("等待X轴到位");
        while(digitalRead(X_end) == 0);
        Serial.println("X轴到位");
        delay(1000);
       }
       
      delay(1000);
      Serial.println("等待Z轴放下");
      while(digitalRead(Z_head) == 0);
      if(digitalRead(Z_head) == 1)
      {
        digitalWrite(Z,HIGH);
        digitalWrite(green,HIGH);
        Serial.println("Z轴放下");
      }
      Serial.println("等待Z轴到位");
      delay(4000);
      digitalWrite(green,LOW);
      digitalWrite(red,HIGH);
      
      Serial.println("Z轴到位成功");
      delay(1000);
      
      digitalWrite(X,LOW);
      Serial.println("等待X轴收回");
      while(digitalRead(X_head) == 0);        
      Serial.println("X轴收回成功");   
      delay(2000);

//收回Z，再收回X
        Serial.println("等待Z轴收回");
        digitalWrite(Z,LOW);
        digitalWrite(red,LOW);
        while(digitalRead(Z_head) == 0);
        Serial.println("Z轴收回");
        delay(1000);
    
//Y轴
      Serial.println("等待Y轴推出");
      while(digitalRead(Y_head) == 0); 
      if(digitalRead(Y_head) == 1)
      {
        Serial.println("Y轴推出");
        digitalWrite(Y,HIGH);
        Serial.println("等待Y轴到位");
        while(digitalRead(Y_end) == 0);
        Serial.println("Y轴到位");
        delay(1000);
      }
      
      
      Serial.println("等待Z轴放下");
      while(digitalRead(Z_head) == 0);
      if(digitalRead(Z_head) == 1)
      {
        digitalWrite(Z,HIGH);
        digitalWrite(green,HIGH);
        Serial.println("Z轴放下");
      }
      Serial.println("等待Z轴到位");

      delay(4000);
      digitalWrite(green,LOW);
      digitalWrite(red,HIGH);
      
        digitalWrite(Y,LOW);
        Serial.println("等待Y轴收回");
        while(digitalRead(Y_head) == 0) ;
        Serial.println("Y轴收回成功");
        
        delay(2000);
        //收回Z，再收回Y
        Serial.println("等待Z轴收回");
        digitalWrite(Z,LOW);
        digitalWrite(red,LOW);
        while(digitalRead(Z_head) == 0);
        Serial.println("Z轴收回");
        delay(1000);
        
  


       flag_switch = 0;
      Serial.println("y");
      
      }
    
  
      if (comdata.length() > 0)
    {
        Serial.println(comdata);
        comdata = "";
    }

}
