// Modo oscuro
const themeToggle = document.getElementById('themeToggle');
themeToggle.addEventListener('click', function() {
    const html = document.documentElement;
    if (html.getAttribute('data-theme') === 'light') {
        html.setAttribute('data-theme', 'dark');
    } else {
        html.setAttribute('data-theme', 'light');
    }
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
    bat1: 3.1,
    bat2: 3.2,
    bat3: 3.3,
    bat4: 3.4,
    bat5: 3.5,
    bat6: 3.6,
    Total: 23,
    Current: 150,
    Temperature: 35, 
}
setInterval(() => {
    const valueVmax = document.getElementById("VMax").value;
    const valueVmin = document.getElementById("VMin").value;
    const valueCap = document.getElementById("Cap").value;
    const valueIMax = document.getElementById("IMax").value;
    const valueTMin = document.getElementById("TMin").value;
    const valueTMax = document.getElementById("TMax").value;
    let m = valueVmax - valueVmin;

    fetch("/lec")
        .then(response => response.json())
        .then(data => { 
/*        let data = {
            bat1: 3.1,
            bat2: 3.2,
            bat3: 3.3,
            bat4: 3.4,
            bat5: 3.5,
            bat6: 3.6,
            Total: 23,
            Current: 1000,
            Temperature: 24, 
        }*/
            let soc = {};
            //updateValues(data.data, data.soc);
            for (let key in data) {
                if (data.hasOwnProperty(key)) {
                    let element = document.getElementById(key);
                    if (element) {
                        element.textContent = data[key];
                        let x = ((parseFloat(datap[key])-valueVmin)/m)
                        x += ((parseFloat(datap["Current"]))*t)/(valueCap*3600);
                        x = x *100;
                        soc["soc"+key] = x;
                    }
                }
            }
            soc["socTotal"] = ((soc["socbat1"] + soc["socbat2"] + soc["socbat3"] + soc["socbat4"] + soc["socbat5"] + soc["socbat6"])/6);
            soc["socCurrent"] = (data["Current"]/valueIMax)*100;
            soc["socTemperature"] = (data["Temperature"]/(valueTMax-valueTMin))*100;
            for (let key in soc) {
                if (soc.hasOwnProperty(key)) {
                    let element = document.getElementById(key);
                    if (element) {
                        element.style.width = `${soc[key]}%`;
                        if (key == "socCurrent" || key == "socTemperature")  {
                            element.className = "percentage-fill " + getColorClass(Math.abs(soc[key]-120));
                        }else {
                            element.className = 'percentage-fill ' + getColorClass(soc[key]);
                        }                 
                    }
                    element = document.getElementById(key+"Text");
                    if (element) {
                        element.textContent = `${soc[key].toFixed(0)}%`;
                    }
                }
            }
            datap = data;
        });
    
}, (t*1000));

function getColorClass(percentage) {
    if (percentage > 70) return 'high';
    if (percentage > 30) return 'medium';
    return 'low';
}


// Función para manejar los sliders
function initializeSliders() {
    const voltageSlider = document.getElementById('voltageSlider');
    const currentSlider = document.getElementById('currentSlider');
    const voltageValue = document.getElementById('voltageValue');
    const currentValue = document.getElementById('currentValue');

    const valueVmax = document.getElementById("VMax");
    const valueVmin = document.getElementById("VMin");
    const valueMbal = document.getElementById("MBal");
    const valueMrec = document.getElementById("MRec");
    const valueImax = document.getElementById("IMax");

    const valueOVP = document.getElementById("OVP");
    const valueOVPR = document.getElementById("OVPR");
    const valueUVP = document.getElementById("UVP");
    const valueUVPR = document.getElementById("UVPR");
    const valueVbal = document.getElementById("VBal");
    const valueCCP = document.getElementById("CCP");
    const valueDCP = document.getElementById("DCP");
    

    voltageSlider.addEventListener("change", function(){
        let volMax = valueVmax.value;
        let margen = volMax - valueVmin.value;
        let nuevoValor = (voltageSlider.value / 100)*margen //+ valueVmax.value;
        nuevoValor = volMax - nuevoValor;
        valueOVP.value = volMax;
        valueOVPR.value = (volMax - (valueMrec.value)/10).toFixed(2);
        valueUVP.value = nuevoValor.toFixed(2);
        valueUVPR.value = (nuevoValor + (valueMrec.value)/10).toFixed(2);
        valueVbal.value = (volMax - (valueMbal.value)/10).toFixed(2);
    });

    currentSlider.addEventListener("change", function(){
        let imax = valueImax.value;
        let nuevoValor = (currentSlider.value / 100)*imax;
        valueCCP.value = nuevoValor.toFixed(0);
        valueDCP.value = nuevoValor.toFixed(0);

    });

    function updateSliderValue(slider, valueDisplay) {
        valueDisplay.textContent = slider.value + '%';
    }

    voltageSlider.addEventListener('input', () => updateSliderValue(voltageSlider, voltageValue));
    currentSlider.addEventListener('input', () => updateSliderValue(currentSlider, currentValue)); 
}

// Función para manejar la navegación entre secciones
function initializeNavigation() {
    const navButtons = document.querySelectorAll('.nav-button');
    const configPanel = document.getElementById('configPanel');
    const monitoringPanel = document.querySelector('.monitoring-panel');

    navButtons.forEach(button => {
        button.addEventListener('click', function() {
            if (this.textContent === 'Configuración') {
                configPanel.style.display = 'block';
                monitoringPanel.style.display = 'none';
            } else if (this.textContent === 'Lecturas') {
                configPanel.style.display = 'none';
                monitoringPanel.style.display = 'grid';
            }
        });
    });
}

// Función para manejar el envío de configuraciones
function initializeConfigButtons() {
    const configButtons = document.querySelectorAll('.config-button');
    
    configButtons.forEach((button,index) => {
        button.addEventListener('click', function() {
            let card;
            if (index === 0) {
                card = this.closest(".card").nextElementSibling;
            }else {
                card = this.closest('.card');
            }
            const inputs = card.querySelectorAll('input');
            let configData = {};
            
            inputs.forEach(input => {
                configData[input.id] = input.value;
            });
            
            console.log('Enviando configuración:', configData);
            // Aquí irá la lógica para enviar los datos al servidor
            fetch ('/conf',{
            method:'POST',
            headers:{'Content-Type': 'application/json'},
            body: JSON.stringify(configData)});
        });
    });
}

function initializeCollapsibles() {
    const collapsibles = document.querySelectorAll('.collapsible');
    
    collapsibles.forEach(button => {
        button.addEventListener('click', function() {
            this.classList.toggle('active');
            const content = this.nextElementSibling;
            
            if (content.style.maxHeight) {
                content.style.maxHeight = null;
            } else {
                content.style.maxHeight = content.scrollHeight + "px";
            }
            
            // Cerrar otras secciones
            collapsibles.forEach(otherButton => {
                if (otherButton !== this) {
                    otherButton.classList.remove('active');
                    otherButton.nextElementSibling.style.maxHeight = null;
                }
            });
        });
    });
}

// Añade esta función a tu inicialización
document.addEventListener('DOMContentLoaded', function() {
    initializeSliders();
    initializeNavigation();
    initializeConfigButtons();
    initializeCollapsibles(); 
});

// logica de acople
function ajustarValores() {
    console.log("ajustar valores.....");
}