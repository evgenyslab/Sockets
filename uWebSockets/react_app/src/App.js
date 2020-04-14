import React, {Component} from 'react';
import logo from './logo.svg';
import './App.css';
import { decode } from "@msgpack/msgpack";

// variable for the inevitable websocket:
let ws = null;

class App extends Component {

    state = {
        url: "127.0.0.1:13049",
        message: "",
        connected: false,
    };



    UNSAFE_componentWillMount() {
        // maybe use state props for this..
        console.log(this.state.url);
    }
    // Attach functions to loaded components:
    componentDidMount() {
        // Attach function to main window div to capture keydown events:
        document.getElementById("main_window").addEventListener('keydown', this.logKey);

        // attach function to input object on "enter" key to send message
        document.getElementById("myInput").addEventListener("keyup", function(event) {
                // Number 13 is the "Enter" key on the keyboard
                if (event.keyCode === 13) {
                    // Cancel the default action, if needed
                    event.preventDefault();
                    // Trigger the button element with a click
                    document.getElementById("mySendBtn").click();
                }
            });
        // attach function to input object on "enter" key to send message
        document.getElementById("URLInput").addEventListener("keyup", function(event) {
            // Number 13 is the "Enter" key on the keyboard
            if (event.keyCode === 13) {
                // Cancel the default action, if needed
                event.preventDefault();
                // Trigger the button element with a click
                document.getElementById("myConnectBtn").click();
            }
        });
    };

    updateConnectionState = () => {
        console.log("attempting to change connection status:");
        if (this.state.connected){
            console.log("connected");
            document.getElementById("connection_info").innerHTML = "Connected!";
        }else{
            console.log("not connected");
            document.getElementById("connection_info").innerHTML = "Not Connected";
        }
    };

    loadWS = () =>{
        console.log("attempting to load ws...");
        ws = new WebSocket("ws://"+this.state.url);
        ws.binaryType = "blob";
        document.getElementById("connection_info").innerHTML = "Attempting connection!";
        ws.onopen = () => {
            console.log("blah");
            this.setState({
                connected:true,
            });
            this.updateConnectionState();
        };

        ws.onclose = () => {
            this.setState({
                connected:false,
            });
            this.updateConnectionState();
        };

        ws.onmessage =  (event) => {
            // TODO: maybe move this somewhere else...
            // create file reader object:
            var reader = new FileReader();
            // arrow function to retain 'this' & handle byte array conversion + pass to decoder:
            reader.onload = (e) =>{
                // buffer = new Uint8Array(e.target.result);  // <-- OLD!
                // binary decoding works! just need to pack correctly...
                var decoded = decode(new Uint8Array(e.target.result));
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

    unloadWS = () =>{
      ws.close();
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


     wsSend = () => {
       if (this.state.connected && this.state.message !== ""){
           console.log("attempting to send message: " + this.state.message);
           ws.send(this.state.message);
       }else{
           if (!this.state.connected)
               console.log("Can't send message, not connected");
           if (this.state.message === "")
               console.log("Message is undefined");
       }
     };

     updateMessage = (event) =>{
         // console.log(event.target.value);
         // this.message = event.target.value;
         this.setState({
             message: event.target.value
         })
     };

     updateURL = (event) =>{
         // can only dynamically update state of react component
         this.setState({
             url: event.target.value
         })
     };




    render(){
        return(
        <div className="App">
            <header className="App-header">
                <div id="main_window" tabIndex="0">
                    <img src={logo} className="App-logo" alt="logo" />
                    <div id="connection_info">Not Connected.</div>
                </div>
                <div id="buttons">
                    <input id="URLInput" type="text" value={this.state.url} onChange={this.updateURL}></input>
                    <button type="button" id="myConnectBtn" onClick={this.loadWS}>Connect</button>
                    <button type="button" id="myDisconnectBtn" onClick={this.unloadWS}>Disconnect</button>
                </div>
                <div id="input_window" tabIndex="0">
                    <input id="myInput" text="" value={this.state.message} onChange={this.updateMessage}></input>
                    <button type="button" id="mySendBtn" onClick={this.wsSend}>Send</button>
                </div>

                <div id="message"></div>
                <img id="image" alt=""/>
            </header>
        </div>
        );

    }
}

export default App;
