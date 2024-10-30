# **Systemspecificering för IoT-baserad temperatur- och luftfuktighetsmätning**

## 1. **Inledning**
Denna Proof of Concept (PoC) beskriver en IoT-lösning för att mäta och skicka temperatur och luftfuktighet trådlöst över ett lokalt IP-nätverk. Säker dataöverföring sker med hjälp av MQTT, mTLS och HTTPS.

PoC:n är utformad för att visa hur systemet kommer att fungera i praktiken, med en DHT11-sensor kopplad till en ESP32 som skickar data över Wi-Fi i ett lokalt nätverk. Backend är byggt med Docker Compose som använder en MQTT-server för kommunikation med ESPn, en klient för kommunikation mellan server och backend, en PostgreSQL-databas används för att spara data, och Grafana för visualisering. Backendklienten som körs under PoC är skriven i Python, ESP32-programmet är skriven i C++ (PlatformIO) och backends Rest Controller är skriven i Java (Spring Boot, Gradle Kotlin).

## 2. **Systemarkitektur**

### **2.1 Enhetsarkitektur**

Enheten som används för övervakning består av:

- **ESP32** med stöd för WiFi.
- **DHT11-sensor** kopplad till ESPn med en 10k resistor som läser temperatur och luftfuktighet.
- **Micro USB kabel** för strömförsörjning och uppdateringar.

### **2.2 Backend**

Backend-systemet körs i en Docker Compose-miljö och innehåller:

- **MQTT-server**:Används för kommunikation mellan IoT-enhet och backendsystem enligt pub/sub-modell och FOTA.
- **Pythonklient**: Används för att sköta kommunikationen mellan MQTT-servern och backend.
- **PostgreSQL**: En SQL-databas för att lagra sensordata som samlas in från enheten.
- **Spring Boot applikation**: Används för att sköta kommunikationen med PostgreSQL-databasen.
- **Grafana**: Används för att visualisera sensordata

### **2.3 Kommunikationsprotokoll**

Systemet använder följande protokoll och standarder:

- **WI-FI**: För trådlös uppkoppling.
- **MQTT**: För sensordata samt fjärruppdateringar.
- **mTLS**: För säker överföring av data mellan enheter och servern, implementerad med en privat CA.
- **HTTPS**: För säker överföring av data mellan MQTT-servern och backend.

### **2.4 Säkerhet**

Säkerhet är en kritisk komponent i systemet:

- **mTLS (Mutual Transport Layer Security)**: Används för att säkra kommunikationen mellan IoT-enheten och MQTT-servern.
- **Manuell Mjukvaruuppdatering via micro USBt**: Mjukvaran uppdateras för närvarande manuellt över micro USB.

## 3. **Cyber Resilience Act (CRA) Överensstämmelse**

Systemet följer Cyber Resilience Act genom att inkludera:

- **Säker kommunikation** via mTLS för att skydda data från avlyssning och manipulation.

Systemet bör implementera följande:


- **Hashning** av MAC-adress i backend.
- **Secure Boot** Så varje enhet startar endast mjukvara som är signerad med en godkänd nyckel, för att förhindra körning av skadlig kod.
- **FOTA** För möjlighet till regelbundna uppdateringar för att fixa sårbarheter. Kan implementeras med MQTT.
- Mjukvaruhantering med kontinuerlig övervakning av CVEs (Common Vulnerabilities and Exposures) för att snabbt kunna adressera nya säkerhetshot.

## 4. **Skalbarhet och framtida utveckling**

Lösningen är utformad för att vara skalbar:

- Systemet kan enkelt skalas för att inkludera fler enheter i framtiden, utan att kompromissa med prestanda eller säkerhet.
- Systemet bör utvecklas för att stödja flera miljöer, som kan anpassas beroende på om de hostas i molnet eller lokalt.
- Grafana möjliggör flexibel och skalbar visualisering, och kan anpassas för fler mätpunkter eller längre tidsperioder.
