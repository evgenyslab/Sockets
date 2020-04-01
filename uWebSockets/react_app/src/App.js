import React, {Component} from 'react';
import logo from './logo.svg';
import './App.css';
import { decode } from "@msgpack/msgpack";



const ws = new WebSocket("ws://127.0.0.1:13049");
ws.binaryType = "blob";
var buffer;

class App extends Component {

    state = {
        test_img: null
    };

    // need to use this.. why? something about loading this when page loads.
    UNSAFE_componentWillMount() {
        ws.onopen = () => {
            console.log("Connected");
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
    }


    updateImage = (data) =>{
        console.log(typeof(data));
        console.log(data);
        var blob = new Blob ([data]);
        console.log(blob);
        document.querySelector("#image").src = URL.createObjectURL(blob);
        document.querySelector("#image").width = 200;
    };

    render(){
        return(
        <div className="App">
            <header className="App-header">
                <img src={logo} className="App-logo" alt="logo" />
                <div id="connection_info">Not Connected.</div>
                <div id="message"></div>
                <img id="image" alt=""/>
            </header>
        </div>
        );

    }
}

export default App;
