/**
 * @file builtinfiles.h
 * @brief This file is part of the WebServer example for the ESP8266WebServer.
 *
 * This file contains long, multiline text variables for  all builtin resources.
 */

// used for $upload.htm
static const char ctrlContent[] PROGMEM = 
R"==(  
  <!doctype html>
<html lang='en'>

<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Controlador</title>
</head>

<body>
  <div style="width: 100%; display: flex; flex-direction: column; justify-content: center; align-items: center;">
    <h1>Controlador</h1>
  </div>
    <hr>
  <div style="width: 100%; display: flex; flex-direction: column; justify-content: center; align-items: center;"> 
    <h2>Led </h2>
    <input id="a" type="number"></input>
    <h2>Servo motor</h2>
    <input id="b" type="number"></input>
    <br/>
    <button id="ctrl" style="margin: 10px;">Enviar</button>
    <br/>
    <p id="res"></p>
  </div>
  <script>
    let a = document.getElementById('a');
    let b = document.getElementById('b');
    let ctrl = document.getElementById('ctrl');

    ctrl.addEventListener('click', () => {
     //talvez precise colocar METHOD GET
      fetch(`/ctrl?a=${+a.value}&b=${+b.value}`,).then(response => {
        response.json().then(json => {
          res.textContent = "Enviado!"
        })        
      });  
    
     }, false)
  </script>
</body>
)==";

// used for $upload.htm
static const char notFoundContent[] PROGMEM = R"==(
<html>
<head>
  <title>Resource not found</title>
</head>
<body>
  <p>The resource was not found.</p>
  <p><a href="/">Start again</a></p>
</body>
)==";
