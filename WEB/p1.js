function validacion() {
    var correcto = true;
    var password = document.getElementById("psswd").value;
    var psswordchk = document.getElementById("chkpsswd").value;
    var email = document.getElementById("email").value;
    var tlf = document.getElementById("tlf").value;
    var metodosDeEnvio = document.getElementsByName("metodoDeEnvio");
    var urlsAccion = document.getElementsByName("urlAEnviar");
    var filtro = /^([a-zA-Z0-9_\.\-])+\@(([a-zA-Z0-9\-])+\.)+([a-zA-Z0-9]{2,4})+$/;

    if (password != "" && (password != psswordchk)) {
        document.getElementById("errorContrasenha").style.visibility = "visible";
        correcto = false;
    }//comprobacion de password
    if (tlf != "" && (tlf.length != 9 || isNaN(tlf))) {
        document.getElementById("errorTlf").style.visibility = "visible";
        correcto = false;
    }//comprobación de telefono
    if (email != "" && !filtro.test(email)) {
        document.getElementById("errorEmail").style.visibility = "visible";
        correcto = false;
    }//comprobacion de correo
    if (!correcto)
        alert("Revisa los datos");
    if (metodosDeEnvio[0].checked)
        document.getElementById("form1").method = metodosDeEnvio[0].value;
    if (metodosDeEnvio[1].checked)
        document.getElementById("form1").method = metodosDeEnvio[1].value;
    if (urlsAccion[0].checked)
        document.getElementById("form1").action = urlsAccion[0].value;
    if (urlsAccion[1].checked) 
        document.getElementById("form1").action = urlsAccion[1].value;

    var date = new Date(); 
    var hora = date.getDate() + "/"
                    + (date.getMonth()+1)  + "/" 
                    + date.getFullYear() + "  "  
                    + date.getHours() + ":"  
                    + date.getMinutes() + ":" 
                    + date.getSeconds();
    document.getElementById("hora").value = hora;
    document.getElementById("form1").enctype = document.getElementById("tipoCodificacion").value;
    return correcto;
}

function borrarArea() {
    var textA = document.getElementById("textComentarios");
    if (textA.value == "Como podriamos mejorar?:")
        textA.value = "";
}

function otros() {
    var chckbx = document.getElementById("otrosChk");
    if (chckbx.checked)
        document.getElementById("otroEstilo").disabled = false;
    else
        document.getElementById("otroEstilo").disabled = true;
    
}