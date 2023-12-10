const datos= {
    Nombre: '',
    Correo: '',
    Mensaje: ''
}

const Nombre = document.querySelector('#Nombre');
const Correo = document.querySelector('#Correo');
const Mensaje = document.querySelector('#Mensaje');
const Formulario= document.querySelector('.Formulario');

Correo.addEventListener('input',leerTexto);
Nombre.addEventListener('input',leerTexto);
Mensaje.addEventListener('input',leerTexto);

Formulario.addEventListener('submit', function(evento){
    
    //Validacion de fommulario
    const {Nombre, Correo, Mensaje} = datos;
    if(Nombre==='' || Correo ==='' || Mensaje===''){
        MostrarText('Favor de llenar los campos obligatorios');
        evento.preventDefault();
        return;
    }else{
        Bienvenida("Datos enviados");
        console.log('Formulario enviado');
    }
});

function leerTexto(e){
    datos[e.target.id] = e.target.value;
}

function MostrarText (data){
    const Alerta = document.createElement('P');
    Alerta.textContent = data;
    Alerta.classList.add('Alerta');
    Formulario.appendChild(Alerta);
    setTimeout(() => {
        Alerta.remove();
    }, 3000);
}

function Bienvenida (data){
    const Bienvenido = document.createElement('P');
    Bienvenido.textContent = data;
    Bienvenido.classList.add('Bienvenido');
    Sesion.appendChild(Bienvenido);
    setTimeout(() => {
        Bienvenido.remove();
    }, 3000);
}




