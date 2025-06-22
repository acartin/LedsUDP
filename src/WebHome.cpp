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
                .slider { width: 160px; }
                .slave-controls { display: flex; align-items: center; gap: 1em; margin-top: 0.5em; }
                .onoff-btn { padding: 0.3em 1.2em; font-size: 1em; background: #4caf50; color: white; border: none; border-radius: 6px; }
                .onoff-btn.off { background: #f44336; }
                .tap-btn { display: none; width: 90vw; max-width: 500px; height: 70px; font-size: 2em; background: #ff4081; color: white; border: none; border-radius: 16px; margin: 2em auto 0 auto; }
                .mode-row { margin-bottom: 1.2em; }
                .config-link { display: inline-block; margin-top: 1em; font-size: 1em; }
            </style>
        </head>
        <body>
            <div class="home-container">
            <h1 style="font-size:1.5em;">Control Maestro LED</h1>
            <div class="mode-row">
                <label for="mode">Modo:</label>
                <select id="mode" onchange="onModeChange()" style="font-size:1em;">
                    <option>Entrada</option>
                    <option>Vals</option>
                    <option>Ceremonia</option>
                    <option>Fotobooth</option>
                    <option>Crucero</option>
                    <option>Beat</option>
                </select>
            </div>
            <hr>
    )rawliteral";

    for (int i = 0; i < config.numSlaves; ++i)
    {
        html += "<fieldset class='slave'><legend>" + String(config.slaves[i].name) + "</legend>";
        html += "<div class='slave-controls'>";
        html += "<label>Intensidad: <span id='int" + String(i) + "val'>128</span></label>";
        html += "<input type='range' min='0' max='255' value='128' class='slider' id='int" + String(i) + "' "
                                                                                                         "oninput=\"int" +
                String(i) + "val.innerText=this.value; updateIntensity(" + String(i) + ", this.value)\">";
        html += "<label style='margin-left:1em;'>Color:</label>";
        html += "<input type='color' id='color" + String(i) + "' value='#ffffff' style='width:40px;height:30px;vertical-align:middle;'>";
        html += "<button id='onoff" + String(i) + "' class='onoff-btn' onclick='toggle(" + String(i) + ")'>ON</button>";
        html += "</div></fieldset>";
    }

    html += R"rawliteral(
            <button id="tapBtn" class="tap-btn" onclick="tap()">TAP</button>
            <a class="config-link" href="/config">Configurar Slaves</a>
            </div>
            <script>
                function updateIntensity(slaveId, value) {
                    fetch('/api/intensity', {
                    method: 'POST',
                    headers: {'Content-Type': 'application/x-www-form-urlencoded'},
                    body: 'slave=' + slaveId + '&intensity=' + value + '&mode=' + encodeURIComponent(document.getElementById('mode').value)
             });
                }
                function toggle(id) {
                    var btn = document.getElementById('onoff'+id);
                    if(btn.classList.contains('off')) {
                        btn.classList.remove('off');
                        btn.innerText = 'ON';
                    } else {
                        btn.classList.add('off');
                        btn.innerText = 'OFF';
                    }
                }
                function onModeChange() {
                    var mode = document.getElementById('mode').value;
                    var tapBtn = document.getElementById('tapBtn');
                    tapBtn.style.display = (mode === 'Beat') ? 'block' : 'none';
                }
                function tap() {
                    alert('¡TAP!');
                }
                window.onload = onModeChange;
            </script>
        </body>
        </html>
    )rawliteral";
    server.send(200, "text/html", html);
}