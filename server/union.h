#ifndef PAGINA_H
#define PAGINA_H

const String paginaHTML = R"====(<!DOCTYPE html>
<html lang='en'>
<head>
    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>BMS</title>
    <link rel='stylesheet' href='style.css'>
</head>
<body>
    <header>
        <div>
            <h2 class='header'>UNIVERSIDAD DE NARIÑO</h2>
        </div>
    </header>
    <main>
        <div class='container-title'>
            <h1>BMS-local</h1>
        </div>
        <div class='container-q'>
            <div class='container-button' id='container-button-id'>
                <div class='button'>
                    <input type='checkbox' class='button-input' id='carga'>
                    <label for='carga' class='button-label'>Carga</label>
                </div>
                <div class='button'>
                    <input type='checkbox' class='button-input' id='descarga'>
                    <label for='descarga' class='button-label'>Descarga</label>
                </div>
                <div class='button'>
                    <input type='checkbox' class='button-input' id='balance'>
                    <label for='balance' class='button-label'>Balance</label>
                </div>
                <div class='button'>
                    <input type='checkbox' class='button-input' id='emergencia'>
                    <label for='emergencia' class='button-label'>Emergencia</label>
                </div>
            </div>
        </div>
        <div class='container-r'>
            <h2 class='h2-sub'>Voltajes en las baterias</h2>
            <div class='container-read' id='container-read-id'>
            </div>
        </div>
        <div class='container-r'>
            <h2 class='h2-sub'>Configuracion</h2>
            <form class='form'></form>
                <div class='container-form' id='container-form-id'>              
                </div>
                <div class='container-input enviar'>
                    <input type='button' value='enviar' id='form-button'>
                </div>
            </form>
        </div>
        <div class="container-r">
            <span id="disp"> = </span>
            <div id="disp-box">
                <input type="number" class="form-input input-conf" value="578">
                <input type="number" class="form-input input-conf" value="223">
                <input type="number" class="form-input input-conf" value="221">
                <input type="number" class="form-input input-conf" value="126">
                <input type="number" class="form-input input-conf" value="111">
                <input type="number" class="form-input input-conf" value="77">
                <input type="number" class="form-input input-conf" value="1">
                <input type="number" class="form-input input-conf" value="1">
                <input type="number" class="form-input input-conf" value="0">
                <input type="button" class="form-input" value="Guardar" id="volta">
            </div>      
        </div>
    </main>
    <script type="module">
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
            const configuraciones = ["OVP(V)", "OVPR(V)", "UVP(V)", "UVPR(V)", "VBalc(V)","CCP(A)", "DCP(A)", "Tmin(°C)", "Tmax(°C)"];
            const prec = [4.2,4,3.2,3.4,2.5,1.5,2,4,60];
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

        setInterval(getinfo,5000);

        function getinfo() {
            let conf = document.getElementsByClassName('form-input');
            let c= [] ;
            for (let i = 0; i < conf.length; i++) {
                c[i]= parseFloat(conf.item(i).value);        
            }
            const lecturas = ['Bat1', 'Bat2', 'Bat3', 'Bat4', 'Bat5', 'Bat6', 'VT','Cbat', 'T°C'];
            let objetojs = {timestamp: new Date().toISOString(),};
            console.log(objetojs);
            fetch('/informacion')
            .then(response => response.text())
            .then(data => {
                //data = '3.1,3.2,3.3,3.4,3.5,3.6,21.1,1,50';
                data = data.split(',');
                data = data.map(str => parseFloat(str));
                for (let i = 0; i < data.length; i++) {
                    document.getElementById(`read${i}`).textContent = data[i];
                    objetojs[lecturas[i]] = data[i];
                }
                for (let i = 0; i < 6; i++) {
                    let d = data[i];
                    d = (((d-c[2])/(c[0]-c[2]))*100);
                    d = d.toFixed(0)
                    d += '%';
                    document.getElementById(`readP${i}`).textContent = d;
                }
                c[0] = c[0] * 6;
                c[2] = c[2] * 6;
                data[6] = (((data[6]-c[2])/(c[0]-c[2]))*100);
                data[6] = data[6].toFixed(0);

                data[7] = (data[7]/c[5])*100;
                data[7] = data[7].toFixed(0);
                data[8] = (((data[8]-c[7])/(c[8]-c[7]))*100);
                data[8] = data[8].toFixed(0);
                for (let i = 6; i < data.length; i++){
                    document.getElementById(`readP${i}`).textContent = data[i] + '%';
                }
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
    </script>
</body>
</html>)====";

//const String paginaJS = R"====()====";

const String paginaCSS = R"====(* {
    box-sizing: border-box;
    margin: 0;
    padding: 0;
    background-color: #ddd9e4;
}

/* -----------------HEADER--------------*/
.header {
    background-color: #766893;
    color: #eee;
    padding: 12px;
    border-radius: 4px;
    margin: 0px 20px;
}
/* --------------------MAIN----------*/
.container-title {
    display: flex;
    justify-content: center;
    border-radius: 4px;
    padding: 10px;
    background-color: #988eae;
    margin: 0px 20px;
}
h1 {
    background-color: transparent;
    color: #222;
    padding: 10px;
}
.h2-sub {
    max-width: 1200px;
    margin: auto;
}
.container-q {    
    container-type: inline-size;
    container-name: button-q;
}
.container-button {
    display: grid;
    grid-template-columns: repeat(4,1fr);
    max-width: 1200px;
    margin: auto 
}
.button {
    padding: 0px;
    margin: 20px;
}
.button-label {
    display: block;
    text-align: center;
    padding: 4px;
    border: 2px solid #988eae;
    border-radius: 10px;
    background-color: #988eae;
    box-shadow: 0 0 2px 0 #988eae;
    color: #222;
}
.button-input {
    display: none;
}
.button-input:checked ~ .button-label {
    animation-name: cambiarc;
    animation-duration: 3s;
    animation-fill-mode: both;
}

@container button-q (max-width: 500px) {
    .container-button {
        grid-template-columns: repeat(2,1fr); 
    }
}
@container button-q (max-width:300px){
    .container-button {
        grid-template-columns: repeat(1,1fr); 
    }
}
@keyframes cambiarc {
    from {
        width: 30%;
    }
    to {
        background-color: #176;
        border: 2px solid #176;
        width: 100%;
        color: #fff;
    }
}
/* ------------Read----------*/
.container-r {    
    container-type: inline-size;
    container-name: read-r;
    padding: 20px;
}
.container-read, .container-form{
    max-width: 1200px;
    margin: auto;
    display: grid;
    grid-template-columns: repeat(auto-fit,minmax(200px,1fr));
    border: 2px solid #988eae;
    border-radius: 6px;
    padding: 20px;
    gap: 20px;
    background: linear-gradient(#bab4c9,transparent);
}
.container-read-items {
    display: grid;
    grid-template-columns: 40px repeat(2,1fr);
    background-color: transparent;
    box-shadow: 0 0 5px 0 #bab4c9;
    border-radius: 5px;
    padding: 2px;
}
.container-read-items:hover {
    box-shadow: 0 0 4px 0 #555 ;
}
label{
    margin-top: 7px;
    background-color: transparent;
}
.read-item {
    border-radius: 6px;
    margin: 4px;
    padding: 4px;
}
@container read-r (max-width:300px) {
    .container-read {
        grid-template-columns: minmax(40px, 1fr);
    }
    .container-read-items {
        grid-template-columns: minmax(40px, 1fr);
    }
    .read-item {
        min-width: 40px;
    }
}
/* -----------Configuracion------------- */
.container-form {
    grid-template-columns: repeat(auto-fit,minmax(120px,1fr));
}
.container-input {
    display: grid;
    grid-template-columns: 70px 1fr;
    background-color: transparent;
}
.form-input {
    min-width: 20px;
    padding: 4px;
    border-radius: 6px;
    border: none;
    outline: none;
    box-shadow: 0 0 5px 0 #bab4c9;
}
.form-input:hover {
    box-shadow: 0 0 4px 0 #555 
}
.enviar {
    margin: 12px;
    display: grid;
    grid-template-columns: minmax(60px,60px);
    place-content: center;
}
#form-button {
    padding: 5px;
    border-radius: 6px;
    border: 2px solid #988eae;
}
#form-button:hover {
    background-color: #988eae;
    box-shadow: 0 0 4px 0 #988eae;
}
#disp-box {
    display: none;
}
)====";

#endif