import React, {Component} from 'react';
import logo from './logo.svg';
import './App.css';
import { decode } from "@msgpack/msgpack";



const ws = new WebSocket("ws://127.0.0.1:13049");
ws.binaryType = "blob";
var buffer;
var connected = false;

class App extends Component {

    state = {
    };



    // need to use this.. why? something about loading this when page loads.
    UNSAFE_componentWillMount() {
        document.addEventListener('keydown', this.logKey);

        ws.onopen = () => {
            console.log("Connected");
            connected = true;
            // connected = true;
            document.getElementById("connection_info").innerHTML = "Connected!";
        };

        ws.onclose = () => {
            // connected = false;
            document.getElementById("connection_info").innerHTML = "Not Connected";
        };

        ws.onmessage =  (event) => {
            // TODO: maybe move this somewhere else...
            // create file reader object:
            var reader = new FileReader();
            // arrow function to retain 'this' & handle byte array conversion + pass to decoder:
            reader.onload = (e) =>{
                buffer = new Uint8Array(e.target.result);  // arraybuffer object
                // binary decoding works! just need to pack correctly...
                var decoded = decode(buffer);
                //  send image to its own function:
                if ("image_binary" in decoded){
                    // parse & update image...
                    this.updateImage(decoded["image_binary"]);
                }
                if ("message" in decoded){
                    // parse & update image...
                    document.getElementById("message").innerHTML = "message Received: " + decoded.message;
                }
                if ("loopback" in decoded){
                    // send back response
                    ws.send("message received");
                }
            };
            // call function to decode data:
            reader.readAsArrayBuffer(event.data);

        };
    };

    // local key logger that is attached to whole window; can be focused to specific iframe..
    // I think this can be src'ed into an iFrame from a different source...
    // this will repeat on hold since 'keydown' code is used
    logKey = (e) =>{
        console.log(` ${e.code}`);
    };

    updateImage = (data) =>{
        console.log(typeof(data));
        console.log(data);
        var blob = new Blob ([data]);
        console.log(blob);
        document.querySelector("#image").src = URL.createObjectURL(blob);
        document.querySelector("#image").width = 200;
    };

     myFunction = () => {
        console.log("button clicked");
        if (connected){
            ws.send("the button was clicked");
        }
    };



    render(){
        return(
        <div className="App">
            <header className="App-header">
                <img src={logo} className="App-logo" alt="logo" />
                <div id="connection_info">Not Connected.</div>
                <div id="button_panel">
                    <input id="myInput" value="Type your input here.."></input>
                    <button type="button" id="myBtn" onClick={this.myFunction}>Try it</button>
                </div>
                <div id="message"></div>
                <img id="image" alt=""/>
            </header>
        </div>
        );

    }
}

export default App;
