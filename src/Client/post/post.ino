#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// ルーター接続情報
#define WIFI_SSID "VPN"
// TODO: パスワードをハッシュ化する
#define WIFI_PASSWORD "94272bb7cf7c77277edfbbd2f8985270911f6faa2fc8f029714af463bd943543"

const int ROOM1_BTN = 2;
boolean postFlag = false;

/* Prototype宣言 */
void connectToWifi();
void onPushed_ROOM1();

void setup() {
  Serial.begin(115200);
  // ピン情報設定
  pinMode(ROOM1_BTN, INPUT_PULLUP);
  attachInterrupt(ROOM1_BTN, onPushed_ROOM1, FALLING);  // 割り込み処理設定
  connectToWifi();
}

void loop() {
  if (digitalRead(ROOM1_BTN) == HIGH) {
    Serial.println("HIGH");
    post("1F-1", "空室");
  } else {
    Serial.println("LOW");
    post("1F-1", "満室");
  }
  delay(100);
}

/* Wi-Fiルーターに接続する */
void connectToWifi() {
  Serial.print("Connecting to Wi-Fi ");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  // モニターにローカル IPアドレスを表示する
  Serial.println("WiFi connected.");
  Serial.print("  *IP address: ");
  Serial.println(WiFi.localIP());
}

void post(String name, String state) {
  const size_t CAPACITY = 300;
  StaticJsonDocument<CAPACITY> json_doc;
  JsonObject json_object = json_doc.to<JsonObject>();
  json_object["name"] = name;
  json_object["state"] = state;
  // JSONをシリアライズ
  char json_buf[CAPACITY];
  serializeJsonPretty(json_doc, json_buf, CAPACITY);

  // 受信側のサーバへJSON形式で送信(POST)
  HTTPClient http;
  http.begin("http://192.168.2.89:3000");
  http.addHeader("Content-Type", "application/json");

  int http_code = http.POST(json_buf);

  if (http_code < 0) //Send the request
  {
    Serial.println("HTTP POST ERROR. CODE:" + String(http_code));
  }

  Serial.println(http.getString());
  http.end(); //Close connection
  delay(50);
}

// ボタン押下時割り込み処理
void IRAM_ATTR onPushed_ROOM1() {
  postFlag = true;
}
