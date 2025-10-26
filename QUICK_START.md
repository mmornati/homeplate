# Guide Rapide des Nouvelles Fonctionnalités

## 🚀 Démarrage Rapide

### Compilation
```bash
pio run                    # Inkplate 10
pio run -e inkplate10v2    # Inkplate 10v2
pio run -e ota             # Mise à jour OTA
```

### Configuration du Watchdog
Dans `config.h` (optionnel, défaut = 120 secondes):
```cpp
#define WDT_TIMEOUT_SEC 180  // 3 minutes
```

### Configuration du Logging
Dans votre code d'initialisation:
```cpp
#include "logger.h"

// En production
Logger::setLevel(LOG_LEVEL_INFO);

// En développement
Logger::setLevel(LOG_LEVEL_DEBUG);
```

## 📖 Exemples d'Utilisation

### 1. Utiliser les Classes RAII pour Mutex

**Avant**:
```cpp
void readBattery() {
    i2cStart();
    double voltage = display.readBattery();
    i2cEnd();
}
```

**Après (méthode recommandée)**:
```cpp
#include "mutex_guards.h"

void readBattery() {
    I2CLock lock;
    double voltage = display.readBattery();
    // Mutex libéré automatiquement, même en cas d'exception
}
```

**Pour opérations combinées**:
```cpp
void displayBattery() {
    I2CDisplayLock lock;  // Acquiert I2C et Display
    double voltage = display.readBattery();
    display.print(voltage);
    // Les deux mutex libérés automatiquement
}
```

### 2. Utiliser le Système de Logging

**Au lieu de**:
```cpp
Serial.printf("[WIFI] Connected to %s\n", ssid);
Serial.printf("[ERROR] Connection failed!\n");
```

**Utilisez**:
```cpp
#include "logger.h"

LOG_INFO("WIFI", "Connected to %s", ssid);
LOG_ERROR("WIFI", "Connection failed!");
LOG_WARNING("BATTERY", "Low battery: %d%%", percent);
LOG_DEBUG("MQTT", "Publishing to topic: %s", topic);
LOG_CRITICAL("SYSTEM", "Out of memory!");
```

**Avantages**:
- Format cohérent avec tags
- Filtrage par niveau
- Facile à désactiver en production
- Meilleur débogage

### 3. Watchdog dans les Tâches Personnalisées

Si vous créez une nouvelle tâche longue:

```cpp
void maTaskPersonnalisee(void* param) {
    // S'inscrire au watchdog
    WatchdogManager::subscribe();
    
    while(true) {
        // Votre code ici...
        performLongOperation();
        
        // Réinitialiser le watchdog régulièrement
        WatchdogManager::reset();
        
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}
```

**Pour opérations très longues**:
```cpp
void operationLongue() {
    // Désactiver temporairement
    WatchdogManager::disable();
    
    // Opération qui prend >2 minutes
    performVeryLongOperation();
    
    // Réactiver
    WatchdogManager::enable();
    WatchdogManager::reset();
}
```

### 4. Automation Home Assistant pour Batterie Faible

Créez une automation dans Home Assistant:

```yaml
automation:
  - alias: "HomePlate - Alerte Batterie Critique"
    trigger:
      - platform: mqtt
        topic: "homeassistant/sensor/homeplate/low_battery_alert/state"
    condition:
      - condition: template
        value_template: "{{ trigger.payload_json.alert == 'critical_low_battery' }}"
    action:
      # Notification mobile
      - service: notify.mobile_app_votre_telephone
        data:
          title: "⚠️ HomePlate Batterie Critique"
          message: >
            Batterie HomePlate très faible! 
            Voltage: {{ trigger.payload_json.voltage }}V
            Niveau: {{ trigger.payload_json.battery }}%
          data:
            priority: high
            ttl: 0
      
      # Notification persistante
      - service: persistent_notification.create
        data:
          title: "HomePlate - Batterie Critique"
          message: "Recharger immédiatement. Voltage: {{ trigger.payload_json.voltage }}V"
      
      # LED rouge si vous avez des LEDs
      - service: light.turn_on
        target:
          entity_id: light.bureau_led
        data:
          rgb_color: [255, 0, 0]
          brightness: 255
```

**Carte Lovelace**:
```yaml
type: entities
title: HomePlate Status
entities:
  - entity: sensor.homeplate_battery
    name: Batterie
  - entity: sensor.homeplate_voltage
    name: Voltage
  - entity: sensor.homeplate_low_battery_alert
    name: Alerte Batterie
  - entity: sensor.homeplate_temperature
    name: Température
  - entity: sensor.homeplate_wifi_signal
    name: Signal WiFi
```

## 🔍 Débogage

### Activer le Mode Debug
```cpp
// Dans votre code d'initialisation
Logger::setLevel(LOG_LEVEL_DEBUG);
```

### Vérifier l'État du Watchdog
```cpp
if (WatchdogManager::isInitialized()) {
    LOG_INFO("SYSTEM", "Watchdog actif");
}
```

### Monitorer l'Utilisation de la Stack
Le code existant utilise déjà:
```cpp
printDebugStackSpace();  // Affiche l'utilisation mémoire
```

## ⚠️ Points d'Attention

### Watchdog
- Ne pas oublier de `reset()` dans les boucles longues
- Désactiver pendant OTA (déjà fait automatiquement)
- Désactiver pendant sleep (déjà fait automatiquement)
- Timeout par défaut = 2 minutes

### Mutex RAII
- Les classes RAII sont plus sûres que `i2cStart()`/`i2cEnd()`
- Utilisez-les pour nouveau code
- Migration progressive du code existant possible
- Empêche les oublis de libération

### Logging
- `Serial.printf()` continue de fonctionner
- Logger est additionnel, pas de remplacement forcé
- Configurez le niveau en production pour réduire le bruit

## 📊 Monitoring en Production

### Via MQTT
Le système publie maintenant:
- État batterie normal (topic existant)
- **NOUVEAU**: Alertes batterie critique
- WiFi signal
- Température
- Boot count

### Créer un Dashboard de Monitoring
```yaml
type: vertical-stack
cards:
  - type: gauge
    entity: sensor.homeplate_battery
    min: 0
    max: 100
    severity:
      green: 50
      yellow: 20
      red: 0
    name: Batterie HomePlate
  
  - type: history-graph
    entities:
      - sensor.homeplate_voltage
      - sensor.homeplate_temperature
    hours_to_show: 24
    
  - type: conditional
    conditions:
      - entity: sensor.homeplate_low_battery_alert
        state_not: "unavailable"
    card:
      type: markdown
      content: |
        ## ⚠️ ALERTE BATTERIE CRITIQUE
        Recharger immédiatement!
```

## 🎓 Bonnes Pratiques

### 1. Pour Nouveau Code
```cpp
#include "mutex_guards.h"
#include "logger.h"

void nouvelleFeature() {
    LOG_INFO("FEATURE", "Starting new feature");
    
    {
        I2CLock lock;
        // Code I2C ici
    }
    
    LOG_DEBUG("FEATURE", "Feature completed");
}
```

### 2. Pour Migration Progressive
- Gardez le code existant comme il est
- Ajoutez les nouvelles fonctionnalités progressivement
- Testez chaque changement séparément

### 3. Pour Production
```cpp
// Au démarrage
Logger::setLevel(LOG_LEVEL_INFO);
WatchdogManager::init(120);  // 2 minutes
```

## 🆘 Résolution de Problèmes

### Le Watchdog Redémarre Trop Souvent
1. Augmentez le timeout: `#define WDT_TIMEOUT_SEC 180`
2. Vérifiez que toutes les tâches appellent `reset()`
3. Identifiez les opérations longues et désactivez temporairement

### Les Mutex Bloquent
1. Vérifiez l'ordre d'acquisition (toujours le même ordre)
2. Utilisez les classes RAII pour éviter les oublis
3. Vérifiez les logs avec `LOG_DEBUG`

### Pas de Notification Batterie
1. Vérifiez la connexion MQTT
2. Vérifiez que le voltage est bien <= threshold
3. Consultez les logs MQTT

## 📚 Documentation Complète
- `IMPROVEMENTS.md` - Documentation détaillée de toutes les améliorations
- `SUMMARY.md` - Résumé des changements
- Headers des fichiers - Documentation inline

## 💡 Conseils
1. Commencez par activer le logging en mode DEBUG pour comprendre le flux
2. Utilisez les classes RAII pour tout nouveau code
3. Configurez les automations Home Assistant pour les alertes
4. Monitorez le watchdog lors des premiers jours
5. Ajustez le timeout du watchdog selon vos besoins
