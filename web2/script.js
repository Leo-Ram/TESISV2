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
        const state = this.classList.contains('on') ? 'activada' : 'desactivada';
        console.log(`Función ${function_name} ${state}`);
    });
});

// Simulación de recepción de datos
function updateValues(data, soc) {
    const values = data.split(',').map(Number);
    const socValues = soc.split(',').map(Number);
    
    // Actualizar voltajes y SOC de baterías
    for (let i = 1; i <= 6; i++) {
        const batteryValue = values[i-1].toFixed(1);
        const socValue = socValues[i-1];
        
        document.getElementById(`bat${i}`).textContent = batteryValue;
        document.getElementById(`soc${i}`).style.width = `${socValue}%`;
        document.getElementById(`socText${i}`).textContent = `${socValue}%`;


        
        // Actualizar color basado en el porcentaje
        const socFill = document.getElementById(`soc${i}`);
        socFill.className = 'percentage-fill ' + getColorClass(socValue);
    }
    // Actualizar valores totales y sus SOC
    const totalElements = [
        {value: values[6].toFixed(0), soc: socValues[6], id: 'Total'},
        {value: values[7].toFixed(0), soc: socValues[7], id: 'Current'},
        {value: values[8].toFixed(0), soc: socValues[8], id: 'Temperature'}
    ];
    
    totalElements.forEach(elem => {
        document.getElementById(`${elem.id}`).textContent = elem.value;
        document.getElementById(`soc${elem.id}`).style.width = `${elem.soc}%`;
        document.getElementById(`socText${elem.id}`).textContent = `${elem.soc}%`;
        
        const socFill = document.getElementById(`soc${elem.id}`);    
        if (elem.id == "Total") {
            socFill.className = 'percentage-fill ' + getColorClass(elem.soc);
        }else{
            socFill.className = 'percentage-fill ' + getColorClass((Math.abs(elem.soc-100)));
        }
    });
}

function getColorClass(percentage) {
    if (percentage > 70) return 'high';
    if (percentage > 30) return 'medium';
    return 'low';
}

// Simular obtención de datos cada 5 segundos
setInterval(() => {
    const mockData = '3.1,3.2,3.3,3.4,3.5,3.6,21.1,500,50';
    const mockSOC = '90,85,80,75,70,65,80,60,20';
    updateValues(mockData, mockSOC);
}, 5000);

// Actualización inicial
updateValues('3.1,3.2,3.3,3.4,3.5,3.6,21.1,500,50', '90,85,80,75,70,65,80,60,45');

// configurationes seccion

// Función para manejar los sliders
function initializeSliders() {
    const voltageSlider = document.getElementById('voltageSlider');
    const currentSlider = document.getElementById('currentSlider');
    const voltageValue = document.getElementById('voltageValue');
    const currentValue = document.getElementById('currentValue');

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
    
    configButtons.forEach(button => {
        button.addEventListener('click', function() {
            const card = this.closest('.card');
            const inputs = card.querySelectorAll('input');
            const configData = {};
            
            inputs.forEach(input => {
                configData[input.id] = input.value;
            });
            
            console.log('Enviando configuración:', configData);
            // Aquí irá la lógica para enviar los datos al servidor
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
    initializeCollapsibles(); // Añade esta línea
});