const datos= {
    Usuario: '',
    Password: ''
}

const Nombre = document.querySelector('#Usuario');
const Password = document.querySelector('#Password');
const Sesion= document.querySelector('.Sesion');
const Cerrar = document.querySelector("#Cerrar");

Usuario.addEventListener('input',leerTexto);
Password.addEventListener('input',leerTexto);

Cerrar.addEventListener('click', function(evento){
    evento.preventDefault();
    $.ajax({
        url: './PHP/Cerrar_Sesion.php',
        method: "Post",
        dataType: 'json',
        success: function(response) {
           if(response==="Cerrado"){
            Bienvenida("Sesion Cerrada");
           }else{               
               MostrarText("Intente de nuevo");
           }
        }
        }); 
});
Sesion.addEventListener('submit', function(evento){
    
    //Validacion de fommulario
    const {Nombre, Password} = datos;
    evento.preventDefault();
    if(Nombre==='' || Password ===''){
        MostrarText('Favor de llenar los campos obligatorios');      
        return;
    }else{
        SesionData();   
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
    Sesion.appendChild(Alerta);
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

function SesionData (){
    var User = document.getElementById("Usuario").value;
    var Pass = document.getElementById("Password").value;
    $.ajax({
        url: './PHP/Inicio_Sesion.php',
        method: "Post",
        dataType: 'json',
        data: {Userx: User,
        Passw: Pass
        },
        success: function(response) {
           console.log(response);
           if(response==="Error"){
               MostrarText("Usuario o Contraseña erronea");
           }else{
               Bienvenida(response);
               window.location.replace("./Datos.html");
           }
        }
        }); 
}







