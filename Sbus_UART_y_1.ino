//凧氏のC++コードをCに書き直す
//プロポの表示範囲を-255から255に変更


int count;


void sbus(int data[]);
void check(int data[]);
void data_encode(int data[],int val[]);
int button(int);
int safe(int getData);


void setup() {
  Serial.begin(115200); // Terminal
  Serial1.begin(100000); // S-BUS
  count = 0;

}

void sbus(int sendData[]) {
  int data[26];
  int val[18];
  long interval;
  int i;

  if (Serial1.available() > 0) {
    data[count] = Serial1.read();
    interval = millis();
    count++;
  }

  if ((interval + 4 < millis()) && (0 < count) ) {
    count = 0;

    val[0] = ((data[1] & 0xff) << 0) + ((data[2] & 0x07) << 8) - 368;
    val[1] = ((data[2] & 0xf8) >> 3) + ((data[3] & 0x3f) << 5) - 368;
    val[2] = (((data[3] & 0xc0) >> 6) + ((data[4] & 0xff) << 2) + ((data[5] & 0x01) << 10) - 1680) * (-1);
    val[3] = ((data[5] & 0xfe) >> 1) + ((data[6] & 0x0f) << 7) - 368;
    val[4] = ((data[6] & 0x0f) >> 4) + ((data[7] & 0x7f) << 4);
    val[5] = ((data[7] & 0x80) >> 7) + ((data[8] & 0xff) << 1) + ((data[9] & 0x03) << 9);
    val[6] = ((data[9] & 0x7c) >> 2) + ((data[10] & 0x1f) << 6);
    val[7] = ((data[10] & 0xe0) >> 5) + ((data[11] & 0xff) << 3);
    val[8] = ((data[12] & 0xff) << 0) + ((data[13] & 0x07) << 8);
    val[9] = ((data[13] & 0xf8) >> 3) + ((data[14] & 0x3f) << 5);
    val[10] = ((data[14] & 0xc0) >> 6) + ((data[15] & 0xff) << 2) + ((data[16] & 0x01) << 10);
    val[11] = ((data[16] & 0xfe) >> 1) + ((data[17] & 0x0f) << 7);
    val[12] = ((data[17] & 0x0f) >> 4) + ((data[18] & 0x7f) << 4);
    val[13] = ((data[18] & 0x80) >> 7) + ((data[19] & 0xff) << 1) + ((data[20] & 0x03) << 9);
    val[14] = ((data[20] & 0x7c) >> 2) + ((data[21] & 0x1f) << 6);
    val[15] = ((data[21] & 0xe0) >> 5) + ((data[22] & 0xff) << 3);
    val[16] = (data[23] & 0x1) ? 0x7ff : 0 ;
    val[17] = (data[23] & 0x2) ? 0x7ff : 0 ;
    val[18] = (data[23] & 0x8) ? 0x7ff : 0 ; // Failsafe

    data_encode(sendData,val);
//    check(sendData);
  }
  check(sendData);  
}

void data_encode(int sendData[],int val[]) {
  
  if (val[18] == 0) {
    sendData[0] = map(val[0], 0, 1312, -255, 255);
    sendData[0] = safe(sendData[0]);
    sendData[1] = -(map(val[1], 0, 1312, -255, 255));
    sendData[1] = safe(sendData[1]);
    sendData[2] = -(map(val[2], 0, 1312, -255, 255));
    sendData[2] = safe(sendData[2]);
    sendData[3] = map(val[3], 0, 1318, -255, 255);
    sendData[3] = safe(sendData[3]);
    sendData[4] = button(val[4]);
    sendData[5] = button(val[5]);
  }else {
    sendData[0] = 0;
    sendData[1] = 0;
    sendData[2] = 0;
    sendData[3] = 0;
    sendData[4] = 0;
    sendData[5] = 0;
  }

//  check(sendData);

}

int safe(int getData) {
  if (getData <= 5 && getData >= -5) {
    getData = 0;
  }
  return getData;
}

void check(int sendData[]) {
  int i;
  for (i = 0 ; i < 5; i++ ) {
    Serial.print(sendData[i], DEC);
    Serial.print(F(" "));
  }
  Serial.println();
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

double mecanumCon(int lx, int ly) {
  float y = (float)ly / 255;
  float x = (float)lx / 255;
  //Serial.println(y,5);

  double Direction = atan2(y, x) ;
  double Distance = sqrt(pow(x, 2) + pow(y, 2));

  double FR = (sin(Direction - PI / 4)) * 255;
  double FL = (sin(Direction + PI / 4)) * 255;
  double BR = (sin(Direction + PI / 4)) * 255;
  double BL = (sin(Direction - PI / 4)) * 255;
  /*
    Serial.print("FR  ");
    Serial.print(FR);
    Serial.print("  FL : ");
    Serial.print(FL);
    Serial.print("  BR : ");
    Serial.print(BR);
    Serial.print("  BL : ");
    Serial.println(BL);
  */
}

void loop() {
  static int sendData[10];
  sbus(sendData);
//  check(sendData);
  //  Serial.print(sendData[1], DEC);
  //  Serial.print(F(" "));
  //  Serial.println("");

  //mecanumCon(sendData[1], sendData[3]);
}
