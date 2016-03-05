<html lang="es" >
<head>
    <meta charset="utf-8" />
    <title>Musi Call</title>
    <script type = "text/javascript" src="p1.js"></script>
    <link rel="stylesheet" href="p1.css">
    <link rel="icon" type="image/gif" href="favicon.gif" />
    <?php
function GetOrPost(){ //post devuelve 1 otros devuelve 0
	if($_SERVER['REQUEST_METHOD'] == 'POST')
		return 1;
	else
		return 0;
}
    ?>
</head>
<body>
    <div id="TODO">
       <div class="tituloPrincipal">
            <h1>Musi Call</h1>
           <hr />
       </div>
       <div id="datos">
		   <h3>Datos básicos:</h3>
		   <div class="dato_formulario"><b>Nombre:	</b><?php if(GetOrPost() == 1)
																echo $_POST['nombre'];
															else
																echo $_GET['nombre'];?></div>
		    <div class="dato_formulario"><b>Contraseña:	</b><?php if(GetOrPost() == 1)
																	 echo $_POST['psswd'];
																 else
																	 echo $_GET['psswd'];?></div>
		    <div class="dato_formulario"><b>Email:	</b><?php if(GetOrPost() == 1)
																	 echo $_POST['email'];
																 else
																	 echo $_GET['email'];?></div>
		   <div class="dato_formulario"><b>Telefono:	</b><?php if(GetOrPost() == 1)
																	 echo $_POST['tlf'];
																 else
																	 echo $_GET['tlf'];?></div>
		    <div class="dato_formulario"><b>Sexo:	</b><?php if(GetOrPost() == 1)
																	 echo $_POST['sexo'];
																 else
																	 echo $_GET['sexo'];?></div>
		   <hr />
		   <h3>Gustos musicales:</h3>
		   <?php 
					$array = array();
					$otro = '';
					if(GetOrPost() == 1){
						$otro = $_POST['otroEstilo'];
						$array = $_POST['generoMusica'];
					}
					else{
						$otro = $_GET['otroEstilo'];
						$array = $_GET['generoMusica'];
					}
                    if(empty($array)){
                        echo("Vemos que no has marcado ningún estilo musical, esperemos que aumentes tus gustos gracias a nostros");
                    }else{
					    echo("Has marcado estos estilos de musica: ");
					    foreach($array as $valor)
						    echo($valor . ", ");
					    if($otro != ''){
						    echo("<br />Añadiremos tu nuevo género musical indicado:" . $otro);
					    }
                    }
			?>
		   <hr />
           <?php $coment;
                 if(GetOrPost() == 1)
                     $coment = $_POST['textComentarios'];
                 else
                     $coment = $_GET['textComentarios'];
                 if($coment == "Como podriamos mejorar?:")
                     echo("Gracias por estar satisfecho con nuestros servicios.");
                 else
                    echo ("Gracias, tu comentario: " .$coment ."será tomado en cuenta" );
                 
                 ?>
		   <hr />
		   <div class="divisionOscura">
				<h3 class="dato_formulario">Información adicional</h3>
				<div class="dato_formulario"><b>Hora:	</b><?php
																if(GetOrPost() == 1)
																	echo $_POST['hora'];
																  else
																	echo $_GET['hora'];?></div>
				<div class="dato_formulario"><b>IP del servidor:	</b><?php echo $_SERVER['SERVER_ADDR'];?></div>
				<div class="dato_formulario"><b>Nombre del servidor:	</b><?php echo $_SERVER['SERVER_NAME'];?></div>
				<div class="dato_formulario"><b>método usado para enviar formulario:	</b><?php echo $_SERVER['REQUEST_METHOD'];?></div>
				<div class="dato_formulario"><b>Navegador usado:	</b><?php echo $_SERVER['HTTP_USER_AGENT'];?></div>
				<b>Carlos Daniel Garrido Suárez</b>
			   <a href="p1.html" >VOLVER</a>
		   </div>
	   </div>
    </div>
</body>
</html>

