#include "WifiCam.hpp"
#include <WiFi.h>
#include <WebServer.h>
#include <memory>

static const char* WIFI_SSID = "Melody4";
static const char* WIFI_PASS = "";

esp32cam::Resolution initialResolution;

WebServer server(80);
unsigned long lastCaptureTime = 0;
const unsigned long captureInterval = 5000; // 10 seconds
std::unique_ptr<esp32cam::Frame> lastFrame;

void setup() {
  Serial.begin(115200);
  Serial.println();
  delay(1000);

  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi failure %d\n", WiFi.status());
    delay(5000);
    ESP.restart();
  }
  Serial.println("WiFi connected");
  delay(1000);

  {
    using namespace esp32cam;

    initialResolution = Resolution::find(1024, 768);

    Config cfg;
    cfg.setPins(pins::AiThinker);
    cfg.setResolution(initialResolution);
    cfg.setJpeg(80);

    bool ok = Camera.begin(cfg);
    if (!ok) {
      Serial.println("camera initialize failure");
      delay(5000);
      ESP.restart();
    }
    Serial.println("camera initialize success");
  }

  Serial.println("camera starting");
  Serial.print("http://");
  Serial.println(WiFi.localIP());

  server.on("/capture", HTTP_GET, handleCapture);
  server.begin();
}

void loop() {
  server.handleClient();

  unsigned long currentTime = millis();
  if (currentTime - lastCaptureTime >= captureInterval) {
    captureImage();
    lastCaptureTime = currentTime;
  }
}

void captureImage() {
  using namespace esp32cam;
  lastFrame = Camera.capture();
  if (!lastFrame) {
    Serial.println("Capture failed");
    return;
  }
  Serial.printf("Captured %u bytes\n", lastFrame->size());
}

void handleCapture() {
  if (!lastFrame) {
    server.send(500, "text/plain", "No image captured");
    return;
  }
  server.sendHeader("Content-Type", "image/jpeg");
  server.sendHeader("Content-Length", String(lastFrame->size()));
  server.send(200);
  server.sendContent((const char*)lastFrame->data(), lastFrame->size());
}
