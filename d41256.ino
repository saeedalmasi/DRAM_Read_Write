// تعریف پین‌ها
const int addressPins[9] = {22, 23, 24, 25, 26, 27, 28, 29, 30}; // پین‌های آدرس A0 تا A8
const int dataInPin = 31; // پین داده ورودی
const int dataOutPin = 32; // پین داده خروجی
const int pinRAS = 33; // پین /RAS
const int pinCAS = 34; // پین /CAS
const int pinWE = 35;  // پین /WE

void setup() {
  // تنظیم پین‌های آدرس به عنوان خروجی
  for (int i = 0; i < 9; i++) {
    pinMode(addressPins[i], OUTPUT);
    digitalWrite(addressPins[i], LOW);
  }

  // تنظیم پین‌های داده
  pinMode(dataInPin, OUTPUT);
  pinMode(dataOutPin, INPUT);

  // تنظیم پین‌های کنترلی
  pinMode(pinRAS, OUTPUT);
  pinMode(pinCAS, OUTPUT);
  pinMode(pinWE, OUTPUT);

  // غیرفعال کردن سیگنال‌های کنترلی در حالت اولیه
  digitalWrite(pinRAS, HIGH);
  digitalWrite(pinCAS, HIGH);
  digitalWrite(pinWE, HIGH);

  // شروع ارتباط سریال برای نمایش داده‌ها
  Serial.begin(115200);

  Serial.println("Writing");
  for (int a = 0; a < 262; a++) {
    for (int b = 0; b < 144; b++) {
      writeDataToDRAM(a, b, 1);
    }
  }
}

void loop() {
  for (int a = 0; a < 262; a++) {
    for (int b = 0; b < 144; b++) {
      int data = readDataFromDRAM(a, b);
      Serial.print("Data read from address\t");
      Serial.print(a, HEX);
      Serial.print("\t");
      Serial.print(b, HEX);
      Serial.print(":\t");
      Serial.println(data);
      delay(2);
    }
  }
}

void writeDataToDRAM(int rowAddr, int colAddr, int data) {
  // تنظیم آدرس سطر
  setAddress(rowAddr);
  digitalWrite(pinRAS, LOW);
  delayMicroseconds(1); // تاخیر برای انتخاب سطر
  digitalWrite(pinWE, LOW); // شروع نوشتن
  delayMicroseconds(5); // تاخیر کوتاه برای همزمانی /WE و /RAS

  // تنظیم آدرس ستون
  setAddress(colAddr);
  digitalWrite(pinCAS, LOW);
  delayMicroseconds(1); // تاخیر برای انتخاب ستون و نوشتن داده

  // نوشتن داده به پین داده
  digitalWrite(dataInPin, data);
  //  delayMicroseconds(1); // تاخیر برای پایداری داده

  // پایان نوشتن
  digitalWrite(pinWE, HIGH);
  delayMicroseconds(1);
  digitalWrite(pinCAS, HIGH);
  digitalWrite(pinRAS, HIGH);
}

int readDataFromDRAM(int rowAddr, int colAddr) {
  int data = 0;

  digitalWrite(pinCAS, HIGH);
  // تنظیم آدرس سطر
  setAddress(rowAddr);
  digitalWrite(pinRAS, LOW);

  digitalWrite(pinWE, HIGH);
  // تنظیم آدرس ستون و فعال‌سازی /CAS
  setAddress(colAddr);
  digitalWrite(pinCAS, LOW);
  delayMicroseconds(1); // تاخیر برای انتخاب ستون و خواندن داده

  // خواندن داده از پین خروجی
  data = digitalRead(dataOutPin);

  // پایان خواندن
  delayMicroseconds(5); // تاخیر برای پایداری داده
  digitalWrite(pinCAS, HIGH);
  digitalWrite(pinRAS, HIGH);

  return data;
}

void setAddress(int addr) {
  for (int i = 0; i < 9; i++) {
    digitalWrite(addressPins[i], (addr >> i) & 0x01);
  }
}
