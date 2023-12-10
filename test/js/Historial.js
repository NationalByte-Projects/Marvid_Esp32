
  
  var i;
  var NewLoad = new XMLHttpRequest();
  var Marcadores =[];

  var marker, i, x;
  var initvar = 0;
  var endvar = 0;
  NewLoad.onload = function() {
    Respuesta= JSON.parse(this.responseText);
    var longitudRespuesta = Respuesta.length;
    var longitudMarcador = Marcadores.length;
    var ConcatString ='<table class="Tabla_Datos"><thead><tr><th>Nombre beacon</th><th>Alerta</th><th>Fecha</th></tr></thead><tbody>';
    var EndConcatString = '</tbody></table>';
    var Data_Alert ="";
    
    
    if(longitudRespuesta>0){        
        for (i = 0; i < longitudRespuesta; i++) { 
          ConcatString = ConcatString.concat(Respuesta[i][0]);

        }
        ConcatString=ConcatString.concat(EndConcatString);
        $('#Tabla_Datos').html(ConcatString);
    }
};

NewLoad.open("get", "PHP/Get_Historial.php", true);
NewLoad.send();  

 
  setInterval(function(){ 
    NewLoad.open("get", "PHP/Get_Historial.php", true);
    NewLoad.send();   
  },10000);
  
