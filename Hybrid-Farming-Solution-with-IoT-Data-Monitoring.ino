/************************************************************
* Project  : Hybrid-Farming-Solution-with-IoT-Data-Monitoring

* Version  : PRv001
* Board    : NodeMCU ESP8266
*
* Sensors  : DHT11 (Temp/Humidity), LDR (Light), Soil Moisture (DO)
* Outputs  : Water Pump Relay, UV Light Relay
* Display  : 16x2 LCD via I2C (addr 0x27)
* Network  : ESP8266 SoftAP + JSON dashboard at 192.168.4.1
*
* Notes:
* - Auto-Watering: Pump ON when soil is dry (DO=HIGH on many modules).
* - UV: UV ON when it’s dark (LDR below threshold).
* - LCD shows Temp/Humidity plus UV/Pump status.
* - Connect phone to AP and open 192.168.4.1.
************************************************************/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SimpleDHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/********** SoftAP config **********/
const char* AP_SSID = "Garden_AP";
const char* AP_PASS = "12345678";
IPAddress AP_IP(192,168,4,1);
IPAddress AP_GW(192,168,4,1);
IPAddress AP_MASK(255,255,255,0);

/********** Pin map (ESP8266) **********
* D4 = GPIO2  (DHT11 data)
* A0 = ADC    (LDR analog)
* D5 = GPIO14 (Soil DO)
* D7 = GPIO13 (UV Relay)
* D8 = GPIO15 (Pump Relay)
* I2C: SDA=D2(GPIO4), SCL=D1(GPIO5)
*****************************************/
#define DHTPIN D4
#define LDR_PIN A0
#define SOIL_DO_PIN D5
#define UV_RELAY_PIN D7
#define PUMP_RELAY_PIN D8

/********** Relay logic **********
* Many relay modules are ACTIVE-LOW.
* If yours are active-low, keep ACTIVE_LOW = true.
* For active-high relays, set ACTIVE_LOW = false.
**********************************/
const bool ACTIVE_LOW = true;

SimpleDHT11 dht11;
LiquidCrystal_I2C lcd(0x27, 16, 2);
ESP8266WebServer server(80);

/********** Live variables **********/
float tempC = 0, hum = 0;
int   ldrValue = 0;
bool  isDark = false;
bool  soilDry = false;
bool  uvOn = false;
bool  pumpOn = false;

/********** Tuning **********/
int LDR_THRESHOLD = 450;  // lower = darker on ESP8266 (0..1023)

/********** HTML UI (Animated + Clean) **********/
const char page[] PROGMEM = R"HTML(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <title>Smart Garden LIVE Dashboard</title>
  <meta name="viewport" content="width=device-width,initial-scale=1">
  <style>
    :root{
      --bg1:#0e1b2c; --bg2:#0b1320; --glass:#14243caa; --card:#14243cf0;
      --pri:#16e0a4; --sec:#5df2ff; --mut:#99b3c7; --on:#20df7f; --off:#ff6060;
      --chip:#0f1f35; --chipbr:#1c3354;
    }
    *{box-sizing:border-box}
    html,body{height:100%;margin:0}
    body{
      font-family:Arial,Helvetica,sans-serif; color:#fff; text-align:center;
      background: radial-gradient(1200px 1200px at 20% 10%, #1a355a44, transparent 60%),
                  linear-gradient(120deg, var(--bg1), var(--bg2));
      animation:bgShift 12s ease-in-out infinite alternate;
      padding:18px;
    }
    @keyframes bgShift{
      0%{background-position:0 0, 0 0}
      100%{background-position:30px -30px, 100px 60px}
    }
    .wrap{max-width:920px;margin:0 auto}
    h1{
      margin:0 0 4px 0; font-size:22px; color:var(--pri); letter-spacing:.5px;
      text-shadow:0 2px 12px #16e0a455;
    }
    .subtitle{font-size:13px;color:var(--mut);margin-bottom:14px}
    .card{
      background:var(--card);
      border:1px solid #244164;
      border-radius:16px;
      padding:18px;
      box-shadow: 0 20px 50px #091326aa, inset 0 1px 0 #2a4669;
      backdrop-filter: blur(6px);
      transform: translateZ(0);
      animation: floatIn .6s ease-out both;
    }
    @keyframes floatIn{
      from{opacity:0; transform:translateY(10px)}
      to{opacity:1; transform:translateY(0)}
    }
    .grid{
      display:grid; gap:14px;
      grid-template-columns: repeat(auto-fit, minmax(180px,1fr));
      margin-top:12px;
    }
    .tile{
      background:var(--glass); border:1px solid #1f3a5a; border-radius:14px;
      padding:16px 14px; min-height:96px;
      transition: transform .2s ease, box-shadow .2s ease, border-color .2s ease;
    }
    .tile:hover{ transform: translateY(-2px); box-shadow:0 12px 26px #0a1628aa; border-color:#2b5486; }
    .k{font-size:12px; color:var(--mut); text-transform:uppercase; letter-spacing:.8px}
    .v{font-size:26px; font-weight:800; color:var(--sec); margin-top:6px}
    .unit{font-size:14px; opacity:.9}
    .chip{
      display:inline-flex; align-items:center; gap:8px;
      padding:8px 12px; border-radius:999px; font-weight:700; font-size:13px;
      background:var(--chip); border:1px solid var(--chipbr); margin:6px 6px 0 6px;
      box-shadow: inset 0 0 0 1px #203651;
    }
    .dot{width:10px; height:10px; border-radius:50%;}
    .on .dot{background:var(--on); animation:pulse 1.6s ease-in-out infinite}
    .off .dot{background:var(--off); opacity:.9}
    @keyframes pulse{
      0%{box-shadow:0 0 0 0 rgba(32,223,127,.5)}
      70%{box-shadow:0 0 0 10px rgba(32,223,127,0)}
      100%{box-shadow:0 0 0 0 rgba(32,223,127,0)}
    }
    .flash{ animation:flash .35s ease; }
    @keyframes flash{ 0%{filter:brightness(1.6)} 100%{filter:brightness(1)} }
  </style>
</head>
<body>
  <div class="wrap">
    <h1>Smart Garden LIVE Dashboard</h1>
    <div class="subtitle">Real-time Temp • Humidity • LDR • Soil • UV • Pump</div>

    <div class="card">
      <div class="grid">
        <div class="tile">
          <div class="k">Temperature</div>
          <div class="v"><span id="t">--</span><span class="unit"> °C</span></div>
        </div>
        <div class="tile">
          <div class="k">Humidity</div>
          <div class="v"><span id="h">--</span><span class="unit"> %</span></div>
        </div>
        <div class="tile">
          <div class="k">Light (LDR)</div>
          <div class="v"><span id="l">--</span></div>
        </div>
        <div class="tile">
          <div class="k">Soil</div>
          <div class="v"><span id="s">--</span></div>
        </div>
      </div>

      <div style="margin-top:10px">
        <span id="chipPump" class="chip off"><span class="dot"></span>Pump: <span id="p">OFF</span></span>
        <span id="chipUV" class="chip off"><span class="dot"></span>UV: <span id="u">OFF</span></span>
      </div>
    </div>
  </div>

  <script>
    function setTextFlash(id, val){
      const el = document.getElementById(id);
      if(el.textContent != String(val)){
        el.textContent = val;
        el.classList.remove('flash');
        void el.offsetWidth;
        el.classList.add('flash');
      }
    }
    function setChip(idChip, idText, isOn){
      const chip = document.getElementById(idChip);
      const txt = document.getElementById(idText);
      txt.textContent = isOn ? "ON" : "OFF";
      chip.classList.toggle('on',  !!isOn);
      chip.classList.toggle('off', !isOn);
    }
    setInterval(()=>{
      fetch('/data')
        .then(r=>r.json())
        .then(j=>{
          setTextFlash('t', j.t);
          setTextFlash('h', j.h);
          setTextFlash('l', j.l);
          setTextFlash('s', j.s ? "DRY" : "WET");
          setChip('chipPump','p', !!j.p);
          setChip('chipUV','u',   !!j.u);
        })
        .catch(()=>{});
    }, 1500);
  </script>
</body>
</html>
)HTML";

/******************** Helpers ********************/
inline void relayWrite(uint8_t pin, bool on){
  if (ACTIVE_LOW) digitalWrite(pin, on ? LOW : HIGH);
  else            digitalWrite(pin, on ? HIGH : LOW);
}

void updateLCD(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("T:"); lcd.print(tempC); lcd.print("C ");
  lcd.print("H:"); lcd.print(hum);   lcd.print("%");

  lcd.setCursor(0,1);
  lcd.print("PUMP:"); lcd.print(pumpOn ? "ON " : "OFF");
  lcd.print(" UV:");  lcd.print(uvOn   ? "ON"  : "OFF"); // fixed label
}

void readSensors(){
  byte t=0, h=0;
  if(dht11.read(DHTPIN,&t,&h,NULL) == SimpleDHTErrSuccess){
    tempC = t; hum = h;
  }
  ldrValue = analogRead(LDR_PIN);
  isDark = (ldrValue < LDR_THRESHOLD);
  soilDry = digitalRead(SOIL_DO_PIN); // HIGH means dry on many DO modules
}

void controlSystem(){
  uvOn = isDark;
  pumpOn = soilDry;
  relayWrite(UV_RELAY_PIN,   uvOn);
  relayWrite(PUMP_RELAY_PIN, pumpOn);
}

void handleRoot(){ server.send_P(200,"text/html",page); }

void handleData(){
  String json = "{";
  json += "\"t\":"+String(tempC)+",";
  json += "\"h\":"+String(hum)+",";
  json += "\"l\":"+String(ldrValue)+",";
  json += "\"s\":"+String(soilDry?1:0)+",";
  json += "\"p\":"+String(pumpOn?1:0)+",";
  json += "\"u\":"+String(uvOn?1:0);
  json += "}";
  server.send(200,"application/json",json);
}

/******************** Setup/Loop ********************/
void setup(){
  pinMode(SOIL_DO_PIN,INPUT);
  pinMode(UV_RELAY_PIN,OUTPUT);
  pinMode(PUMP_RELAY_PIN,OUTPUT);
  relayWrite(UV_RELAY_PIN,false);
  relayWrite(PUMP_RELAY_PIN,false);

  Serial.begin(115200);
  Serial.println();
  Serial.println("System Boot...");

  Wire.begin(D2,D1);
  lcd.init(); lcd.backlight();
  lcd.print("Smart Garden Ready");

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(AP_IP,AP_GW,AP_MASK);
  WiFi.softAP(AP_SSID,AP_PASS);

  server.on("/",handleRoot);
  server.on("/data",handleData);
  server.begin();

  Serial.println("Hotspot IP: 192.168.4.1");
  Serial.print("SSID: "); Serial.println(AP_SSID);
}

void loop(){
  server.handleClient();
  readSensors();
  controlSystem();
  updateLCD();
  delay(1000);
}
