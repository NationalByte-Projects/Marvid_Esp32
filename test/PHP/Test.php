<?php
$contents = '<table class="Tabla_Datos">

<thead>
    <tr>
    <th>Nombre beacon</th> 
    <th>Temperatura</th> 
    <th>Velocidad</th> 
    <th>Tiempo de uso</th> 
    <th>Horas faltantes</th> 
    <th>Ultima señal recibida</th> 
    </tr>
</thead>
<tbody>
            <tr>
                <th>Beacon1</th> 
                <th>30°</th> 
                <th>10 km/h</th> 
                <th>400 hr</th> 
                <th>100 hr</th> 
            </tr>
            <tr>
                <th>Beacon2</th> 
                <th>30°</th> 
                <th>10 km/h</th> 
                <th>400 hr</th> 
                <th>100 hr</th> 
            </tr>
            <tr>
                <th>Beacon3</th> 
                <th>30°</th> 
                <th>10 km/h</th> 
                <th>400 hr</th> 
                <th>100 hr</th> 
            </tr>
I;


echo json_encode($contents);
?>