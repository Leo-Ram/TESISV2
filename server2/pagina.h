#ifndef PAGINA_H
#define PAGINA_H

const String paginaHTML = R"====(
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>UDENAR BMS-Local</title>
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <header>
        <div class="container header-content">
            <div class="logo">UDENAR BMS-Local</div>
            <nav>
                <button class="nav-button">Lecturas</button>
                <button class="nav-button">Configuración</button>
                <button class="nav-button">Guía</button>
                <button class="theme-toggle" id="themeToggle">🌓</button>
            </nav>
        </div>
    </header>

    <main class="container">
        <div class="controls">
            <button class="control-button off" data-function="carga" id="carga">Carga</button>
            <button class="control-button off" data-function="descarga" id="descarga">Descarga</button>
            <button class="control-button off" data-function="balance" id="balance">Balance</button>
            <button class="control-button off" data-function="emergencia" id="emergencia">Emergencia</button>
        </div>
        <div class="monitoring-panel">
            <div class="card">
                <h2>Voltajes de Baterías</h2>
                <div class="battery-grid">
                    <div class="battery-cell">
                        <p>Batería 1</p>
                        <span id="bat1">--</span>V
                        <span class="percentage-text" id="socbat1Text">--%</span>
                        <div class="percentage-bar">
                            <div class="percentage-fill" id="socbat1"></div>
                        </div>
                    </div>
                    <div class="battery-cell">
                        <p>Batería 2</p>
                        <span id="bat2">--</span>V
                        <span class="percentage-text" id="socbat2Text">--%</span>
                        <div class="percentage-bar">
                            <div class="percentage-fill" id="socbat2"></div>
                        </div>
                    </div>
                    <div class="battery-cell">
                        <p>Batería 3</p>
                        <span id="bat3">--</span>V
                        <span class="percentage-text" id="socbat3Text">--%</span>
                        <div class="percentage-bar">
                            <div class="percentage-fill" id="socbat3"></div>
                        </div>
                    </div>
                    <div class="battery-cell">
                        <p>Batería 4</p>
                        <span id="bat4">--</span>V
                        <span class="percentage-text" id="socbat4Text">--%</span>
                        <div class="percentage-bar">
                            <div class="percentage-fill" id="socbat4"></div>
                        </div>
                    </div>
                    <div class="battery-cell">
                        <p>Batería 5</p>
                        <span id="bat5">--</span>V
                        <span class="percentage-text" id="socbat5Text">--%</span>
                        <div class="percentage-bar">
                            <div class="percentage-fill" id="socbat5"></div>
                        </div>
                    </div>
                    <div class="battery-cell">
                        <p>Batería 6</p>
                        <span id="bat6">--</span>V
                        <span class="percentage-text" id="socbat6Text">--%</span>
                        <div class="percentage-bar">
                            <div class="percentage-fill" id="socbat6"></div>
                        </div>
                    </div>
                </div>
            </div>
            <div class="card">
                <h2>Valores Totales</h2>
                <div class="total-values">
                    <div class="total-value">
                        <p>Voltaje Total</p>
                        <span id="Total">--</span>V
                        <span class="percentage-text" id="socTotalText">--%</span>
                        <div class="percentage-bar large">
                            <div class="percentage-fill" id="socTotal"></div>
                        </div>
                    </div>
                    <div class="total-value">
                        <p>Corriente</p>
                        <span id="Current">--</span>mA
                        <span class="percentage-text" id="socCurrentText">--%</span>
                        <div class="percentage-bar large">
                            <div class="percentage-fill" id="socCurrent"></div>
                        </div>
                    </div>
                    <div class="total-value">
                        <p>Temperatura</p>
                        <span id="Temperature">--</span>°C
                        <span class="percentage-text" id="socTemperatureText">--%</span>
                        <div class="percentage-bar large">
                            <div class="percentage-fill" id="socTemperature"></div>
                        </div>    
                    </div>
                </div>
            </div>
        </div>
        <!--config-->
        <div class="config-panel" id="configPanel" style="display: none;">
            <!-- Configuración Básica siempre visible -->
            <div class="card">
                <h2>Configuración Básica</h2>
                <div class="slider-container">
                    <label for="voltageSlider">Ciclo de carga (%)</label>
                    <input type="range" id="voltageSlider" min="0" max="100" value="70" class="slider">
                    <span id="voltageValue">70%</span>
                </div>
                <div class="slider-container">
                    <label for="currentSlider">Flujo de corriente (%)</label>
                    <input type="range" id="currentSlider" min="0" max="100" value="100" class="slider">
                    <span id="currentValue">100%</span>
                </div>
                <button class="config-button">Enviar</button>
            </div>
        
            <!-- Secciones desplegables -->
            <div class="collapsible-section">
                <button class="collapsible">Configuraciones de Protección</button>
                <div class="collapsible-content">
                    <div class="card card2">
                        <div class="input-grid">
                            <div class="input-group">
                                <label for="OVP">OVP (V)</label>
                                <input type="number" id="OVP" step="0.1" value="4.2">
                            </div>
                            <div class="input-group">
                                <label for="OVPR">OVPR (V)</label>
                                <input type="number" id="OVPR" step="0.1" value="4">
                            </div>
                            <div class="input-group">
                                <label for="UVP">UVP (V)</label>
                                <input type="number" id="UVP" step="0.1" value="3.2">
                            </div>
                            <div class="input-group">
                                <label for="UVPR">UVPR (V)</label>
                                <input type="number" id="UVPR" step="0.1" value="3.4">
                            </div>
                            <div class="input-group">
                                <label for="VBal">VBal (V)</label>
                                <input type="number" id="VBal" step="0.1" value="4.2">
                            </div>
                            <div class="input-group">
                                <label for="CCP">CCP (mA)</label>
                                <input type="number" id="CCP" value="1500">
                            </div>
                            <div class="input-group">
                                <label for="DCP">DCP (mA)</label>
                                <input type="number" id="DCP" value="1500">
                            </div>
                            <div class="input-group">
                                <label for="TMin">Tmin (°C)</label>
                                <input type="number" id="TMin" value="4.1">
                            </div>
                            <div class="input-group">
                                <label for="TMax">Tmax (°C)</label>
                                <input type="number" id="TMax" value="60">
                            </div>
                            <div class="input-group">
                                <label for="Cap">Cap (A/H)</label>
                                <input type="number" id="Cap" step="0.1" value="1800">
                            </div>
                        </div>
                        <button class="config-button">Enviar</button>
                    </div>
                </div>
            </div>
        
            <div class="collapsible-section">
                <button class="collapsible">Configuraciones Absolutas</button>
                <div class="collapsible-content">
                    <div class="card">
                        <div class="input-grid">
                            <div class="input-group">
                                <label for="VMax">VMax(V)</label>
                                <input type="number" id="VMax" step="0.1" value="4.2">
                            </div>
                            <div class="input-group">
                                <label for="VMin">VMin(V)</label>
                                <input type="number" id="VMin" step="0.1" value="2.6">
                            </div>
                            <div class="input-group">
                                <label for="IMax">Imax(mA)</label>
                                <input type="number" id="IMax" value="1500">
                            </div>
                            <div class="input-group">
                                <label for="ACap">ACap(mA/H)</label>
                                <input type="number" id="ACap" step="0.1" value="1800">
                            </div>
                            <div class="input-group">
                                <label for="MBal">Margen-Balanceo(mV)</label>
                                <input type="number" id="MBal" step="0.1" value="1">
                            </div>
                            <div class="input-group">
                                <label for="MRec">Margen-Recuperacion(mV)</label>
                                <input type="number" id="MRec" step="0.1" value="2">
                            </div>
                            
                            <!-- ... resto de los inputs ... -->
                        </div>
                        <button class="config-button">Enviar</button>
                    </div>
                </div>
            </div>
        
            <div class="collapsible-section">
                <button class="collapsible">Configuraciones de Administrador</button>
                <div class="collapsible-content">
                    <div class="card">
                        <div class="input-grid">
                            <div class="input-group">
                                <label for="GB1">GB1</label>
                                <input type="number" id="GB1" value="588">
                            </div>
                            <div class="input-group">
                                <label for="GB2">GB2</label>
                                <input type="number" id="GB2" value="271">
                            </div>
                            <div class="input-group">
                                <label for="GB3">GB3</label>
                                <input type="number" id="GB3" value="199">
                            </div>
                            <div class="input-group">
                                <label for="GB4">GB4</label>
                                <input type="number" id="GB4" value="143.5">
                            </div>
                            <div class="input-group">
                                <label for="GB5">GB5</label>
                                <input type="number" id="GB5" value="124.2">
                            </div>
                            <div class="input-group">
                                <label for="GB6">GB6</label>
                                <input type="number" id="GB6" value="101.7">
                            </div>
                            <div class="input-group">
                                <label for="GBI">GI</label>
                                <input type="number" id="GBI" value="4.8">
                            </div>
                            <div class="input-group">
                                <label for="GT">GT</label>
                                <input type="number" id="GT" value="1">
                            </div>
                            <div class="input-group">
                                <label for="GANG">ANG</label>
                                <input type="number" id="GANG" value="0">
                            </div>
                            <div class="input-group">
                                <label for="GS">Server off</label>
                                <input type="number" id="GS" value="0">
                            </div>
                            <!-- ... resto de los inputs ... -->
                        </div>
                        <button class="config-button">Enviar</button>
                    </div>
                </div>
            </div>
        </div>
    </main>
    <script type="module" src="script.javascript">
    </script>
</body>
</html>
)====";
const String paginaJS = R"====(
// Objeto para almacenar referencias a elementos DOM
const elements = {
    sliders: {
        voltage: {
            slider: document.querySelector('#voltageSlider'),
            value: document.querySelector('#voltageValue')
        },
        current: {
            slider: document.querySelector('#currentSlider'),
            value: document.querySelector('#currentValue')
        }
    },
    inputs: {
        VMax: document.querySelector('#VMax'),
        VMin: document.querySelector('#VMin'),
        MBal: document.querySelector('#MBal'),
        MRec: document.querySelector('#MRec'),
        IMax: document.querySelector('#IMax'),
        OVP: document.querySelector('#OVP'),
        OVPR: document.querySelector('#OVPR'),
        UVP: document.querySelector('#UVP'),
        UVPR: document.querySelector('#UVPR'),
        VBal: document.querySelector('#VBal'),
        CCP: document.querySelector('#CCP'),
        DCP: document.querySelector('#DCP'),
        Cap: document.querySelector('#Cap'),
        TMin: document.querySelector('#TMin'),
        TMax: document.querySelector('#TMax')
    }
};

// Modo oscuro
const themeToggle = document.getElementById('themeToggle');
themeToggle.addEventListener('click', function() {
    const html = document.documentElement;
    html.setAttribute('data-theme', html.getAttribute('data-theme') === 'light' ? 'dark' : 'light');
});

// Control buttons
document.querySelectorAll('.control-button').forEach(button => {
    button.addEventListener('click', function() {
        this.classList.toggle('on');
        this.classList.toggle('off');
        
        const function_name = this.getAttribute('data-function');
        const state = this.classList.contains('on') ? 'true' : 'false';
        let configData = {};
        configData[function_name] = state;

        fetch ('/conf',{method:'POST',headers:{'Content-Type': 'application/json'},body: JSON.stringify(configData)});
        console.log(configData);
        //console.log(`${function_name} ${state}`);
    });
});



const t = 4; //segundos
let datap = {
    bat1: 3.1, bat2: 3.2, bat3: 3.3, bat4: 3.4, bat5: 3.5,
    bat6: 3.6, Total: 23, Current: 150, Temperature: 35, 
}
// Función para actualizar valores de monitoreo
function updateMonitoringValues() {
    fetch("/lec")
        .then(response => response.json())
        .then(data => {
            let soc = {};
            for (let key in data) {
                if (data.hasOwnProperty(key)) {
                    let element = document.getElementById(key);
                    if (element) {
                        element.textContent = data[key].toFixed(2);
                        let x = ((parseFloat(datap[key]) - elements.inputs.VMin.value) / 
                                (elements.inputs.VMax.value - elements.inputs.VMin.value));
                        x += ((parseFloat(datap["Current"]) * t) / (elements.inputs.Cap.value * 3600));
                        x = x * 100;
                        soc["soc" + key] = x;
                    }
                }
            }
            
            // Calcular SOC total y otros valores
            calculateAndUpdateSOC(soc, data);
            datap = data;
        });
}

function calculateAndUpdateSOC(soc, data) {
    soc.socTotal = Object.keys(soc)
        .filter(key => key.startsWith('socbat'))
        .reduce((sum, key) => sum + soc[key], 0) / 6;
    
    soc.socCurrent = (data.Current / elements.inputs.IMax.value) * 100;
    soc.socTemperature = (data.Temperature / 
        (elements.inputs.TMax.value - elements.inputs.TMin.value)) * 100;

    updateSOCDisplay(soc);
}

function updateSOCDisplay(soc) {
    for (let key in soc) {
        if (soc.hasOwnProperty(key)) {
            let fillElement = document.getElementById(key);
            let textElement = document.getElementById(key + "Text");
            
            if (fillElement) {
                fillElement.style.width = `${soc[key]}%`;
                fillElement.className = 'percentage-fill ' + 
                    getColorClass(key.includes('Current') || key.includes('Temperature') ? 
                        Math.abs(soc[key] - 120) : soc[key]);
            }
            if (textElement) {
                textElement.textContent = `${soc[key].toFixed(0)}%`;
            }
        }
    }
}

function getColorClass(percentage) {
    if (percentage > 70) return 'high';
    if (percentage > 30) return 'medium';
    return 'low';
}

// Función para manejar los sliders con bidireccionalidad
function initializeSliders() {
    function updateVoltageValues() {
        const volMax = parseFloat(elements.inputs.VMax.value);
        const volMin = parseFloat(elements.inputs.VMin.value);
        const mRec = parseFloat(elements.inputs.MRec.value);
        const mBal = parseFloat(elements.inputs.MBal.value);
        
        const margen = volMax - volMin;
        const sliderValue = elements.sliders.voltage.slider.value;
        const nuevoValor = volMax - ((sliderValue / 100) * margen);

        elements.inputs.OVP.value = volMax;
        elements.inputs.OVPR.value = (volMax - mRec/10).toFixed(2);
        elements.inputs.UVP.value = nuevoValor.toFixed(2);
        elements.inputs.UVPR.value = (nuevoValor + mRec/10).toFixed(2);
        elements.inputs.VBal.value = (volMax - mBal/10).toFixed(2);
    }

    function updateCurrentValues() {
        const iMax = parseFloat(elements.inputs.IMax.value);
        const sliderValue = elements.sliders.current.slider.value;
        const nuevoValor = (sliderValue / 100) * iMax;

        elements.inputs.CCP.value = nuevoValor.toFixed(0);
        elements.inputs.DCP.value = nuevoValor.toFixed(0);
    }

    function updateSlidersFromInputs() {
        // Actualizar slider de voltaje
        const volMax = parseFloat(elements.inputs.VMax.value);
        const volMin = parseFloat(elements.inputs.VMin.value);
        const uvp = parseFloat(elements.inputs.UVP.value);
        const margen = volMax - volMin;
        const voltagePercentage = ((volMax - uvp) / margen) * 100;
        elements.sliders.voltage.slider.value = voltagePercentage;
        updateSliderDisplay('voltage');

        // Actualizar slider de corriente
        const iMax = parseFloat(elements.inputs.IMax.value);
        const ccp = parseFloat(elements.inputs.CCP.value);
        const currentPercentage = (ccp / iMax) * 100;
        elements.sliders.current.slider.value = currentPercentage;
        updateSliderDisplay('current');
    }

    function updateSliderDisplay(sliderType) {
        elements.sliders[sliderType].value.textContent = 
            `${elements.sliders[sliderType].slider.value}%`;
    }

    // Event listeners
    elements.sliders.voltage.slider.addEventListener('input', () => {
        updateSliderDisplay('voltage');
        updateVoltageValues();
    });

    elements.sliders.current.slider.addEventListener('input', () => {
        updateSliderDisplay('current');
        updateCurrentValues();
    });

    // Event listeners para inputs relevantes
    ['UVP', 'CCP'].forEach(inputId => {
        elements.inputs[inputId].addEventListener('input', updateSlidersFromInputs);
    });

    // Inicialización
    updateVoltageValues();
    updateCurrentValues();
    updateSliderDisplay('voltage');
    updateSliderDisplay('current');
}

// Funciones de navegación y configuración
function initializeNavigation() {
    const navButtons = document.querySelectorAll('.nav-button');
    const configPanel = document.getElementById('configPanel');
    const monitoringPanel = document.querySelector('.monitoring-panel');

    navButtons.forEach(button => {
        button.addEventListener('click', function() {
            configPanel.style.display = this.textContent === 'Configuración' ? 'block' : 'none';
            monitoringPanel.style.display = this.textContent === 'Lecturas' ? 'grid' : 'none';
        });
    });
}

function initializeConfigButtons() {
    const configButtons = document.querySelectorAll('.config-button');
    
    configButtons.forEach((button, index) => {
        button.addEventListener('click', function() {
            const card = index === 0 ? 
                this.closest(".card").nextElementSibling : 
                this.closest('.card');
            
            const inputs = card.querySelectorAll('input');
            let configData = {};
            
            inputs.forEach(input => configData[input.id] = input.value);
            
            fetch('/conf', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify(configData)
            });
        });
    });
}

function initializeCollapsibles() {
    const collapsibles = document.querySelectorAll('.collapsible');
    
    collapsibles.forEach(button => {
        button.addEventListener('click', function() {
            this.classList.toggle('active');
            const content = this.nextElementSibling;
            
            content.style.maxHeight = content.style.maxHeight ? 
                null : `${content.scrollHeight}px`;
            
            collapsibles.forEach(otherButton => {
                if (otherButton !== this) {
                    otherButton.classList.remove('active');
                    otherButton.nextElementSibling.style.maxHeight = null;
                }
            });
        });
    });
}

// Inicialización cuando el DOM está listo
document.addEventListener('DOMContentLoaded', function() {
    initializeSliders();
    initializeNavigation();
    initializeConfigButtons();
    initializeCollapsibles();
});

// Carga inicial de datos
window.addEventListener("load",async () => {
    try{
        setTimeout( () => {
            fetch("/datasaved")
            .then(response => response.json())
            .then( data =>{
                for (let key in data) {
                    if (data.hasOwnProperty(key)) {
                        let element = document.getElementById(key);
                        if (element) {
                            if (key == "carga" || key == "descarga" || key == "balance" || key == "emergencia") {
                                element.classList.remove("on" , "off");
                                if (data[key]) {
                                    element.classList.add("on");
                                }else {
                                    element.classList.add("off");
                                }
                            }else {
                                element.value = data[key];
                            }
                        }
                    }
                }
            })
            initializeSliders();
        },2000)
    }catch (error){console.error("red no disponible");}
})

// Iniciar el intervalo de monitoreo
setInterval(updateMonitoringValues, t * 1000);
)====";
const String paginaCSS = R"====(:root {
        /* Modo claro */
        --primary-color: #1a237e;
        --secondary-color: #3949ab;
        --background-color: #f0f2f5;
        --card-background: #ffffff;
        --text-color: #333333;
        --text-color-h: #efefef;
        --button-off-bg: #e0e0e0;
        --button-off-color: #757575;
        --button-on-bg: #4caf50;
        --button-on-color: white;
    }

    [data-theme="dark"] {
        --primary-color: #000051;
        --secondary-color: #1a237e;
        --background-color: #121212;
        --card-background: #1e1e1e;
        --text-color: #ccc;
        --text-color-h: #ddd;
        --button-off-bg: #424242;
        --button-off-color: #bdbdbd;
        --button-on-bg: #1b5e20;
        --button-on-color: white;
    }

    * {
        margin: 0;
        padding: 0;
        box-sizing: border-box;
        font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
    }

    body {
        background: var(--background-color);
        color: var(--text-color);
        transition: all 0.3s ease;
    }

    .container {
        max-width: 100%;
        margin: 0 auto;
        padding: 20px;
    }

    header {
        background: var(--primary-color);
        color: var(--text-color-h);
        padding: 20px 0;
        box-shadow: 0 2px 4px rgba(0,0,0,0.1);
    }

    .header-content {
        display: flex;
        justify-content: space-between;
        align-items: center;
        flex-wrap: wrap;
    }

    .logo {
        font-size: 24px;
        font-weight: bold;
        text-transform: uppercase;
    }

    nav {
        display: flex;
        gap: 10px;
        align-items: center;
    }

    .nav-button {
        padding: 10px 20px;
        border: none;
        border-radius: 5px;
        background: var(--secondary-color);
        color: white;
        cursor: pointer;
        transition: all 0.3s ease;
    }

    .nav-button:hover {
        opacity: 0.9;
        transform: translateY(-2px);
    }

    .theme-toggle {
        background: none;
        border: none;
        color: white;
        cursor: pointer;
        font-size: 24px;
        padding: 5px;
    }

    .controls {
        margin-top: 20px;
        display: grid;
        grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
        gap: 20px;
    }

    .control-button {
        padding: 20px;
        border: none;
        border-radius: 10px;
        font-size: 18px;
        font-weight: bold;
        cursor: pointer;
        transition: all 0.3s ease;
        box-shadow: 0 4px 6px rgba(0,0,0,0.1);
    }

    .control-button.off {
        background: var(--button-off-bg);
        color: var(--button-off-color);
    }

    .control-button.on {
        background: var(--button-on-bg);
        color: var(--button-on-color);
    }

    .control-button:hover {
        transform: translateY(-3px);
        box-shadow: 0 6px 8px rgba(0,0,0,0.2);
    }

    .monitoring-panel {
        margin-top: 40px;
        display: grid;
        grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
        gap: 20px;
    }

    .card {
        background: var(--card-background);
        border-radius: 10px;
        padding: 20px;
        box-shadow: 0 4px 6px rgba(0,0,0,0.1);
    }

    .card h2 {
        margin-bottom: 15px;
        font-size: 18px;
    }

    .battery-grid {
        display: grid;
        grid-template-columns: repeat(3, 1fr);
        gap: 10px;
    }

    .battery-cell {
        text-align: center;
        padding: 10px;
        background: var(--background-color);
        border-radius: 5px;
    }

    .battery-cell p {
        margin-bottom: 5px;
        font-weight: bold;
    }

    .battery-cell span {
        font-size: 18px;
    }

    .total-values {
        display: grid;
        grid-template-columns: repeat(3, 1fr);
        gap: 10px;
        text-align: center;
    }

    .total-value {
        background: var(--background-color);
        padding: 10px;
        border-radius: 5px;
    }

    .total-value p {
        margin-bottom: 5px;
        font-weight: bold;
    }

    .total-value span {
        font-size: 24px;
    }

    @media (max-width: 768px) {
        .header-content {
            flex-direction: column;
            text-align: center;
        }

        nav {
            margin-top: 20px;
            flex-wrap: wrap;
            justify-content: center;
        }

        .controls {
            grid-template-columns: repeat(auto-fit, minmax(150px, 1fr));
        }
    }

    /* porcentajes */

    .battery-cell, .total-value {
        position: relative;
    }

    .percentage-bar {
        width: 100%;
        height: 4px;
        background-color: #e0e0e0;
        border-radius: 2px;
        margin-top: 5px;
        overflow: hidden;
    }

    .percentage-bar.large {
        height: 6px;
        margin-top: 10px;
    }

    .percentage-fill {
        height: 100%;
        background-color: #4caf50;
        transition: width 0.3s ease;
    }

    .percentage-text {
        font-size: 12px;
        color: var(--text-color);
        opacity: 0.7;
        margin-left: 5px;
    }

    /* Colores para diferentes niveles de porcentaje */
    .percentage-fill.high { background-color: #4caf50; }
    .percentage-fill.medium { background-color: #ffc107; }
    .percentage-fill.low { background-color: #f44336; }

    /* coniguraciones seccion*/

    /* Estilos para la sección de configuraciones */
    .slider-container {
        margin-bottom: 20px;
    }

    .slider {
        width: 100%;
        height: 8px;
        background: var(--button-off-bg);
        border-radius: 4px;
        outline: none;
        -webkit-appearance: none;
    }

    .slider::-webkit-slider-thumb {
        -webkit-appearance: none;
        width: 20px;
        height: 20px;
        background: var(--secondary-color);
        border-radius: 50%;
        cursor: pointer;
    }

    .input-grid {
        display: grid;
        grid-template-columns: repeat(auto-fill, minmax(200px, 1fr));
        gap: 15px;
    }

    .input-group {
        display: flex;
        flex-direction: column;
    }

    .input-group label {
        margin-bottom: 5px;
        font-weight: 500;
    }

    .input-group input {
        padding: 8px;
        border: 1px solid var(--button-off-bg);
        border-radius: 4px;
        background: var(--card-background);
        color: var(--text-color);
    }

    .config-button {
        margin-top: 15px;
        padding: 10px 20px;
        background: var(--secondary-color);
        color: var(--text-color-h);
        border: none;
        border-radius: 4px;
        cursor: pointer;
        transition: all 0.3s ease;
    }

    .config-button:hover {
        opacity: 0.9;
    }

    /* Estilos para secciones desplegables */
    .collapsible-section {
        margin-top: 20px;
    }

    .collapsible {
        background-color: var(--secondary-color);
        color: var(--text-color-h);
        cursor: pointer;
        padding: 18px;
        width: 100%;
        border: none;
        text-align: left;
        outline: none;
        font-size: 16px;
        transition: 0.3s;
        border-radius: 5px;
        display: flex;
        justify-content: space-between;
        align-items: center;
    }

    .collapsible:after {
        content: '\002B';
        color: var(--text-color-h);
        font-weight: bold;
        float: right;
        margin-left: 5px;
        transition: 0.3s;
    }

    .collapsible.active:after {
        content: "\2212";
        transform: rotate(180deg);
    }

    .collapsible:hover {
        background-color: var(--primary-color);
    }

    .collapsible-content {
        max-height: 0;
        overflow: hidden;
        transition: max-height 0.3s ease-out;
        background-color: var(--card-background);
    }

    .collapsible-content .card {
        margin-top: 0;
        border-top-left-radius: 0;
        border-top-right-radius: 0;
    })====";


#endif