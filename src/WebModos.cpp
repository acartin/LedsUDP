#include "WebModos.h"
#include "ConfigManager.h"
#include "appConfig.h"

extern ConfigManager configManager;

void handleModos(WebServer &server)
{
    ConfigManager::Config config = configManager.loadConfig();

    String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>Configuración de Modos</title>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body { font-family: Arial, sans-serif; margin: 1em; font-size: 1.1em; }
        .home-container { max-width: 500px; margin: auto; }
        .modo-row { margin-bottom: 1em; }
        input[type="text"], input[type="number"] { width: 200px; font-size: 1em; }
        .save-btn { padding: 0.5em 2em; font-size: 1em; border-radius: 6px; background: #2196f3; color: #fff; border: none; }
        .save-btn:hover { background: #1976d2; }
        .back-link { display: inline-block; margin-top: 1em; font-size: 1em; }
        h1 { font-size: 1.5em; }
        label { font-weight: bold; }
        form { margin-bottom: 2em; }
        .config-link { display: inline-block; margin-top: 1em; font-size: 1em; }
    </style>
</head>
<body>
    <div class="home-container">
    <h1>Configuración de Modos</h1>
    <form method="POST" action="/modos">
)rawliteral";

    // Campo para cantidad de modos
    html += "<div class='modo-row'>";
    html += "<label for='numModes' style='margin-right:1em;'>Cantidad de modos:</label>";
    html += "<input type='number' id='numModes' name='numModes' min='1' max='" + String(MAX_MODES) + "' value='" + String(config.numModes) + "' required style='width:80px; font-size:1em;'>";
    html += "</div>";

    for (int i = 0; i < config.numModes; ++i)
    {
        html += "<div class='modo-row'>";
        html += "Modo " + String(i + 1) + ": <input type='text' name='mode" + String(i) + "' value='" + String(config.modeNames[i]) + "' required>";
        html += "</div>";
    }

    html += R"rawliteral(
        <button class="save-btn" type="submit">Guardar</button>
    </form>
    <a class="config-link" href="/">Volver al Home</a>
    </div>
</body>
</html>
)rawliteral";

    server.send(200, "text/html", html);
}

void handleModosPost(WebServer &server)
{
    ConfigManager::Config config = configManager.loadConfig();

    int numModes = server.arg("numModes").toInt();
    if (numModes < 1)
        numModes = 1;
    if (numModes > MAX_MODES)
        numModes = MAX_MODES;
    config.numModes = numModes;

    for (int i = 0; i < config.numModes; ++i)
    {
        String param = "mode" + String(i);
        if (server.hasArg(param))
        {
            String value = server.arg(param);
            value.toCharArray(config.modeNames[i], sizeof(config.modeNames[i]));
        }
        else
        {
            config.modeNames[i][0] = '\0';
        }
    }
    // Limpia los modos no usados
    for (int i = config.numModes; i < MAX_MODES; ++i)
    {
        config.modeNames[i][0] = '\0';
    }

    configManager.saveConfig(config);
    server.sendHeader("Location", "/modos");
    server.send(303);
}