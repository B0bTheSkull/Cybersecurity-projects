#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include "mbedtls/sha256.h"

const char* AP_SSID = "ESP32 Security Lab";
const byte DNS_PORT = 53;

IPAddress apIP(192,168,4,1);
IPAddress netMsk(255,255,255,0);

DNSServer dnsServer;
WebServer server(80);

// --- helpers ---------------------------------------------------------

String sha256Hex(const String& input) {
  byte hash[32];
  mbedtls_sha256_context ctx;
  mbedtls_sha256_init(&ctx);
  mbedtls_sha256_starts_ret(&ctx, 0);
  mbedtls_sha256_update_ret(&ctx, (const unsigned char*)input.c_str(), input.length());
  mbedtls_sha256_finish_ret(&ctx, hash);
  mbedtls_sha256_free(&ctx);

  const char* hex = "0123456789abcdef";
  String out;
  out.reserve(64);
  for (int i = 0; i < 32; i++) {
    out += hex[(hash[i] >> 4) & 0xF];
    out += hex[hash[i] & 0xF];
  }
  return out;
}

String clientIP() {
  return server.client().remoteIP().toString();
}

String userAgent() {
  // WebServer doesn't always expose headers unless requested; this works when present.
  return server.header("User-Agent");
}

// --- portal page -----------------------------------------------------

String portalPage() {
  // IMPORTANT: training/education banner to keep this recruiter-safe.
  return R"HTML(
<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <title>Security Lab Captive Portal — Sign In</title>
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
      --warn: rgba(255, 170, 0, 0.16);
      --warnb: rgba(255, 170, 0, 0.35);
    }

    * { box-sizing: border-box; }
    body {
      margin: 0;
      font-family: ui-sans-serif, system-ui, -apple-system, Segoe UI, Roboto, Helvetica, Arial;
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
    @media (max-width: 860px) { .wrap { grid-template-columns: 1fr; } }

    .hero, .card {
      border: 1px solid var(--border);
      border-radius: var(--radius);
      background: linear-gradient(180deg, var(--card), var(--card2));
      box-shadow: var(--shadow);
      backdrop-filter: blur(10px);
      -webkit-backdrop-filter: blur(10px);
      overflow: hidden;
    }

    .hero { padding: 26px 26px 22px; }
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
      width: 10px; height: 10px; border-radius: 50%;
      background: radial-gradient(circle at 30% 30%, var(--accent2), var(--accent));
      box-shadow: 0 0 16px rgba(124,92,255,0.7);
    }
    h1 { margin: 16px 0 10px; font-size: 34px; line-height: 1.12; letter-spacing: -0.02em; }
    .sub { margin: 0 0 18px; color: var(--muted); max-width: 52ch; line-height: 1.55; }

    .training {
      margin-top: 14px;
      padding: 12px 12px;
      border-radius: 14px;
      border: 1px solid var(--warnb);
      background: var(--warn);
      color: rgba(255,255,255,0.86);
      font-size: 13px;
      line-height: 1.45;
    }

    .card { padding: 22px; display: grid; align-content: start; gap: 14px; }
    .card h2 { margin: 0; font-size: 18px; letter-spacing: -0.01em; }
    .hint { margin: 0 0 4px; color: var(--muted); font-size: 13px; line-height: 1.4; }

    form { display: grid; gap: 12px; margin-top: 6px; }
    label { display: block; font-size: 13px; color: var(--muted); margin-bottom: 6px; }

    input {
      width: 100%;
      padding: 12px 12px;
      border-radius: 12px;
      border: 1px solid var(--border);
      background: rgba(0,0,0,0.22);
      color: var(--text);
      outline: none;
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
    }

    .fineprint { margin: 10px 0 0; color: rgba(255,255,255,0.55); font-size: 12px; line-height: 1.45; }
    .footer { margin-top: 10px; color: rgba(255,255,255,0.55); font-size: 12px; text-align: center; }
  </style>
</head>

<body>
  <div class="wrap">
    <section class="hero" aria-label="Network information">
      <div class="badge">
        <span class="dot" aria-hidden="true"></span>
        Captive Portal Demo • ESP32 Security Lab
      </div>

      <h1>Welcome to<br>ESP32 Security Lab Wi-Fi</h1>
      <p class="sub">
        This network intentionally triggers a captive portal to demonstrate how rogue AP phishing works
        and how to spot it.
      </p>

      <div class="training">
        <strong>TRAINING MODE:</strong> Do not enter real passwords. This page is a safe demo.
        Submissions are not stored; passwords are never logged in plaintext.
      </div>
    </section>

    <section class="card" aria-label="Sign in form">
      <div>
        <h2>Sign in</h2>
        <p class="hint">Enter a demo username and any password to see the awareness screen.</p>
      </div>

      <form action="/login" method="POST">
        <div>
          <label for="username">Username (demo):</label>
          <input type="text" id="username" name="username" required autocomplete="username" />
        </div>

        <div>
          <label for="password">Password (demo):</label>
          <input type="password" id="password" name="password" required autocomplete="current-password" />
        </div>

        <button type="submit">Connect</button>

        <p class="fineprint">
          This is an educational captive portal simulation. If you ever see a login like this in the wild,
          treat it as hostile until proven otherwise.
        </p>
      </form>

      <div class="footer">© ESP32 Security Lab • Captive Portal Demo</div>
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

// --- setup/loop ---

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(AP_SSID);

  dnsServer.start(DNS_PORT, "*", apIP);

  // Request headers we want (User-Agent)
  const char* headerKeys[] = {"User-Agent"};
  server.collectHeaders(headerKeys, 1);

  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", portalPage());
  });

  // NO PLAINTEXT PASSWORD LOGGING
  // Original Version had plaintext password logging but for writeup we're not doing that.
  server.on("/login", HTTP_POST, []() {
    String username = server.arg("username");
    String password = server.arg("password");

    // Safe telemetry
    String ip = clientIP();
    String ua = userAgent();

    // Hash the password immediately and discard plaintext
    String pwHash = sha256Hex(password);
    password = ""; // explicitly drop plaintext copy (for legal purposes I'm not risking that haha)

    Serial.println("\n--- Captive Portal Submission (Training Mode) ---");
    Serial.print("Uptime(ms): "); Serial.println(millis());
    Serial.print("Client IP: ");  Serial.println(ip);
    Serial.print("User-Agent: "); Serial.println(ua);
    Serial.print("Username: ");   Serial.println(username);
    Serial.print("Password SHA-256: "); Serial.println(pwHash);
    Serial.println("NOTE: plaintext password was NOT logged or stored.\n");

    // Awareness response
    server.send(200, "text/html",
      "<html><body style='font-family:system-ui;padding:22px;'>"
      "<h2>✅ Demo Complete</h2>"
      "<p>This was a <b>captive portal security awareness demo</b>.</p>"
      "<p><b>Lesson:</b> Never type real credentials into a Wi-Fi portal unless you trust the network.</p>"
      "<ul>"
      "<li>Verify the SSID/BSSID</li>"
      "<li>Prefer cellular or a trusted VPN</li>"
      "<li>Assume portals are hostile by default</li>"
      "</ul>"
      "<p>You can close this window.</p>"
      "</body></html>"
    );
  });

  // Captive checks
  server.on("/hotspot-detect.html", HTTP_GET, redirectToRoot);
  server.on("/library/test/success.html", HTTP_GET, redirectToRoot);
  server.on("/generate_204", HTTP_GET, redirectToRoot);

  // Catch-all
  server.onNotFound(redirectToRoot);

  server.begin();

  Serial.println("Captive portal running (recruiter-safe mode).");
  Serial.print("AP SSID: "); Serial.println(AP_SSID);
  Serial.print("Portal IP: "); Serial.println(apIP);
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}
