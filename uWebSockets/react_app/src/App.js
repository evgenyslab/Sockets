import React, {Component} from 'react';
import logo from './logo.svg';
import './App.css';
import { encode, decode } from "@msgpack/msgpack";



const ws = new WebSocket("ws://127.0.0.1:13049");
ws.binaryType = "blob";
var buffer;

class App extends Component {

    state = {
        test_img: null
    };

    componentWillMount() {
        ws.onopen = () => {
            console.log("Connected");
            // connected = true;
        };

        ws.onclose = () => {
            // connected = false;
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
                <div id="message"></div>
                <img id="image"/>
            </header>
        </div>
        );

    }
}

export default App;
