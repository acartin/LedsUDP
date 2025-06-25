#include "WebHome.h"
#include <WebServer.h>
#include "ConfigManager.h"

extern ConfigManager configManager;

void handleRoot(WebServer &server)
{
    ConfigManager::Config config = configManager.loadConfig();

    String html = R"rawliteral(
        <!DOCTYPE html>
        <html>
        <head>
            <title>Control Maestro LED</title>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1">
            <style>
        body { font-family: Arial, sans-serif; margin: 1em; font-size: 1.1em; }
        .home-container { max-width: 500px; margin: auto; }
        .slave { border: 1px solid #2196f3; padding: 0.7em; margin-bottom: 1em; border-radius: 8px; background: #f0f7ff; }
        .slave legend { font-weight: bold; font-size: 1em; }
        .slider {
            width: 33vw;
            min-width: 80px;
            max-width: 160px;
            height: 28px;
            touch-action: pan-x;
            }

        .slave-controls { 
             display: flex; 
            flex-direction: row;
            align-items: center;
            justify-content: space-between;
            gap: 0.5em;
            margin-top: 0.5em;
            width: 100%;
        }

        .onoff-btn { padding: 0.3em 1.2em; font-size: 1em; background: #4caf50; color: white; border: none; border-radius: 6px; }
        .onoff-btn.off { background: #f44336; }
        .tap-btn { display: none; width: 90vw; max-width: 500px; height: 70px; font-size: 2em; background: #ff4081; color: white; border: none; border-radius: 16px; margin: 2em auto 0 auto; }
        .mode-row { margin-bottom: 1.2em; }
        .config-link { display: inline-block; margin-top: 1em; font-size: 1em; }
        .mode-btn {
            margin: 0 4px 4px 0;
            padding: 8px 16px;
            border-radius: 6px;
            border: 1px solid #888;
            background: #eee;
            cursor: pointer;
            min-width: 80px;
        }
        .mode-btn.selected {
            background: #4caf50;
            color: #fff;
            border-color: #388e3c;
        }
        /* Responsive: stack controls vertically on small screens */
        @media (max-width: 600px) {
        .home-container { max-width: 98vw; }
        .slave-controls {
            display: flex;
            flex-direction: row;
            align-items: center;
            justify-content: space-between;
            gap: 0.5em;
            margin-top: 0.5em;
            width: 100%;
        }
        .slider {
            width: 33vw;
            min-width: 60px;
            max-width: none;
            height: 36px;
            }
        .tap-btn { width: 98vw; }
}
    </style>
        </head>
        <body>
            <div class="home-container">
            <h1 style="font-size:1.5em;">Control Maestro LED</h1>
            <hr>
    )rawliteral";

    // Selector de modos como botones (grid accesible)
    html += "<div id='modeSelector' style='margin-bottom:1em; display: flex; flex-wrap: wrap; gap: 0.5em;'>";
    for (int i = 0; i < config.numModes; ++i)
    {
        html += "<button class='mode-btn' id='modeBtn" + String(i) + "' type='button' onclick='selectMode(" + String(i) + ")' aria-label='Seleccionar modo " + String(config.modeNames[i]) + "'>";
        html += String(config.modeNames[i]);
        html += "</button>";
    }
    html += "</div>";

    // Slaves con sliders y controles
    for (int i = 0; i < config.numSlaves; ++i)
    {
        html += "<fieldset class='slave'><legend>" + String(config.slaves[i].name) + "</legend>";
        html += "<div class='slave-controls' style='flex-direction: row; align-items: center; gap: 0.5em;'>";
        html += "<input type='range' min='0' max='255' value='128' class='slider' id='int" + String(i) + "' "
                                                                                                         "oninput=\"updateIntensity(" +
                String(i) + ", this.value)\" onchange=\"updateIntensity(" + String(i) + ", this.value)\">";
        html += "<input type='color' id='color" + String(i) + "' value='#ffffff' style='width:36px;height:28px;vertical-align:middle;' onchange=\"updateColor(" + String(i) + ", this.value)\">";
        html += "<button id='onoff" + String(i) + "' class='onoff-btn' onclick='toggle(" + String(i) + ")'>ON</button>";
        html += "</div></fieldset>";
    }

    html += "<div id='tapOptions' style='display:none; margin-bottom:1em;'>";
    html += "<label for='tapEffect'>Efecto:</label>";
    html += "<select id='tapEffect'><option value='strobe'>Strobe</option><option value='fire'>Fire</option></select>";
    html += "<label for='tapColor' style='margin-left:1em;'>Color:</label>";
    html += "<input type='color' id='tapColor' value='#ffffff'>";
    html += "<label for='tapSpeed' style='margin-left:1em;'>Velocidad:</label>";
    html += "<input type='range' id='tapSpeed' min='1' max='20' value='10' style='width:80px;'>";
    html += "</div>";

    // Botón TAP y enlace a configuración
    html += R"rawliteral(
                <button id="tapBtn" class="tap-btn" onclick="sendTap()">TAP</button>
                <a class="config-link" href="/slaves">Configurar Slaves</a>
                <a class="config-link" href="/modos">Configurar Modos</a>
                <a class="config-link" href="/debug">Debug</a>
                </div>                
            )rawliteral";

    // ajax
    html += "<script>";
    html += "var selectedMode = 0;\n";
    html += "var NUM_MODES = " + String(config.numModes) + ";\n";
    html += "var modeNames = [";
    for (int i = 0; i < config.numModes; ++i)
    {
        if (i > 0)
            html += ",";
        html += "\"" + String(config.modeNames[i]) + "\"";
    }
    html += "];\n";
    html += "</script>\n";

    // JS principal
    html += R"rawliteral(
        <script>
        function selectMode(idx) {
            selectedMode = idx;
            for (var i = 0; i < NUM_MODES; ++i) {
                var btn = document.getElementById('modeBtn' + i);
                if (btn) btn.classList.remove('selected');
            }
            var btn = document.getElementById('modeBtn' + idx);
            if (btn) btn.classList.add('selected');
            onModeChange();

            // --- Notifica al backend el cambio de modo ---
            fetch('/api/setMode?mode=' + encodeURIComponent(modeNames[selectedMode]), {
                method: 'POST'
            })
            .then(response => {
                if (response.ok) {
                    // Recarga el estado del modo para refrescar la UI
                    loadModeState(modeNames[selectedMode]);
                } else {
                 alert('Error cambiando de modo');
                }
            });
        }

        function updateIntensity(slaveId, value) {
            fetch('/api/intensity', {
            method: 'POST',
            headers: {'Content-Type': 'application/x-www-form-urlencoded'},
            body: 'slave=' + slaveId + '&intensity=' + value + '&mode=' + encodeURIComponent(modeNames[selectedMode])
            });
        }

        function updateColor(slaveId, value) {
            fetch('/api/color', {
            method: 'POST',
            headers: {'Content-Type': 'application/x-www-form-urlencoded'},
            body: 'slave=' + slaveId + '&color=' + encodeURIComponent(value.replace('#','')) + '&mode=' + encodeURIComponent(modeNames[selectedMode])
        });
    }

        function updateOnOff(slaveId, value) {
            fetch('/api/onoff', {
            method: 'POST',
            headers: {'Content-Type': 'application/x-www-form-urlencoded'},
            body: 'slave=' + slaveId + '&on=' + (value ? '1' : '0') + '&mode=' + encodeURIComponent(modeNames[selectedMode])
        });
    }

        function toggle(id) {
            var btn = document.getElementById('onoff'+id);
            var isOn = !btn.classList.contains('off');
            if(isOn) {
            btn.classList.add('off');
            btn.innerText = 'OFF';
            updateOnOff(id, 0);
        } else {
            btn.classList.remove('off');
            btn.innerText = 'ON';
            updateOnOff(id, 1);
        }
    }

        function onModeChange() {
            var mode = modeNames[selectedMode];
            var tapBtn = document.getElementById('tapBtn');
            var tapOptions = document.getElementById('tapOptions');
            tapBtn.style.display = (mode === 'Beat') ? 'block' : 'none';
            if (tapOptions) tapOptions.style.display = (mode === 'Beat') ? 'block' : 'none';
        }
        // Cargar el estado del modo seleccionado
        
        function loadModeState(modeName) {
    fetch('/api/get_mode_state?mode=' + encodeURIComponent(modeName))
        .then(response => response.json())
        .then(data => {
            for (let i = 0; i < data.length; ++i) {
                let slider = document.getElementById('int' + i);
                let color = document.getElementById('color' + i);
                let btn = document.getElementById('onoff' + i);
                if (slider) slider.value = data[i].intensity;
                if (color) color.value = data[i].color.padStart(7, '#');
                if (btn) {
                    if (data[i].on) {
                        btn.classList.remove('off');
                        btn.innerText = 'ON';
                    } else {
                        btn.classList.add('off');
                        btn.innerText = 'OFF';
                    }
                }
            }
        });
}

        function sendTap() {
            fetch('/api/tap', {
                method: 'POST',
                headers: {'Content-Type': 'application/x-www-form-urlencoded'},
                body: 'mode=' + encodeURIComponent(modeNames[selectedMode]) +
                    '&effect=' + encodeURIComponent(document.getElementById('tapEffect').value) +
                    '&color=' + encodeURIComponent(document.getElementById('tapColor').value) +
                    '&speed=' + encodeURIComponent(document.getElementById('tapSpeed').value)
            });
        }
        window.onload = function() {
            selectMode(0);
            onModeChange();
            loadModeState(modeNames[selectedMode]);
        };
    </script>
</body>
</html>
)rawliteral";

    // Serial.println(html);

    server.send(200, "text/html", html);
}