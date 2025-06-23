#include "WebSlaves.h"
#include "ConfigManager.h"

void handleSlaves(WebServer &server, ConfigManager &configManager)
{
    if (server.method() == HTTP_POST)
    {
        ConfigManager::Config config = configManager.loadConfig();
        config.numSlaves = server.arg("numSlaves").toInt();
        for (int i = 0; i < config.numSlaves && i < MAX_SLAVES; ++i)
        {
            String nameArg = "name" + String(i);
            String ipArg = "ip" + String(i);
            strncpy(config.slaves[i].name, server.arg(nameArg).c_str(), sizeof(config.slaves[i].name));
            config.slaves[i].name[sizeof(config.slaves[i].name) - 1] = '\0';
            strncpy(config.slaves[i].ip, server.arg(ipArg).c_str(), sizeof(config.slaves[i].ip));
            config.slaves[i].ip[sizeof(config.slaves[i].ip) - 1] = '\0';
        }

        // Limpia los datos de los slaves no usados
        for (int i = config.numSlaves; i < MAX_SLAVES; ++i)
        {
            config.slaves[i].name[0] = '\0';
            config.slaves[i].ip[0] = '\0';
        }
        configManager.saveConfig(config);
        server.sendHeader("Location", "/slaves");
        server.send(303);
        return;
    }

    ConfigManager::Config config = configManager.loadConfig();
    String html = R"rawliteral(
        <!DOCTYPE html>
        <html>
        <head>
            <title>Configuración de Slaves</title>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1">
            <style>
                body { font-family: Arial, sans-serif; margin: 1em; font-size: 1.1em; }
                .config-container { max-width: 500px; margin: auto; }
                .slave { border: 1px solid #2196f3; padding: 0.7em; margin-bottom: 1em; border-radius: 8px; background: #f0f7ff; }
                .slave legend { font-weight: bold; font-size: 1em; }
                .form-label { display: inline-block; width: 90px; font-size: 1em; }
                .form-input { width: 160px; font-size: 1em; padding: 0.2em; }
                .form-row { margin-bottom: 0.7em; }
                .submit-btn { padding: 0.5em 1.5em; font-size: 1.1em; background: #2196f3; color: white; border: none; border-radius: 6px; }
                .submit-btn:hover { background: #1976d2; }
                .back-link { display: inline-block; margin-top: 1em; font-size: 1em; }
            </style>
        </head>
        <body>
            <div class="config-container">
            <h1 style="font-size:1.5em;">Configuración de Slaves</h1>
            <form method="POST" action="/slaves" id="slavesForm" enctype="application/x-www-form-urlencoded" autocomplete="off">
                <p>Configura los slaves que se conectarán al maestro. Puedes tener hasta 8 slaves.</p>
              
    )rawliteral";

    // Genera el campo de cantidad correctamente
    html += "<div class=\"form-row\">";
    html += "<label class=\"form-label\" for=\"numSlaves\">Cantidad:</label>";
    html += "<input type=\"number\" id=\"numSlaves\" name=\"numSlaves\" min=\"1\" max=\"8\" value=\"" + String(config.numSlaves) + "\" required class=\"form-input\">";
    html += "</div>";

    for (int i = 0; i < config.numSlaves; ++i)
    {
        html += "<fieldset class='slave'><legend>Slave " + String(i + 1) + "</legend>";
        html += "<div class='form-row'><label class='form-label'>Nombre:</label>";
        html += "<input class='form-input' type='text' name='name" + String(i) + "' value='" + String(config.slaves[i].name) + "' required></div>";
        html += "<div class='form-row'><label class='form-label'>IP:</label>";
        html += "<input class='form-input' type='text' name='ip" + String(i) + "' value='" + String(config.slaves[i].ip) + "' required></div>";
        html += "</fieldset>";
    }

    html += R"rawliteral(
                <input class="submit-btn" type="submit" value="Guardar">
            </form>
            <a class="back-link" href="/">Volver al Home</a>
            </div>
        </body>
        </html>
    )rawliteral";

    server.send(200, "text/html", html);
}