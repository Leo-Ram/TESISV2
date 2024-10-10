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
        },2000)
    }catch (error){console.error("red no disponible");}
})

// Iniciar el intervalo de monitoreo
setInterval(updateMonitoringValues, t * 1000);