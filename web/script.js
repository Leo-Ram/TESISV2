//script
import { initializeApp } from 'https://www.gstatic.com/firebasejs/10.12.5/firebase-app.js';
import { getAnalytics } from 'https://www.gstatic.com/firebasejs/10.12.5/firebase-analytics.js';
import { getDatabase, ref, set, push } from 'https://www.gstatic.com/firebasejs/10.12.5/firebase-database.js';

const firebaseConfig = {
    apiKey: "AIzaSyAChf6C2FTFydmMWaOlMRkd46AlebQ627o",
    authDomain: "bms-local-575d1.firebaseapp.com",
    databaseURL: "https://bms-local-575d1-default-rtdb.firebaseio.com",
    projectId: "bms-local-575d1",
    storageBucket: "bms-local-575d1.appspot.com",
    messagingSenderId: "484916931861",
    appId: "1:484916931861:web:61be6d20f8ce3cb7bbcd1c",
    measurementId: "G-TXNLS8C1TQ"
};


const app = initializeApp(firebaseConfig);
const analytics = getAnalytics(app);
const database = getDatabase(app); 
let datap = [3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 21.1, 500, 50];

medidas();
configuracion();
function medidas() {
    const container = document.getElementById('container-read-id');
    const data = '3000.1, 3000.2, 3000.3, 3000.4, 3000.5, 3000.6, 3000.7, 3000.8, 3000.9,';
    const lecturas = ['Bat1', 'Bat2', 'Bat3', 'Bat4', 'Bat5', 'Bat6', 'VT','Cbat', 'T°C',];
    const especial = ['mA', '°C'];
    const dataS = data.split(',');
    for (let i = 0; i < dataS.length -1; i++) {
        const label = document.createElement('label');
        label.setAttribute('for', `read${i} readP${i}`);
        label.innerHTML = lecturas[i];

        const containerReadItems = document.createElement('div');
        containerReadItems.className = 'container-read-items';
    
        const read1 = document.createElement('div');
        read1.setAttribute('id', `read${i}`);
        read1.className = 'read-item';
        if (i < 7){
            read1.innerHTML = dataS[i] + 'V';
        }else{
            read1.innerHTML = dataS[i] + especial[i-7];
        }          
        const readP1 = document.createElement('div');
        readP1.setAttribute('id', `readP${i}`);
        readP1.className = 'read-item';
        if (i < 6){
            readP1.innerHTML = dataS[i] + '%';
        }else if (i == 6){
            readP1.innerHTML = dataS[i] + '%1';
        }else {
            readP1.innerHTML = dataS[i] + '%2';
        }
        containerReadItems.appendChild(label);
        containerReadItems.appendChild(read1);
        containerReadItems.appendChild(readP1);

        container.appendChild(containerReadItems);                   
    }
}
function configuracion() {
    const container2 = document.getElementById('container-form-id');
    const configuraciones = ["OVP(V)", "OVPR(V)", "UVP(V)", "UVPR(V)", "VBalc(V)","CCP(mA)", "DCP(mA)", "Tmin(°C)", "Tmax(°C)","Cap(A/H)"];
    const prec = [4.2,4,3.2,3.4,2.5,1000,1000,4,60,1.8];
    configuraciones.forEach(element => {
        const containerdiv = document.createElement('div');
        containerdiv.className = 'container-input';

        const label = document.createElement('label');
        label.setAttribute('for', element);
        label.innerHTML = element;

        const inputs = document.createElement('input');
        inputs.setAttribute('type', 'text');
        inputs.setAttribute('id', element);
        inputs.className = 'form-input';
        inputs.value = prec[configuraciones.indexOf(element)];

        containerdiv.appendChild(label);
        containerdiv.appendChild(inputs);

        container2.appendChild(containerdiv);
    });
}

let t = 5000;
setInterval(getinfo,t);

function getinfo() {
    let conf = document.getElementsByClassName('form-input');
    let c= [] ;
    let soc = [];
    for (let i = 0; i < conf.length; i++) {
        c[i]= parseFloat(conf.item(i).value);        
    }
    let med = c[0]-c[2];
    const lecturas = ['Bat1', 'Bat2', 'Bat3', 'Bat4', 'Bat5', 'Bat6', 'VT','Cbat', 'T°C'];
    let objetojs = {timestamp: new Date().toISOString(),};
    console.log(objetojs);
    fetch('/informacion')
    .then(response => response.text())
    .then(data => {
        //data = '3.1,3.2,3.3,3.4,3.5,3.6,21.1,500,50';
        data = data.split(',');
        data = data.map(str => parseFloat(str));
        for (let i = 0; i < data.length; i++) {
            document.getElementById(`read${i}`).textContent = data[i];
            objetojs[lecturas[i]] = data[i];
        }
        let socvt = 0;
        for (let i = 0; i < 6; i++) {
            soc[i] = ((datap[i]-c[2])/med) + (((data[7]/1000)*(t/1000))/(c[9]*3600));
            soc[i] = soc[i].toFixed(2);
            soc[i] = soc[i] * 100
            socvt += soc[i];
            soc[i] = soc[i] + '%';
            document.getElementById(`readP${i}`).textContent = soc[i];
        }
        document.getElementById(`readP${6}`).textContent = (socvt/6) + '%';
        document.getElementById(`readP${7}`).textContent = ((data[7]/c[5])*100).toFixed(0) + '%';
        document.getElementById(`readP${8}`).textContent = (((data[8]-c[7])/(c[8]-c[7]))*100).toFixed(0) + '%';
        datap = data;
        enviar('data',objetojs)
    })
}

function enviar(ruta, datos) {
    const nRefer = push(ref(database, ruta)); // Crea una nueva referencia con una clave única
    set(nRefer, datos)
    .then(() => {
        console.log('Datos enviados correctamente');
    })
    .catch((error) => {
        console.error('Error al enviar los datos: ', error);
    });
}

document.getElementById('container-button-id').addEventListener('click', function() {
    setTimeout(() => {
        console.log('hola estados');
        const cargar = document.getElementById('carga');
        const descargar = document.getElementById('descarga');
        const balancear = document.getElementById('balance');
        const emergencias = document.getElementById('emergencia');
        let datosconf = [cargar.checked,descargar.checked,balancear.checked,emergencias.checked];
        fetch ('/boton',{method:'POST',body: datosconf});
        console.log(datosconf);
    }, 1000); 
})
document.getElementById('form-button').addEventListener('click', function() {
    let configdata = document.getElementsByClassName('form-input');
    let info = '';
    for (let i = 0; i < configdata.length ; i++) {
        if ( i<configdata.length -1 ){
            info += configdata.item(i).value +','; 
        }else {info += configdata.item(i).value}
    }
    console.log('informacion',info);
    fetch ('/config',{method: 'POST', body: info});
})
window.addEventListener('load',async () =>{
    try{
        setTimeout(() => {
            let lista = document.getElementsByClassName('form-input');
            let boton = document.getElementsByClassName('button-input');
            fetch('/data')
            .then(response => response.text())
            .then(data => {
                //data = '0,1,0,1,3.1,3.2,3.3,3.4,3.5,3.6,3.7,3.8,3.9';
                let datos = data.split(',');
                for (let i = 0; i < datos.length; i++) {
                    if (i<4){
                        if (datos[i] == '1'){
                            boton.item(i).checked = true;
                        }else{
                            boton.item(i).checked = false;
                        }
                    }else {
                        console.log(datos[i])
                        lista.item(i-4).value = datos[i]; 
                    }                                                 
                }
            })
        }, 1000);
    } catch (error){ console.error('error en red'); }
})

document.getElementById('disp').addEventListener('click', function() {
    const formulario = document.getElementById('disp-box');
    if (formulario.style.display === 'none') {
        formulario.style.display = 'block';
    } else {
        formulario.style.display = 'none';
    }
});

document.getElementById('volta').addEventListener('click', function() {
    let configdata = document.getElementsByClassName('input-conf');
    let info = '';
    for (let i = 0; i < configdata.length ; i++) {
        if ( i<configdata.length -1 ){
            info += configdata.item(i).value +','; 
        }else {info += configdata.item(i).value}
    }
    console.log('informacion',info);
    fetch ('/gain',{method: 'POST', body: info});
})

//fin