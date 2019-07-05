//2019-07-04　modify by yukimakura
//凧氏のC++コードをCに書き直す
//プロポの表示範囲を-255から255に変更

int button(int);
int safe(int getData);

typedef struct {
  double lx,ly,rx;
}DIR;

class SBUS{
  private:
    int data_[26];
    int val_[18];
    long interval_;
    int count_;
    int send_data_[10];


    void data_to_val_();
    void read_sentence_();
    void data_encode_();

  public:
    SBUS(){
      count_ = 0;//オブジェクト読み出し時の初期化
    }
    _SBUS(){
    }
    void loop_do(void (*move_func)());
    void print_send_data();
    DIR get_dir(){
      DIR d;
      d.lx = send_data_[1];
      d.ly = send_data_[3];
      d.rx = send_data_[0];
      return d;
    }
    void get_send_data(int s_d[]){
      s_d = send_data_;
    }
    void count_init(){
      count_ = 0;
    }
    int get_count(){
      return count_;
    }

};

void SBUS::read_sentence_(){
  if (Serial1.available() > 0) {
    data_[count_] = Serial1.read();
    interval_ = millis();
    count_++;
  }
}

void SBUS::data_to_val_(){
  val_[0] = ((data_[1] & 0xff) << 0) + ((data_[2] & 0x07) << 8) - 368;
  val_[1] = ((data_[2] & 0xf8) >> 3) + ((data_[3] & 0x3f) << 5) - 368;
  val_[2] = (((data_[3] & 0xc0) >> 6) + ((data_[4] & 0xff) << 2) + ((data_[5] & 0x01) << 10) - 1680) * (-1);
  val_[3] = ((data_[5] & 0xfe) >> 1) + ((data_[6] & 0x0f) << 7) - 368;
  val_[4] = ((data_[6] & 0x0f) >> 4) + ((data_[7] & 0x7f) << 4);
  val_[5] = ((data_[7] & 0x80) >> 7) + ((data_[8] & 0xff) << 1) + ((data_[9] & 0x03) << 9);
  val_[6] = ((data_[9] & 0x7c) >> 2) + ((data_[10] & 0x1f) << 6);
  val_[7] = ((data_[10] & 0xe0) >> 5) + ((data_[11] & 0xff) << 3);
  val_[8] = ((data_[12] & 0xff) << 0) + ((data_[13] & 0x07) << 8);
  val_[9] = ((data_[13] & 0xf8) >> 3) + ((data_[14] & 0x3f) << 5);
  val_[10] = ((data_[14] & 0xc0) >> 6) + ((data_[15] & 0xff) << 2) + ((data_[16] & 0x01) << 10);
  val_[11] = ((data_[16] & 0xfe) >> 1) + ((data_[17] & 0x0f) << 7);
  val_[12] = ((data_[17] & 0x0f) >> 4) + ((data_[18] & 0x7f) << 4);
  val_[13] = ((data_[18] & 0x80) >> 7) + ((data_[19] & 0xff) << 1) + ((data_[20] & 0x03) << 9);
  val_[14] = ((data_[20] & 0x7c) >> 2) + ((data_[21] & 0x1f) << 6);
  val_[15] = ((data_[21] & 0xe0) >> 5) + ((data_[22] & 0xff) << 3);
  val_[16] = (data_[23] & 0x1) ? 0x7ff : 0 ;
  val_[17] = (data_[23] & 0x2) ? 0x7ff : 0 ;
  val_[18] = (data_[23] & 0x8) ? 0x7ff : 0 ; // Failsafe
}

void SBUS::loop_do(void (*move_func)()) {
  
  int i;
  read_sentence_();
  if ((interval_ + 4 < millis()) && (0 < count_) ) {
    count_ = 0;
    data_to_val_();
    data_encode_();
    move_func();
//    check(send_data_);
  }
}

void SBUS::data_encode_() {
  
  if (val_[18] == 0) {
    send_data_[0] = map(val_[0], 0, 1312, -255, 255);
    send_data_[0] = safe(send_data_[0]);
    send_data_[1] = -(map(val_[1], 0, 1312, -255, 255));
    send_data_[1] = safe(send_data_[1]);
    send_data_[2] = -(map(val_[2], 0, 1312, -255, 255));
    send_data_[2] = safe(send_data_[2]);
    send_data_[3] = map(val_[3], 0, 1318, -255, 255);
    send_data_[3] = safe(send_data_[3]);
    send_data_[4] = button(val_[4]);
    send_data_[5] = button(val_[5]);
  }else {
    send_data_[0] = 0;
    send_data_[1] = 0;
    send_data_[2] = 0;
    send_data_[3] = 0;
    send_data_[4] = 0;
    send_data_[5] = 0;
  }

//  check(send_data_);

}

void SBUS::print_send_data() {
  int i;
//  for (i = 0 ; i < 5; i++ ) {
//    Serial.print(send_data_[i], DEC);
//    Serial.print(F(" "));
//  }
//  Serial.println();
}

int safe(int getData) {
  if (getData <= 5 && getData >= -5) {
    getData = 0;
  }
  return getData;
}

int button(int num) {
  int back;
  switch (num) {
    case 144:
      back = 0;
      break;

    case 1024:
      back = 1;
      break;

    case 1904:
      back = 2;
      break;

    default:
      break;
  }

  return back;
}

void mecanumCon(float lx, float ly ,float rx) {
  int AMP=127;
   ly = (float)ly / 255.0;
   lx = (float)lx / 255.0;
   rx = (float)rx / 255.0;

 char printbuff[128] = "";

  float power = 0;
  float rad = 0.0;
  power = fabs(lx + ly);
  rad = atan2(ly, lx);
  if (ly <= 0) {
    rad += 6.28;
  }
//    Serial.print("power = ");
//    Serial.print(power);
//    Serial.print("rad = ");
//    Serial.println(rad);

  float FR = (char)((sin(rad) - cos(rad)) * power * AMP + (rx) * AMP);
  float BL = (char)((sin(rad) - cos(rad)) * power * AMP - (rx) * AMP);
  float FL = (char)((sin(rad) + cos(rad)) * power * AMP - (rx) * AMP);
  float BR = (char)((sin(rad) + cos(rad)) * power * AMP + (rx) * AMP);

  Serial.print(FR);
  Serial.print("  ");
  Serial.print(FL);
  Serial.print("  ");
  Serial.print(BR);
  Serial.print("  ");
  Serial.print(BL);
  Serial.println("  ");

}

SBUS sbus;


void setup() {
  Serial.begin(115200); // Terminal
  Serial1.begin(100000); // S-BUS

}

void loop() {
  sbus.loop_do(do_something);
  
}

void do_something(){
//  int send_data[10]; 
  DIR dir;
  sbus.print_send_data();
  dir = sbus.get_dir();
  mecanumCon(dir.lx,dir.ly,dir.rx);
  
}
