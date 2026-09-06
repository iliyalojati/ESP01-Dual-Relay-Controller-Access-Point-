#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/* ============================================================
   ⚙️  بخش تنظیمات — این قسمت رو خودت تغییر بده
   ============================================================ */

// 📡 نام و رمز شبکهی Access Point
const char* AP_SSID   = "ESP01_Control";
const char* AP_PASS   = "12345678";

// 🎛️ پایههای رله
#define RELAY1_PIN   0      // GPIO0
#define RELAY2_PIN   2      // GPIO2

// 💡 نوع رله: اگر رله با LOW روشن می‌شود (رایج)، روی LOW بماند
#define RELAY_ACTIVE_LEVEL  LOW   

/* ============================================================
   🧠 متغیرهای داخلی
   ============================================================ */
bool state1 = false;
bool state2 = false;

ESP8266WebServer server(80);

/* ============================================================
   📱 صفحه وب (دقیقاً همان استایل زیبا و مدرن شما)
   ============================================================ */
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fa" dir="rtl">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>کنترل هوشمند</title>
<style>
  * { margin:0; padding:0; box-sizing:border-box; }

  body {
    font-family: 'Segoe UI', Tahoma, sans-serif;
    min-height:100vh;
    background: linear-gradient(135deg, #0f2027, #203a43, #2c5364);
    color:#fff;
    display:flex;
    justify-content:center;
    align-items:center;
    padding:20px;
  }

  .card {
    background: rgba(255,255,255,0.08);
    backdrop-filter: blur(10px);
    border:1px solid rgba(255,255,255,0.15);
    border-radius:24px;
    padding:35px 30px;
    width:100%;
    max-width:420px;
    box-shadow: 0 20px 50px rgba(0,0,0,0.5);
  }

  h1 {
    text-align:center;
    font-size:1.6rem;
    margin-bottom:5px;
    background: linear-gradient(90deg,#00e5ff,#8e2de2);
    -webkit-background-clip:text;
    background-clip:text; /* اصلاح برای استاندارد */
    -webkit-text-fill-color:transparent;
    color:transparent; /* اصلاح برای استاندارد */
  }

  .subtitle {
    text-align:center;
    color:#9fb6c8;
    font-size:0.85rem;
    margin-bottom:30px;
  }

  .row {
    display:flex;
    align-items:center;
    justify-content:space-between;
    background: rgba(255,255,255,0.05);
    padding:16px 20px;
    border-radius:16px;
    margin-bottom:16px;
    border:1px solid rgba(255,255,255,0.08);
    transition: all .3s;
  }

  .row.active-row-1 { border-color:#00e676; box-shadow:0 0 20px rgba(0,230,118,.25); }
  .row.active-row-2 { border-color:#ff4081; box-shadow:0 0 20px rgba(255,64,129,.25); }

  .info {
    display:flex;
    flex-direction:column;
    gap:4px;
  }

  .name { font-size:1.1rem; font-weight:600; }
  .status { font-size:0.75rem; color:#8fa7b8; }

  .status .dot {
    display:inline-block;
    width:8px; height:8px; border-radius:50%;
    background:#777; margin-left:6px;
    transition: all .3s;
  }

  .row.active-row-1 .dot { background:#00e676; box-shadow:0 0 8px #00e676; }
  .row.active-row-2 .dot { background:#ff4081; box-shadow:0 0 8px #ff4081; }

  .switch {
    position:relative;
    width:64px; height:34px;
    -webkit-appearance:none;
    appearance:none;
    background:#3a4a55;
    border-radius:34px;
    outline:none;
    cursor:pointer;
    transition: background .3s;
    box-shadow: inset 0 2px 6px rgba(0,0,0,.4);
  }

  .switch::before {
    content:"";
    position:absolute;
    width:26px; height:26px;
    border-radius:50%;
    top:4px; left:4px;
    background:#cfd8dc;
    transition: all .3s ease;
    box-shadow: 0 2px 5px rgba(0,0,0,.4);
  }

  .switch:checked { background:#00e676; }
  .switch:checked::before { transform: translateX(30px); background:#fff; }
  .switch#sw2:checked { background:#ff4081; }

  .footer {
    text-align:center;
    font-size:0.7rem;
    color:#5f7d8c;
    margin-top:20px;
  }
</style>
</head>
<body>

<div class="card">
  <h1>⚡ کنترل هوشمند</h1>
  <p class="subtitle">ESP-01 Access Point</p>

  <div class="row" id="row1">
    <div class="info">
      <span class="name">مصرف کننده ۱</span>
      <span class="status"><span class="dot"></span><span id="st1">OFF</span></span>
    </div>
    <input type="checkbox" class="switch" id="sw1" onclick="sendCmd(1, this.checked)">
  </div>

  <div class="row" id="row2">
    <div class="info">
      <span class="name">مصرف کننده ۲</span>
      <span class="status"><span class="dot"></span><span id="st2">OFF</span></span>
    </div>
    <input type="checkbox" class="switch" id="sw2" onclick="sendCmd(2, this.checked)">
  </div>

  <div class="footer">ESP-01 · پروژهی کنترل از راه دور</div>
</div>

<script>
  function sendCmd(n, on) {
    const val = on ? 1 : 0;
    fetch('/cmd?r='+n+'&v='+val)
      .then(r => r.json())
      .then(d => {
        if (d.ok) { updateUI(d); } 
        else { alert('خطا در ارسال فرمان'); }
      }).catch(() => alert('اتصال قطع شد!'));
  }

  function updateUI(d) {
    document.getElementById('sw1').checked = d.r1;
    document.getElementById('sw2').checked = d.r2;
    document.getElementById('st1').textContent = d.r1 ? 'ON' : 'OFF';
    document.getElementById('st2').textContent = d.r2 ? 'ON' : 'OFF';
    document.getElementById('row1').classList.toggle('active-row-1', d.r1);
    document.getElementById('row2').classList.toggle('active-row-2', d.r2);
  }

  fetch('/state').then(r => r.json()).then(updateUI);
</script>

</body>
</html>
)rawliteral";

/* ============================================================
   🛠️ توابع کمکی
   ============================================================ */

void setRelay(int pin, bool on) {
  // اگر رله Active LOW باشد، برای روشن کردن باید LOW بفرستیم
  if (RELAY_ACTIVE_LEVEL == LOW)
    digitalWrite(pin, on ? LOW : HIGH);
  else
    digitalWrite(pin, on ? HIGH : LOW);
}

void sendState() {
  String json = "{\"ok\":true,\"r1\":" + String(state1) +
                ",\"r2\":" + String(state2) + "}";
  server.send(200, "application/json", json);
}

void handleRoot() {
  server.send_P(200, "text/html", index_html);
}

void handleCmd() {
  int r = server.arg("r").toInt();
  int v = server.arg("v").toInt();

  if (r == 1) {
    state1 = (v == 1);
    setRelay(RELAY1_PIN, state1);
  } else if (r == 2) {
    state2 = (v == 1);
    setRelay(RELAY2_PIN, state2);
  }
  sendState();
}

void handleState() {
  sendState();
}

/* ============================================================
   🚀 setup و loop
   ============================================================ */
void setup() {
  Serial.begin(115200);
  delay(100);

  // --- 🛡️ بخش حیاتی برای جلوگیری از کلیک رله هنگام بوت ---
  
  // ۱. ابتدا وضعیت "خاموش" را مستقیماً روی پایه اعمال می‌کنیم 
  // (اگر رله Active LOW باشد، پایه باید HIGH باشد تا خاموش بماند)
  if (RELAY_ACTIVE_LEVEL == LOW) {
    digitalWrite(RELAY1_PIN, HIGH);
    digitalWrite(RELAY2_PIN, HIGH);
  } else {
    digitalWrite(RELAY1_PIN, LOW);
    digitalWrite(RELAY2_PIN, LOW);
  }

  // ۲. حالا پایه را به عنوان خروجی تعریف می‌کنیم
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  
  // -------------------------------------------------------

  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASS);

  server.on("/", handleRoot);
  server.on("/cmd", handleCmd);
  server.on("/state", handleState);
  server.begin();

  Serial.println("Server Ready!");
}

void loop() {
  server.handleClient();
}
