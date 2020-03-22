import React, {Component} from 'react';
import logo from './logo.svg';
import './App.css';
//
// function App() {
//
//   var ws = new WebSocket("ws://127.0.0.1:13049");
//   var connected = false;
//   var myobj = {
//     field0: 20,
//     field2: "blah",
//     field3: 14.6,
//     field5: null,
//   };
//
//   var test_img = null;
//
//   ws.onopen = function(event) {
//     console.log("Connected");
//     connected = true;
//   };
//
//   ws.onclose = function(event){
//     connected = false;
//   };
//
//   ws.onmessage = function(event){
//     // if a byte stream is received...
//     if (typeof(event.data)=="object"){
//       console.log("message received");
//       // byte stream, this seems to work with read jpgs without any other processing
//       // why does this work?
//       // ONLY WORKS WITH JPEG ENCODED IMAGES!
//       // this should work with JPEGS
//       // Maybe need to use this:
//       // Make a Blob from the bytes
//       // var blob = new Blob([bytes], {type: 'image/bmp'});
//       //
//       // // Use createObjectURL to make a URL for the blob
//       // var image = new Image();
//       // image.src = URL.createObjectURL(blob);
//       // might be able to use msgpack for this too and encode on c++ side, decode here...
//       test_img = event.data;
//       // var blob = new Blob(event.data.arrayBuffer(), {type: "image/png"});
//       // var imageUrl = URL.createObjectURL(data);
//       // test_immg = URL.createObjectURL(data);
//       // document.querySelector("#image").src = imageUrl;
//     }else{
//       // string msg is received, might as well be a json dict....
//       console.log("message received");
//       var msg = JSON.parse(event.data);
//       console.log(msg);
//       if (msg["message"] !== undefined){
//         console.log(msg.message);
//         //document.getElementById("content").innerHTML = "message Received: " + msg.message;
//       }
//       if (msg["epoch"] !== undefined){
//         //document.getElementById("epoch").innerHTML = "Server time: " + msg.epoch;
//       }
//       if (msg["image"] !== undefined){
//         var imageUrl = URL.createObjectURL(msg["image"]);
//         //document.querySelector("#image").src = imageUrl;
//       }
//     }
//
//
//   };
//
//   return (
//     <div className="App">
//       <header className="App-header">
//         <img src={logo} className="App-logo" alt="logo" />
//
//         <img src={test_img}/>
//       </header>
//     </div>
//   );
// }

const ws = new WebSocket("ws://127.0.0.1:13049");

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

        ws.onmessage = (event) =>{
            // if a byte stream is received...
            console.log("msg received");
            if (typeof(event.data)=="object"){
                // byte stream, this seems to work with read jpgs without any other processing
                // why does this work?
                // ONLY WORKS WITH JPEG ENCODED IMAGES!
                // this should work with JPEGS
                // Maybe need to use this:
                // Make a Blob from the bytes
                // var blob = new Blob([bytes], {type: 'image/bmp'});
                //
                // // Use createObjectURL to make a URL for the blob
                // var image = new Image();
                // image.src = URL.createObjectURL(blob);
                // might be able to use msgpack for this too and encode on c++ side, decode here...
                this.updateImage(event);
                // document.querySelector("#image").src = URL.createObjectURL(event.data);
                // document.querySelector("#image").width = 200;
            }else{
                console.log("JSON msg probably received");
                this.updateMessage(event);
            }

        };
    }

    updateMessage=(event) => {
        console.log(event.data);
        var msg  = JSON.parse(event.data);
        if (msg["message"] !== undefined){
            console.log(msg.message);
            document.getElementById("message").innerHTML = "Message Received: " + msg.message;
        }
    };

    updateImage = (event) =>{
        var blob = new Blob([event.data]);
        console.log("Raw received, bytes: " + blob.size);

        document.querySelector("#image").src = URL.createObjectURL(event.data);
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
