
/*var locations = [
    ['Bondi Beach', -33.890542, 151.274856, 4],
    ['Coogee Beach', -33.923036, 151.259052, 5],
    ['Cronulla Beach', -34.028249, 151.157507, 3],
    ['Manly Beach', -33.80010128657071, 151.28747820854187, 2],
    ['Maroubra Beach', -33.950198, 151.259302, 1]
  ];
/// Variables de prueba
*/ 
  /*var map = new google.maps.Map(document.getElementById('map'), {
    zoom: 10,
    center: new google.maps.LatLng(-33.92, 151.25),
    mapTypeId: google.maps.MapTypeId.ROADMAP
  });

  var infowindow = new google.maps.InfoWindow();*/
  
  var i;
  var NewLoad = new XMLHttpRequest();
  var map = L.map('map').setView([23.81676571661237, -102.6059968289123],5);
  var Marcadores =[];
  L.tileLayer('http://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
  attribution: 'Map data &copy; <a href="http://openstreetmap.org">OpenStreetMap</a> contributors, <a href="http://creativecommons.org/licenses/by-sa/2.0/">CC-BY-SA</a>, Imagery © <a href="http://cloudmade.com">CloudMade</a>',
  maxZoom: 18
  }).addTo(map);
  L.control.scale().addTo(map);
  var marker, i, x;
  var initvar = 0;
  
  var endvar = 0;
  NewLoad.onload = function() {
    
    Respuesta= JSON.parse(this.responseText);

    var longitudRespuesta = Respuesta.length;
    var longitudMarcador = Marcadores.length;
    var ConcatString ='<table class="Tabla_Datos"><thead><tr><th>Estatus</th><th>Nombre beacon</th><th>Temperatura</th><th>Velocidad</th> <th>Tiempo de uso</th> <th>Horas faltantes</th><th>Ultima señal recibida</th></tr></thead><tbody>';
    var EndConcatString = '</tbody></table>';
    //var Data_Alert ="";
    
    
    if(longitudRespuesta>0){        
        for (i = 0; i < longitudRespuesta; i++) { 
          ConcatString = ConcatString.concat(Respuesta[i][3]);
          Data_Alert=(Respuesta[i][3]);

         // console.log(Data_Alert);
         /*var Data_array =[];
          for(x = 0; x < 7; x++){
            initvar=Data_Alert.indexOf('<th>', initvar)+4; 
            endvar=Data_Alert.indexOf('</th>', endvar);
            Data_array.push(Data_Alert.substring(initvar,endvar));
            endvar+=1;
            //console.log(Data_Alert.substring(initvar,endvar));
            //console.log(initvar);
            ///QUIERO SACAR DATOS INDIVIDUALES PARA LAS ALARMAS
          }    
          console.log(Data_array[1]); */
          /*if(Data_array[1]>85){
            alert("Sobretemperatura en "+Data_array[0]+": "+ Data_array[1]);
          }
          if(Data_array[2]>11){
            alert("Exceso de velocidad en "+Data_array[0]+": "+ Data_array[2]);
          }*/
          
          if(longitudMarcador<=i){           
            Marcadores[i] = new L.marker([Respuesta[i][1], Respuesta[i][2]],{draggable: false}).addTo(map).bindPopup(Respuesta[i][0]);
          }else{                
            Marcadores[i].setLatLng([Respuesta[i][1], Respuesta[i][2]]).bindPopup(Respuesta[i][0]).update();
          } 
        }
        ConcatString=ConcatString.concat(EndConcatString);
        $('#Tabla_Datos').html(ConcatString);
    }
};


NewLoad.open("get", "PHP/Get_Data.php", true);
NewLoad.send();  

/*$.ajax({
  url: 'PHP/Test.php',
  method: "GET",
  dataType: 'json',
  success: function(response) {
  $('#Tabla_Datos').html(response);
}
});*/

  /*setInterval(function(){
  for (i = 0; i < locations.length; i++) {  
    L.marker([locations[i][1], locations[i][2]],{draggable: false}).addTo(map).bindPopup(locations[i][0]);
    /*marker = new google.maps.Marker({
      position: new google.maps.LatLng(locations[i][1], locations[i][2]),
      map: map
    });

    google.maps.event.addListener(marker, 'click', (function(marker, i) {
      return function() {
        infowindow.setContent(locations[i][0]);
        infowindow.open(map, marker);
      }
    })(marker, i));*/
/*}},5000);*/
 
  setInterval(function(){ 
    NewLoad.open("get", "PHP/Get_Data.php", true);
    NewLoad.send();   
  },10000);
  
   
    /*setInterval(function(){ $.ajax({
    url: 'PHP/Test.php',
    method: "GET",
    dataType: 'json',
    success: function(response) {
    $('#Tabla_Datos').html(response);
}
}); }, 3000);*/

/*document.addEventListener('DOMContentLoaded',function(){
    //window.location="Testfafaf.html"

});*/